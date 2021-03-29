#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <time.h>
#include <queue>
#include <stdexcept>

#ifndef _log_folder_path
#define _log_folder_path "./logs"
#endif

#ifndef _log_error_path
#define _log_error_path "./logs/error.txt"
#endif

#ifndef _log_config_path
#define _log_config_path "./logs/config.txt"
#endif


namespace rxs
{
	namespace log
	{
		int get_last_log_nr()
		{
			std::fstream conf;
			conf.open(_log_config_path, ios::out);
			if (!conf.good()) throw std::runtime_error("file config.txt couldn't be opened");
			int nr;
			conf >> nr;
		}
	}
}