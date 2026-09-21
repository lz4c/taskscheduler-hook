## TaskScheduler

Simple taskscheduler implementation that runs jobs on a separate thread.

**MADE FOR M-SERIES MACS**

Usage

```c
#include "taskscheduler.hpp"

taskscheduler::TaskScheduler scheduler;

scheduler.schedule([]()
{
    // Your job
});
```

Multiple jobs:

```c
scheduler.schedule([]()
{
    // Job 1
});

scheduler.schedule([]()
{
    // Job 2
});
```

Jobs are executed in the order they are added
