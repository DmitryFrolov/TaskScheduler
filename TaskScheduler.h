#include <queue>
#include <chrono>
#include <thread>
#include <memory>
#include <functional>

namespace
{
// Special structure
struct function_timer
{
    std::function<void()> func;
    std::chrono::system_clock::time_point time;

    function_timer() = delete;

    function_timer(std::function<void()> &&f, const std::chrono::system_clock::time_point &t)
        : func(f),
          time(t)
    { }

    // Note: we want our priority_queue to be ordered in terms of
    // smallest time to largest, hence the > in operator<. This isn't good
    // practice - it should be a separate struct. Done this for brevity.
    bool operator<(const function_timer &rhs) const
    {
        return time > rhs.time;
    }

    void call()
    {
        func();
    }
};
}

// TODO: also it would be a good idea to implement a way to remove endless task from the queue
class Scheduler
{
private:
    std::priority_queue<function_timer> tasks;
    std::unique_ptr<std::thread> thread;
    bool go_on;

    Scheduler &operator=(const Scheduler &rhs) = delete;
    Scheduler(const Scheduler &rhs) = delete;

public:
    Scheduler();
    ~Scheduler();
    // Schedule single call of @func when the @time comes
    void ScheduleAt(const std::chrono::system_clock::time_point &time, std::function<void()> &&func);
    // Same with @time in format "%s %M %H %d %m %Y" "sec min hour date month year"
    void ScheduleAt(const std::string &time, std::function<void()> func);
    // Schedule endless calls of @func with given @inverval
    void ScheduleEndless(std::chrono::system_clock::duration interval, std::function<void()> func);
    // Schedule @repeats amount of calls of @func with given @inverval
    void ScheduleRepeatable(std::chrono::system_clock::duration interval, std::function<void()> func, int repeats);

private:
    // Insert task into tasks pool
    void ScheduleTask(const std::chrono::system_clock::time_point &time, std::function<void()> &&func);
    /// Shedule @func call after @interval time if @repeats not equals to 0
    /// Use repeats < 0 for endless scheduling
    void ScheduleEveryIntern(std::chrono::system_clock::duration interval, std::function<void()> func, int repeats);
    void ThreadLoop();
};
