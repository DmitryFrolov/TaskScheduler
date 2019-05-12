cmake_minimum_required(VERSION 3.10)

set(SCHEDULER task_scheduler)

set(HEADERS TaskScheduler.h)
set(SOURCES TaskScheduler.cpp)

add_library(${SCHEDULER} ${SOURCES} ${HEADERS})
target_include_directories(${SCHEDULER}
                           PUBLIC ./)
target_link_libraries(${SCHEDULER}
                      PUBLIC pthread)

add_executable(${SCHEDULER}_exe main.cpp ${SOURCES} ${HEADERS})
target_include_directories(${SCHEDULER}_exe
                           PUBLIC ./)
target_link_libraries(${SCHEDULER}_exe
                      PUBLIC ${SCHEDULER})
