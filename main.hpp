#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace taskscheduler
{
    class TaskScheduler
    {
    public:
        TaskScheduler();
        ~TaskScheduler();

        TaskScheduler(const TaskScheduler&) = delete;
        TaskScheduler& operator=(const TaskScheduler&) = delete;

        void schedule(std::function<void()> task);

        void start();
        void stop();

        bool isRunning() const;

    private:
        void worker();

        std::queue<std::function<void()>> m_queue;
        std::mutex m_mutex;
        std::condition_variable m_condition;
        std::thread m_thread;

        bool m_running = false;
        bool m_stopping = false;
    };
}
