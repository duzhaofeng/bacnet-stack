#ifndef HARDWARE_H
#define HARDWARE_H

#include <rtthread.h>
#define DBG_SECTION_NAME               "bacnet"
#define DBG_LEVEL                      DBG_INFO
#include <rtdbg.h>

#define HAVE_BINARY_OUTPUTS 0

#define MAX_BINARY_OUTPUTS 8
#define MAX_BINARY_INPUTS  16

#define SETTING_MB_ADDRESS  0
#define SETTING_MB_PARITY   1
#define SETTING_MB_BAUDRATE 2
#define SETTING_BN_ADDRESS  3
#define SETTING_BN_BAUDRATE 4
#define SETTING_BN_OBJECT   5

extern uint8_t bn_address;
extern rt_uint16_t read_di(void);
extern rt_bool_t save_config(rt_uint8_t type, rt_uint32_t value);

#endif
