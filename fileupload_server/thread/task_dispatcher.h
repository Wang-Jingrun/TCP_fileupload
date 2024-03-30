#pragma once

#include <list>
#include <thread/thread.h>
#include <thread/thread_pool.h>
#include <thread/task.h>

namespace yazi
{
    namespace thread
    {
        class TaskDispatcher : public Thread
        {
            SINGLETON(TaskDispatcher);
        public:
            void init(int threads);
            void assign(Task * task);
            void handle(Task * task);
            bool empty();
            virtual void run();

        protected:
            std::list<Task *>   m_queue;
            Mutex               m_mutex;
            Condition           m_cond;
        };
    }
}
