#pragma once
#include "log/configuration.h"

// Makros
#define Log(message,message_type) rxs::log::write(message,message_type,__LINE__)
#define L_INFO(message) rxs::log::write(message,rxs::log::INFO,__LINE__)
#define L_WARN(message) rxs::log::write(message,rxs::log::WARN,__LINE__)
#define L_ERROR(message) rxs::log::write(message,rxs::log::ERROR,__LINE__)
#define LOGINIT() rxs::log::init()

// Declarations
namespace rxs
{
	namespace log
	{
		typedef std::string MESSAGE_FLAG;
		MESSAGE_FLAG INF = "Info";
		MESSAGE_FLAG WARN = "Warning";
		MESSAGE_FLAG ERROR = "Error";

		int init() throw(std::runtime_error);
		int deinit(); 
		inline void write(double message, MESSAGE_FLAG message_type, unsigned int line, std::thread::id thread_id);
		inline void write(std::string message, MESSAGE_FLAG message_type, unsigned int line, std::thread::id thread_id);
		void run_listener() throw(std::runtime_error);
		void add_record(std::string message, MESSAGE_FLAG message_type, int line, std::thread::id thread_id);

		class LOG
		{
			std::fstream logfile;
			const char *path;

			std::queue <std::string> records;
			std::thread *recorder_handle;

			unsigned long sampling;
			std::thread *consumer_handle;
			bool should_close;
			int consumer_state;

			std::string get_time() noexcept;
			
			friend void run_listener() throw(std::runtime_error);
			friend void add_record(std::string message, MESSAGE_FLAG message_type, int line, std::thread::id thread_id);			
			friend inline void write(std::string message, MESSAGE_FLAG message_type, unsigned int line, std::thread::id thread_id);
			friend int deinit();

		public:	
			int init(const char *name) throw(std::runtime_error);
			~LOG()
			{
				if (consumer_state == 1) deinit();	
			}
		};

		LOG Log;

		int rxs::log::LOG::init(const char *name) throw(std::runtime_error)
		{
			path = name;
			logfile.open(name, std::ios::out);
			if (!logfile.good())
			{
				consumer_state = -1;
				throw std::runtime_error("file openning error has ocured");
			}

			logfile << "Log file:	" << name;
			logfile << "	Main thread: " << std::this_thread::get_id();
			logfile << "	Configuration: " << CONF;
			logfile << "	Platform: " << PLATFORM;
			logfile << "	Date: " << __DATE__ << " " << __TIME__ << std::endl;
			logfile.close();

			std::thread consumer(&run_listener);
			consumer.detach();
			consumer_handle = &consumer;
			return 0;
		}

		int init() throw(std::runtime_error)
		{
			std::fstream conf;
			const char *conf_path = "./logs/config.txt";
			if(_mkdir("logs") != EEXIST)
			{
				conf.open(conf_path, std::ios::out);
				if (!conf.good()) throw std::runtime_error("./logs/config.txt file couldn't be created");
				conf << 0;
				conf.close();
			}

			conf.open("./logs/config.txt", std::ios::in | std::ios::out);
			if (!conf.good()) throw std::runtime_error("./logs/config.txt file couldn't be opened");		
			int nr;
			conf >> nr;
			conf << (nr + 1);
			conf.close();

			std::string name = "./logs/log" + std::to_string(nr) + ".txt";	
			const char * log_path = name.c_str();
			Log.init(log_path);
			return 0;
		}

		void run_listener() throw(std::runtime_error)
		{
			std::chrono::milliseconds(100);
			Log.logfile.open(Log.path, std::ios::app);
			if (!Log.logfile.good()) throw std::runtime_error("file openning error has ocured");

			Log.should_close = false;
			
			while (!Log.should_close)
			{
				while(!Log.records.empty())
				{
						Log.logfile << Log.records.front() << std::endl;
						Log.records.pop();				
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(Log.sampling));
			} 
			Log.logfile.close();
		}

		std::string rxs::log::LOG::get_time() noexcept
		{
			char bufor[9];
			time_t date;
			tm timeTM;

			time(&date);
			gmtime_s(&timeTM, &date);
			strftime(bufor, sizeof(bufor), "%H:%M:%S", &timeTM);
			return bufor;
		}

		void add_record(std::string message, log::MESSAGE_FLAG message_type, int line, std::thread::id thread_id)
		{
					
			std::string out_message = "[" + Log.get_time() + "]	" +
	#ifdef _DEBUG
				" line: " + std::to_string(line) +
	#endif
				 "	" + message_type + "	: " + message;
			
			Log.records.push(out_message);	
		}

		inline void write(std::string message, log::MESSAGE_FLAG message_type,unsigned int line = 0,std::thread::id thread_id = std::this_thread::get_id())
		{	
			std::thread queue_thread(add_record, message, message_type, line, thread_id);
			Log.recorder_handle = &queue_thread;
			queue_thread.detach();			
		}
		inline void write(double message, MESSAGE_FLAG message_type,unsigned int line = 0, std::thread::id thread_id = std::this_thread::get_id())
		{
			write(std::to_string(message), message_type, line, thread_id );
		}
		int deinit()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			Log.should_close = true;
			if (Log.consumer_state == 1)
			{
				while (Log.consumer_state != 1)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(Log.sampling));
				}
			}
			else if (Log.consumer_state == -1)
			{
				while (!Log.records.empty())
				{
					std::cout << Log.records.front() << std::endl;
					Log.records.pop();
				}
				return -1;
			}			
			return 0;
		}
	}
}
