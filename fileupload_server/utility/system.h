#pragma once

#include <string>
using std::string;

namespace yazi
{
    namespace utility
    {
        class System
        {
        public:
            System() = default;
            ~System() = default;

            void init();
            string get_root_path();

        private:
            void core_dump();

        private:
            string m_root_path;
        };
    }
}
