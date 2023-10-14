#ifndef __BSP_UART_H
#define __BSP_UART_H
#include "main.h"

uint8_t Power_ReadParam_OnOff(uint8_t reg);

typedef struct{

   uint8_t gTimer_receive_times;



}usart_rx_tx_t;

extern usart_rx_tx_t usart_t;


#endif 


