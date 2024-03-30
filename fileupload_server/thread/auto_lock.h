#pragma once

#include <thread/mutex.h>

namespace yazi
{
    namespace thread
    {
        class AutoLock
        {
        public:
            AutoLock() = delete;
            AutoLock(Mutex * mutex);
            ~AutoLock();

        private:
            Mutex * m_mutex;
        };
    }
}
