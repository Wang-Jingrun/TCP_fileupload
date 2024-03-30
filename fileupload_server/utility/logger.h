#pragma once

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <mutex>
#include <string>
using std::string;

#include <ctime>
#include <cstdarg>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <utility/singleton.h>

namespace yazi
{
    namespace utility
    {
        #define log_debug(format, ...) \
            Singleton<Logger>::instance()->log(Logger::LOG_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

        #define log_info(format, ...) \
            Singleton<Logger>::instance()->log(Logger::LOG_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

        #define log_warn(format, ...) \
            Singleton<Logger>::instance()->log(Logger::LOG_WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)

        #define log_error(format, ...) \
            Singleton<Logger>::instance()->log(Logger::LOG_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

        #define log_fatal(format, ...) \
            Singleton<Logger>::instance()->log(Logger::LOG_FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

        class Logger
        {
            SINGLETON(Logger);

        public:
            enum Level
            {
                LOG_DEBUG = 0,
                LOG_INFO,
                LOG_WARN,
                LOG_ERROR,
                LOG_FATAL,
                LOG_COUNT
            };

            void open(const string & filename);
            void close();
            void log(Level level, const char* file, int line, const char* format, ...);
            void set_level(int level);
            void set_console(bool console);
            void set_max_size(int bytes);

        private:
            void rotate();
            void sleep(int milliseconds);
            void localtime(struct tm * time_info, const time_t * ticks);

        private:
            string m_filename;
            std::ofstream m_ofs;
            int m_max = 0;
            int m_len = 0;
            int m_level = LOG_DEBUG;
            bool m_console = true;
            static const char * s_level[LOG_COUNT];
            std::mutex m_mutex;
        };
    }
}
