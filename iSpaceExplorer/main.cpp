/****************************************************************************************************************/
/*                                                                                                              */ 
/*                                                                                                              */
/*   Bogdan Mihalcea                                                                                            */ 
/*   bogdanmihalcea@yahoo.com                                                                                   */
/*                                                                                                              */  
/****************************************************************************************************************/

#include "pch.h"
#include "hc_sr04.h"
#include "grove_ultra_sonic_v2.h"
#include "analog_sensor_adc832.h"
#include "ir_generic.h"

#include <fstream>

static constexpr int DEFAULT_PIN_OUTPUT = 7;
static constexpr int DEFAULT_PIN_INPUT = 29;
static constexpr int DEFAULT_PIN_CLOCK = 18;
static constexpr int DEFAULT_PIN_CS = 17;
static constexpr int DEFAULT_CHANNEL = 0;
static constexpr int DEFAULT_PACE = 200;

using namespace std;

bool ctrl_c_pressed = false;
bool sensor_base::is_inited = 0;

void sig_handler(int sig)
{
	printf("User cancel\n");
	ctrl_c_pressed = true;
}

bool allow_user_cancel()
{
	struct sigaction sig_struct;
	sig_struct.sa_handler = sig_handler;
	sig_struct.sa_flags = 0;
	sigemptyset(&sig_struct.sa_mask);

	if (sigaction(SIGINT, &sig_struct, NULL) == -1)
	{
		printf("Problem with sigaction\n");
		return false;
	}

	return true;
}

static void show_usage(const string& name)
{
	cerr << "Usage: " << name << " <option(s)> SOURCES\n"
		<< "Options:\n"
		<< "\t-h,--help\tShow this help message\n"
		<< "\t-pi,--pin_input \tSpecify the board pin connected to the sensor's input/trigger\n"
		<< "\t-pt,--pin_output \tSpecify the board pin connected to the sensor's output/data\n"
		<< "\t-pclk,--pin_clock \tSpecify the board pin connected to the sensor's clock\n"
		<< "\t-pcs,--pin_cs \tSpecify the board pin connected to the sensor's cs\n"
		<< "\t-s,--sensor \tSpecify the sensor type (0=Array 1=HC-SRO4, 2=Groove-Ultra-Sonic-V2, 3=keys-ir-obstacle)\n"
		<< "\t-sf, --sensor_file\n"
		<< "\t-t,--time \tMeasurament pace in ms\n"
		<< "\t-d,--debug \tShow debug data\n"
		<< endl;
}

static void compute_standard_deviation(list<float> &values, double& average, double &std_deviation)
{
	double sum = accumulate(values.begin(), values.end(), 0.0);
	average = sum / values.size();

	vector<double> diff(values.size());
	transform(values.begin(), values.end(), diff.begin(), [average](double x) { return x - average; });
	double sq_sum = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
	std_deviation = sqrt(sq_sum / values.size());
}


