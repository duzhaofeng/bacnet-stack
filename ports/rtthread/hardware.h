#ifndef HARDWARE_H
#define HARDWARE_H

#include <rtthread.h>
#define DBG_SECTION_NAME               "bacnet"
#define DBG_LEVEL                      DBG_INFO
#include <rtdbg.h>

#define HAVE_BINARY_OUTPUTS 0

#define MAX_BINARY_OUTPUTS 8
#define MAX_BINARY_INPUTS  16

extern rt_uint16_t read_di(void);

#endif
