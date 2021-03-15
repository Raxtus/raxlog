#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <time.h>
#include <queue>
#include <stdexcept>

#ifdef _DEBUG
	#define CONF "Debug"
#else 
	#define CONF "Release"
#endif

#ifdef _M_IX86
#define PLATFORM "x86"
#endif

#ifdef _M_AMD64
#define PLATFORM "x64"
#endif

#ifdef _M_ARM
#define PLATFORM "ARM"
#endif

#ifdef _M_ARM64
#define PLATFORM "ARM64"
#endif

namespace rxs
{
	namespace log
	{
		enum MODE
		{
			INFO = 0,
			WARRING = 1,
			ERROR = 2
		};

		class LOG
		{
			std::fstream log;
			bool log_used;
			const char *path;
		
			friend void init(const char *name);			
			friend void save(std::string text);
			
		};

		
		LOG rec;	
		

		void init(const char *name)
		{

			rec.path = name;
			rec.log.open(name, std::ios::out);
			if (!rec.log.good())throw std::runtime_error("file couldn't be opened");

			rec.log << "Log file:	" << name;
			rec.log << "	Main thread: " << std::this_thread::get_id();
			rec.log << "	Configuration: " << CONF;
			rec.log << "	Platform: " << PLATFORM;
			rec.log << "	Date: " << __DATE__ << " " << __TIME__ << std::endl;
			rec.log.close();
			rec.log_used = false;
			
		}
		void save(std::string text)
		{
			int n = 0;
			while (rec.log_used)
			{
				n++;
			}
			rec.log.open(rec.path, std::ios::app);
			if (!rec.log.good())throw std::runtime_error("file couldn't be opened");
			rec.log << text << std::endl;
			rec.log.close();
		}
		
		void write(std::string message, MODE m, std::thread::id thread_id = std::this_thread::get_id(), int line = __LINE__)
		{
			char bufor[9];
			time_t date;
			std::string s_mode[3] = { "info","warrning","error" };

			time(&date);
			tm timeTM;
			gmtime_s(&timeTM,&date);

			strftime(bufor, sizeof(bufor), "%H:%M:%S", &timeTM);

			std::string m_buff = message;
			std::string t_buff = bufor;
			std::string o_message = "[" + t_buff + "]	" + " line: " + std::to_string(line) + "	" + s_mode[m] + "	: " + m_buff;
					
			try
			{
				save(o_message);
			}
			catch (std::runtime_error&)
			{
				abort();
			}
		}
		void write(double message, MODE m, std::thread::id thread_id = std::this_thread::get_id(), int line = __LINE__)
		{
			char bufor[9];
			time_t date;
			std::string s_mode[3] = { "info","warrning","error" };

			time(&date);
			tm timeTM;
			gmtime_s(&timeTM, &date);

			strftime(bufor, sizeof(bufor), "%H:%M:%S", &timeTM);

			std::string m_buff = std::to_string(message);
			std::string t_buff = bufor;
			std::string o_message = "[" + t_buff + "]	" + " line: " + std::to_string(line) + "	" + s_mode[m] + "	: " + m_buff;

			try
			{
				save(o_message);
			}
			catch (std::runtime_error &e)
			{
				std::cerr << e.what();
			}
		}
	}
}