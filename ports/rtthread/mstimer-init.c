#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <limits.h>
#include <sys/time.h>
#include <rthw.h>
#include <rtthread.h>
#include "bacnet/basic/sys/mstimer.h"

#define MILLISECOND_PER_SECOND  1000UL
#define MICROSECOND_PER_SECOND  1000000UL
#define NANOSECOND_PER_SECOND   1000000000UL

#define MILLISECOND_PER_TICK    (MILLISECOND_PER_SECOND / RT_TICK_PER_SECOND)
#define MICROSECOND_PER_TICK    (MICROSECOND_PER_SECOND / RT_TICK_PER_SECOND)
#define NANOSECOND_PER_TICK     (NANOSECOND_PER_SECOND  / RT_TICK_PER_SECOND)

/* start time for the clock */
static struct timespec start;

int clock_gettime(clockid_t clockid, struct timespec *tp)
{
    int ret = 0;
    int tick;
    rt_base_t level;

    level = rt_hw_interrupt_disable();
    tick = rt_tick_get(); /* get tick */
    tp->tv_sec = tick / RT_TICK_PER_SECOND;
    tp->tv_nsec = (tick % RT_TICK_PER_SECOND) * MICROSECOND_PER_TICK * 1000;
    rt_hw_interrupt_enable(level);

    return ret;
}

static unsigned long timeGetTime(void)
{
    struct timespec now;
    unsigned long ticks;

    clock_gettime(CLOCK_REALTIME, &now);

    ticks = (now.tv_sec - start.tv_sec) * 1000 +
        (now.tv_nsec - start.tv_nsec) / 1000000;

    return ticks;
}

/**
 * @brief returns the current millisecond count
 * @return millisecond counter
 */
unsigned long mstimer_now(void)
{
    return timeGetTime();
}

/**
 * @brief Initialization for timer
 */
void mstimer_init(void)
{
    clock_gettime(CLOCK_REALTIME, &start);
}
