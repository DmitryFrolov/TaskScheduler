# TaskScheduler
Simple and portable task scheduler written in C++
## Available shedulers
```
inst.ScheduleEndless(repeat period, function)  -  schedule function for endless calls
inst.ScheduleAt(time point, function) - schedule function at perticular timepoint
inst.SchedulerRepeatable(repeat period, function, repeat count) - schedule function for particular amount of calls
```
