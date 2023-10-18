#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#include "main.h"

void Display_DHT11_Value(void);

//void DisplaySMG_LED(void);


void Display_DHT11_Temperature_Value(void);
void Display_DHT11_Humidity_Value(void);

void Display_GMT(uint8_t hours,uint8_t minutes);


void Display_Error_Digital(uint8_t errnumbers,uint8_t sel);



void Display_TimeColon_Blink_Fun(void);

void Display_Timing_Blink(uint8_t hours,uint8_t minutes);
#endif 
