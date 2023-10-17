#ifndef __BSP_UART_H
#define __BSP_UART_H
#include "main.h"



typedef struct{

   uint8_t gTimer_receive_times;



}usart_rx_tx_t;

extern usart_rx_tx_t usart_t;

uint8_t Power_ReadParam_OnOff(uint8_t reg);


uint8_t Wifi_LoginParam_On(void);

#endif 


