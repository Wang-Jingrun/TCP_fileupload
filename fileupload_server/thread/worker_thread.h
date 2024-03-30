#pragma once

#include <thread/thread.h>
#include <thread/mutex.h>
#include <thread/auto_lock.h>
#include <thread/condition.h>
#include <thread/task.h>

#include <utility/logger.h>
using namespace yazi::utility;

namespace yazi
{
    namespace thread
    {
        class WorkerThread : public Thread
        {
        public:
            WorkerThread();
            ~WorkerThread();

            virtual void run();
            void assign(Task * task);
            static void cleanup(void * ptr);

        private:
            Task *      m_task;
            Mutex       m_mutex;
            Condition   m_cond;
        };
    }
}
