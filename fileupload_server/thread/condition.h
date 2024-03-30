#pragma once

#include <thread/mutex.h>

namespace yazi
{
    namespace thread
    {
        class Condition
        {
        public:
            Condition();
            ~Condition();

            void wait(Mutex * mutex);
            void signal();
            void broadcast();

        protected:
            pthread_cond_t  m_cond;
        };
    }
}
