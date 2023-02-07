#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rtdevice.h"

/* Local includes */
#include "bacnet/datalink/mstp.h"
#include "bacnet/basic/sys/fifo.h"
#include "bacnet/datalink/dlmstp.h"
#include "rs485.h"

// #include <sys/select.h>
// #include <sys/time.h>

/* modbus slave serial device */
static rt_serial_t *serial = NULL;

extern unsigned int RS485_Baud;

static char *RS485_Port_Name = "uart1";

/* Ring buffer for incoming bytes, in order to speed up the receiving. */
static FIFO_BUFFER Rx_FIFO;
/* buffer size needs to be a power of 2 */
static uint8_t Rx_Buffer[4096];

static rt_err_t serial_rx_ind(rt_device_t dev, rt_size_t size)
{
    uint8_t buf[1024];
    rt_size_t n = serial->parent.read(&(serial->parent), 0, buf, size > sizeof(buf) ? sizeof(buf) : size);
    FIFO_Add(&Rx_FIFO, &buf[0], n);
    return RT_EOK;
}

/*********************************************************************
 * DESCRIPTION: Configures the interface name
 * RETURN:      none
 * ALGORITHM:   none
 * NOTES:       none
 *********************************************************************/
void RS485_Set_Interface(char *ifname)
{
    /* note: expects a constant char, or char from the heap */
    if (ifname) {
        RS485_Port_Name = ifname;
    }
}

/*********************************************************************
 * DESCRIPTION: Returns the interface name
 * RETURN:      none
 * ALGORITHM:   none
 * NOTES:       none
 *********************************************************************/
const char *RS485_Interface(void)
{
    return RS485_Port_Name;
}

/****************************************************************************
 * DESCRIPTION: Returns the baud rate that we are currently running at
 * RETURN:      none
 * ALGORITHM:   none
 * NOTES:       none
 *****************************************************************************/
uint32_t RS485_Get_Baud_Rate(void)
{
    return RS485_Baud;
}

/****************************************************************************
 * DESCRIPTION: Sets the baud rate for the chip USART
 * RETURN:      none
 * ALGORITHM:   none
 * NOTES:       none
 *****************************************************************************/
bool RS485_Set_Baud_Rate(uint32_t baud)
{
    bool valid = true;

    switch (baud) {
        case 9600:
        case 19200:
        case 38400:
        case 57600:
        case 76800:
        case 115200:
            break;
        default:
            valid = false;
            break;
    }
    if (valid) {
        return save_config(SETTING_BN_BAUDRATE, baud);
    }
    return valid;
}

/****************************************************************************
 * DESCRIPTION: Transmit a frame on the wire
 * RETURN:      none
 * ALGORITHM:   none
 * NOTES:       none
 *****************************************************************************/
void RS485_Send_Frame(
    volatile struct mstp_port_struct_t *mstp_port, /* port specific data */
    uint8_t *buffer, /* frame to send (up to 501 bytes of data) */
    uint16_t nbytes)
{ /* number of bytes of data (up to 501) */
    rt_thread_mdelay(Tturnaround * 1000 / RS485_Get_Baud_Rate());
    serial->ops->control(serial, RT_DEVICE_CTRL_CLR_INT, (void *)RT_DEVICE_FLAG_INT_RX);
    serial->parent.write(&(serial->parent), 0, buffer, nbytes);
    serial->ops->control(serial, RT_DEVICE_CTRL_SET_INT, (void *)RT_DEVICE_FLAG_INT_RX);
    /* per MSTP spec, sort of */
    if (mstp_port) {
        mstp_port->SilenceTimerReset((void *)mstp_port);
    }
}

/****************************************************************************
 * DESCRIPTION: Get a byte of receive data
 * RETURN:      none
 * ALGORITHM:   none
 * NOTES:       none
 *****************************************************************************/
void RS485_Check_UART_Data(volatile struct mstp_port_struct_t *mstp_port)
{
    /* check for data */
    if (!FIFO_Empty(&Rx_FIFO)) {
        mstp_port->DataRegister = FIFO_Get(&Rx_FIFO);
        mstp_port->DataAvailable = 1;
    }
}

void RS485_Cleanup(void)
{
    serial->parent.close(&(serial->parent));
}

void RS485_Initialize(void)
{
    rt_device_t dev = RT_NULL;

    LOG_I("RS485: Initializing %s", RS485_Port_Name);
    
    dev = rt_device_find(RS485_Port_Name);
    if(dev == RT_NULL)
    {
        /* can not find uart */
        LOG_E("RS485: Can not find %s", RS485_Port_Name);
        return;
    }
    else
    {
        serial = (struct rt_serial_device*)dev;
    }

    /* set serial configure parameter */
    serial->config.baud_rate = RS485_Baud;
    serial->config.stop_bits = STOP_BITS_1;
    serial->config.data_bits = DATA_BITS_8;
    serial->config.parity = PARITY_NONE;
    /* set serial configure */
    serial->ops->configure(serial, &(serial->config));

    /* open serial device */
    if (!rt_device_open(&serial->parent, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX)) {
        rt_device_set_rx_indicate(&serial->parent, serial_rx_ind);
    } else {
        LOG_E("RS485: Can not open %s", RS485_Port_Name);
        return;
    }
    FIFO_Init(&Rx_FIFO, Rx_Buffer, sizeof(Rx_Buffer));
    LOG_I("RS485: Initializing success");
}
