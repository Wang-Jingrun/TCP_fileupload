#pragma once

#include <pthread.h>

namespace yazi
{
    namespace thread
    {
        class Mutex
        {
            friend class Condition;
        public:
            Mutex();
            ~Mutex();

            // lock
            void lock();

            // try to lock
            bool try_lock();

            // unlock
            void unlock();

        private:
            pthread_mutex_t m_mutex;
        };
    }
}
