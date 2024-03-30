#include <thread/task.h>
using namespace yazi::thread;

Task::Task() : m_data(nullptr)
{
}

Task::Task(void * data) : m_data(data)
{
}

Task::~Task()
{
    m_data = nullptr;
}

void * Task::get_data()
{
    return m_data;
}

void Task::set_data(void * data)
{
    m_data = data;
}
