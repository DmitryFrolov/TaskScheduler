#include <iostream>
#include <TaskScheduler.h>

int main(int argc, char const *argv[])
{
    Scheduler sch;
    sch.ScheduleEndless(std::chrono::seconds(1), [&]{
        std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(p);
        std::cout << "Endless calls " << std::ctime(&t) << std::endl;
    });

    sch.ScheduleAt(std::chrono::system_clock::now() + std::chrono::seconds(20), [&]{
        std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(p);
        std::cout << "At call" << std::ctime(&t) << std::endl;
        exit(0);
    });

    sch.ScheduleRepeatable(std::chrono::seconds(1), [&]{
        std::cout << "Limited calls" << std::endl;
    }, 3);
    while (true);
    return 0;
}
