#include <TaskScheduler.h>

Scheduler::Scheduler()
    : go_on(true),
    thread(std::make_unique<std::thread>([this]() { ThreadLoop(); }))
    {}

Scheduler::~Scheduler()
{
    go_on = false;
    thread->join();
}

void Scheduler::ScheduleAt(const std::chrono::system_clock::time_point &time, std::function<void()> &&func)
{
    std::function<void()> threadFunc = [func]() {
        std::thread t(func);
        t.detach();
    };
    ScheduleTask(time, std::move(threadFunc));
}

void Scheduler::ScheduleEndless(std::chrono::system_clock::duration interval, std::function<void()> func)
{
    this->ScheduleRepeatable(interval, func, -1);
}

void Scheduler::ScheduleRepeatable(std::chrono::system_clock::duration interval, std::function<void()> func, int repeats)
{
    std::function<void()> threadFunc = [func]() {
        std::thread t(func);
        t.detach();
    };
    this->ScheduleEveryIntern(interval, threadFunc, repeats);
}

void Scheduler::ScheduleTask(const std::chrono::system_clock::time_point &time, std::function<void()> &&func)
{
    tasks.push(function_timer(std::move(func), time));
}

void Scheduler::ScheduleEveryIntern(std::chrono::system_clock::duration interval, std::function<void()> func, int repeats)
{
    if(repeats != 0) {
        std::function<void()> waitFunc = [this, interval, func, repeats]() {
            func();
            this->ScheduleEveryIntern(interval, func, repeats - 1);
        };
        ScheduleTask(std::chrono::system_clock::now() + interval, std::move(waitFunc));
    }
}

void Scheduler::ThreadLoop()
{
    while (go_on)
    {
        auto now = std::chrono::system_clock::now();
        while (!tasks.empty() && tasks.top().time <= now)
        {
            function_timer ftimer = tasks.top();
            ftimer.call();
            tasks.pop();
        }

        if (tasks.empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        else
        {
            std::this_thread::sleep_for(tasks.top().time - std::chrono::system_clock::now());
        }
    }
}