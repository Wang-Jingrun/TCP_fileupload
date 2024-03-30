#pragma once

#include <signal.h>
#include <pthread.h>

namespace yazi
{
    namespace thread
    {
        class Thread
        {
        public:
            Thread();
            virtual ~Thread();

            virtual void run() = 0;

            void start();
            void stop();

        protected:
            static void * thread_func(void * ptr);

        protected:
            pthread_t   m_tid;
        };
    }
}
