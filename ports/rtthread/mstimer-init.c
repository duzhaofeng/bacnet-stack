#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <limits.h>
#include <sys/time.h>
#include "bacnet/basic/sys/mstimer.h"

/* start time for the clock */
static struct timespec start;

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
