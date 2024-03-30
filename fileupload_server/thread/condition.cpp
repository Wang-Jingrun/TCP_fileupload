#include <thread/condition.h>
using namespace yazi::thread;

Condition::Condition()
{
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    pthread_cond_init(&m_cond, &attr);
    pthread_condattr_destroy(&attr);
}

Condition::~Condition()
{
    pthread_cond_destroy(&m_cond);
}

void Condition::wait(Mutex * mutex)
{
    pthread_cond_wait(&m_cond, &(mutex->m_mutex));
}

void Condition::signal()
{
    pthread_cond_signal(&m_cond);
}

void Condition::broadcast()
{
    pthread_cond_broadcast(&m_cond);
}
