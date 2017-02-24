/****************************************************************************************************************/
/*                                                                                                              */
/*   Copyright (c) Bogdan Mihalcea 2017                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "pch.h"
#include "hc_sr04.h"
#include "grove_ultra_sonic_v2.h"
#include "adc832_analog.h"
#include "ir_generic.h"
#include "groove_lcd.h"
#include "dht11.h"
#include "rth03.h"

#include <fstream>

enum SUPPORTED_SENSORS
{
	HC_SR04 = 1,
	GROOVE_ULTRA_SONIC_V2,
	ADC832_ANALOG,
	IR_GENERIC,
	DHT11,
	RTH03,
};

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
		<< "\t-s,--sensor \tSpecify the sensor type (0=Array 1=HC-SRO4, 2=Groove-Ultra-Sonic-V2, 3=adc832, 4=keys-ir-obstacle, 5=dht11, 6=rth03)\n"
		<< "\t-sf, --sensor_file\n"
		<< "\t-t,--time \tMeasurament pace in ms\n"
		<< "\t-d,--debug \tShow debug data\n"
		<< endl;
}

int main(int argc, char** argv)
{
	bool verbosity_mode = false;

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
			verbosity_mode = true;
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
		case HC_SR04:
			s = make_unique<hc_sr04>(sensor_name, pin_output, pin_input);
			break;
		case GROOVE_ULTRA_SONIC_V2:
			s = make_unique<grove_ultra_sonic_v2>(sensor_name, pin_output);
			break;
		case ADC832_ANALOG:
			s = make_unique<adc832_analog>(sensor_name, pin_output, pin_input, pin_cs, pin_clock, pin_channel);
			break;
		case IR_GENERIC:
			s = make_unique<ir_generic>(sensor_name, pin_output, pin_input);
			break;
		case DHT11:
			s = make_unique<dht11>(sensor_name, pin_output);
			break;
		case RTH03:
			s = make_unique<rht03>(sensor_name, pin_output);
			break;
		default:
			break;
		}
		
		if (verbosity_mode)
		{
			s->set_verbosity(verbosity_mode);
		}

		sensors.push_back(std::move(s));

		sensor_type = 0;
	}
	
	groove_lcd lcd(0x62);

	while (!ctrl_c_pressed)
	{
		for (auto &&s : sensors)
		{
			lcd.print(s->name());
			if (s->name().find("ir_generic") != string::npos)
			{

				if (pin_input != 0)
				{
					ir_generic_data data;
					data.set_data(0xFFFFFFFF, 0x0000, ERROR_SUCCESS); // 0x40040100, 0xbcbd;
					(dynamic_cast<ir_generic*>(s.get()))->set_data(data);

					delay(2000);

					continue;
				}
			}
			
			auto err = s->sample();

			if ( err == ERROR_SUCCESS)
			{
				char print_buff[100];
				snprintf(print_buff, sizeof(print_buff), "\n Sensor: %s \t\t Data:%s\n", s->name().c_str(), const_cast<sensor_data&>(s->get_data()).to_string().c_str());
				printf("%s", print_buff);
			}
			else
			{
				printf("\n Sensor %s Error: %d", s->name().c_str(), err);
			}
		}
	
		delay(measurement_pace);
	}
	return 0;
}

