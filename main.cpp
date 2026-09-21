#include "taskscheduler.hpp"

namespace taskscheduler
{
    TaskScheduler::TaskScheduler()
    {
        start();
    }

    TaskScheduler::~TaskScheduler()
    {
        stop();
    }

    void TaskScheduler::start()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_running)
            return;

        m_stopping = false;
        m_running = true;

        m_thread = std::thread(&TaskScheduler::worker, this);
    }

    void TaskScheduler::stop()
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            if (!m_running)
                return;

            m_stopping = true;
        }

        m_condition.notify_one();

        if (m_thread.joinable())
            m_thread.join();

        std::lock_guard<std::mutex> lock(m_mutex);

        m_running = false;

        while (!m_queue.empty())
            m_queue.pop();
    }

    void TaskScheduler::schedule(std::function<void()> task)
    {
        if (!task)
            return;

        {
            std::lock_guard<std::mutex> lock(m_mutex);

            if (m_stopping)
                return;

            m_queue.push(std::move(task));
        }

        m_condition.notify_one();
    }

    bool TaskScheduler::isRunning() const
    {
        return m_running;
    }

    void TaskScheduler::worker()
    {
        while (true)
        {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(m_mutex);

                m_condition.wait(lock, [this]
                {
                    return m_stopping || !m_queue.empty();
                });

                if (m_stopping && m_queue.empty())
                    break;

                if (!m_queue.empty())
                {
                    task = std::move(m_queue.front());
                    m_queue.pop();
                }
            }

            if (task)
                task();
        }
    }
}
