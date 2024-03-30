#pragma once

#include <string>
using std::string;

#include <utility/system.h>
#include <utility/logger.h>
#include <utility/ini_file.h>
#include <utility/singleton.h>
using namespace yazi::utility;

namespace yazi
{
    namespace frame
    {
        class Server
        {
            SINGLETON(Server);
        public:
            void start();

        private:
            string m_ip;
            int m_port = 0;
            int m_threads = 0;
            int m_max_conns = 0;
            int m_wait_time = 0;
            int m_log_level = 0;
        };
    }
}