int main(int argc, char** argv)
{
	bool debug_mode = false;

	if (!allow_user_cancel())
	{
		return -1;
	}

	uint32_t measurement_pace = DEFAULT_PACE;
	uint32_t pin_output = DEFAULT_PIN_OUTPUT;
	uint32_t pin_input = DEFAULT_PIN_INPUT;
	uint32_t pin_cs = DEFAULT_PIN_CS;
	uint32_t pin_clock = DEFAULT_PIN_CLOCK;
	uint32_t pin_channel = DEFAULT_CHANNEL;
	uint32_t sensor_type = 0;
	string sensor_file;

	for (int i = 1; i < argc; ++i)
	{
		string arg = argv[i];
		if ((arg == "-h") || (arg == "--help"))
		{
			show_usage(argv[0]);
			return 0;
		}
		else if ((arg == "-pcs") || (arg == "--pin_cs"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				pin_cs = atoi(argv[++i]); // Increment 'i' so we don't get the argument as the next argv[i].
			}
			else { // Uh-oh, there was no argument to the destination option.
				cerr << "--pin_cs option requires one argument." << endl;
				return 1;
			}
		}
		else if ((arg == "-pclk") || (arg == "--pin_clock"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				pin_clock = atoi(argv[++i]); // Increment 'i' so we don't get the argument as the next argv[i].
			}
			else { // Uh-oh, there was no argument to the destination option.
				cerr << "--pin_clock option requires one argument." << endl;
				return 1;
			}
		}
		else if ((arg == "-po") || (arg == "--pin_output"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				pin_output = atoi(argv[++i]); // Increment 'i' so we don't get the argument as the next argv[i].
			}
			else { // Uh-oh, there was no argument to the destination option.
				cerr << "--pin_output option requires one argument." << endl;
				return 1;
			}
		}
		else if ((arg == "-pch") || (arg == "--pin_channel"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				pin_channel = atoi(argv[++i]); // Increment 'i' so we don't get the argument as the next argv[i].
			}
			else { // Uh-oh, there was no argument to the destination option.
				cerr << "--pin_channel option requires one argument." << endl;
				return 1;
			}
		}
		else if ((arg == "-pi") || (arg == "--pin_input"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				pin_input = atoi(argv[++i]);
			}
			else { // Uh-oh, there was no argument to the destination option.
				cerr << "--pin_input option requires one argument." << endl;
				return 1;
			}
		}
		else if ((arg == "-s") || (arg == "--sensor"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				sensor_type = atoi(argv[++i]); 
			}
			else { // Uh-oh, there was no argument to the destination option.
				cerr << "--sensor option requires one argument." << endl;
				return 1;
			}
		}
		else if ((arg == "-t") || (arg == "--time"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				measurement_pace = atoi(argv[++i]); 
			}
			else { // Uh-oh, there was no argument to the destination option.
				cerr << "--time option requires one argument." << endl;
				return 1;
			}
		}
		else if ((arg == "-d") || (arg == "--debug"))
		{
			debug_mode = true;
		}
		else if ((arg == "-sf") || (arg == "--sensor_file"))
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				sensor_file = argv[++i];
			}
			else { // Uh-oh, there was no argument to the destination option.
				cerr << "--sensor_file option requires one argument." << endl;
				return 1;
			}
			
		}
	}

	list<unique_ptr<sensor_base>> sensors;
	
	std::ifstream infile(sensor_file);	
	std::string sensor_name;

	while (sensor_type > 0 || (!sensor_file.empty() && infile.is_open() && infile >> sensor_type >> sensor_name >> pin_output >> pin_input >> pin_cs >> pin_clock >> pin_channel))
	{
		unique_ptr<sensor_base> s;
		switch (sensor_type)
		{
		case 1:
			s = make_unique<hc_sr04>(sensor_name, pin_output, pin_input);
			break;
		case 2:
			s = make_unique<grove_ultra_sonic_v2>(sensor_name, pin_output);
			break;
		case 3:
			s = make_unique<analog_sensor_adc832>(sensor_name, pin_output, pin_input, pin_cs, pin_clock, pin_channel);
			break;
		case 4:
			s = make_unique<ir_generic>(sensor_name, pin_output, pin_input);
			break;
		default:
			break;
		}
		
		if (debug_mode)
		{
			s->set_debug_mode(debug_mode);
		}

		sensors.push_back(std::move(s));

		sensor_type = 0;
	}
	

	while (!ctrl_c_pressed)
	{
		for (auto &&s : sensors)
		{
			if (s->name()->find("ir_generic") != string::npos)
			{
				sensor_data data;
				data.i32data1 = 0x40040100;
				data.i16data1 = 0xbcbd;
				s->write(data);

				delay(1);
			}
			
			sensor_data data = s->read();
			
			if (data.result_state == ERROR_SUCCESS)
			{
				char print_buff[100];
				//snprintf(print_buff, sizeof(print_buff), "\n Sensor: %s \t\t Value1:\t%.2f\n", s->name().get()->c_str(), data.data1);
				snprintf(print_buff, sizeof(print_buff), "\n Sensor: %s \t\t Values:\t%.2f \t%.2f \t%x \t%x\n", s->name().get()->c_str(), data.data1, data.data2, data.i32data1, data.i32data2);
				printf("%s", print_buff);
			}
			else
			{
				printf("\n Sensor %s Error: %d", s->name().get()->c_str(), data.result_state);
			}
		}
		delay(measurement_pace);
	}
	return 0;
}

