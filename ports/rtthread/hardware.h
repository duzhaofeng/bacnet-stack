#ifndef HARDWARE_H
#define HARDWARE_H

#include <rtthread.h>
#define DBG_SECTION_NAME               "bacnet"
#define DBG_LEVEL                      DBG_INFO
#include <rtdbg.h>

#define MAX_BINARY_OUTPUTS 16
#define MAX_BINARY_INPUTS  16

#define SETTING_MB_ADDRESS  0
#define SETTING_MB_PARITY   1
#define SETTING_MB_BAUDRATE 2
#define SETTING_BN_ADDRESS  3
#define SETTING_BN_BAUDRATE 4
#define SETTING_BN_OBJECT   5

extern uint16_t bn_port;
extern uint8_t bn_address;
extern uint8_t di_num;
extern uint8_t do_num;
extern uint8_t ai_num;
extern uint8_t ao_num;
extern uint32_t ip_address;
extern uint8_t ip_mask;
extern uint16_t read_di(void);
extern void write_do(uint16_t value);
extern uint32_t read_ai_float(float *buf, uint32_t idx, uint32_t num);
extern void write_ao_float(float *buf, uint32_t idx, uint32_t num);
extern rt_bool_t save_config(uint8_t type, uint32_t value);

#endif
