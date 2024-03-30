#pragma once

#include <list>
#include <thread/worker_thread.h>

namespace yazi
{
    namespace thread
    {
        class ThreadPool
        {
            SINGLETON(ThreadPool);
        public:
            void create(int threads);
            void assign(Task * task);

            WorkerThread * get();
            void put(WorkerThread * thread);
            bool empty();

        private:
            int m_threads = 0;
            std::list<WorkerThread *> m_pool;
            Mutex m_mutex;
            Condition m_cond;
        };
    }
}
