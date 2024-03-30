#include <thread/mutex.h>
using namespace yazi::thread;

Mutex::Mutex()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&m_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_mutex);
}

void Mutex::lock()
{
    pthread_mutex_lock(&m_mutex);
}

bool Mutex::try_lock()
{
    return pthread_mutex_trylock(&m_mutex) == 0;
}

void Mutex::unlock()
{
    pthread_mutex_unlock(&m_mutex);
}


