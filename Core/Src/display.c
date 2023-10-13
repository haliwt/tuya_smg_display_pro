#include "display.h"
#include "smg.h"
#include "cmd_link.h"
#include "run.h"
#include "key.h"
#include "led.h"
#include "bsp_display_dynamic.h"



static void TimeColon_Smg_Blink_Fun(void);


/**********************************************************************
*
*Functin Name: void Display_DHT11_Value(void)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Display_DHT11_Value(void)
{
    
  static uint8_t hum1,hum2; 
  static uint8_t temp1,temp2;

	hum1 =  run_t.gReal_humtemp[0]/10;  //Humidity 
	hum2 =  run_t.gReal_humtemp[0]%10;

	temp1 = run_t.gReal_humtemp[1]/10 ;  // temperature
	temp2 = run_t.gReal_humtemp[1]%10;

    if(run_t.set_temperature_flag==0){
	  TM1639_Write_2bit_TempData(temp1,temp2);
    }
	TM1639_Write_2bit_HumData(hum1,hum2);
	


}  


/**********************************************************************
*
*Functin Name: void Display_GMT(uint8_t hours,uint8_t minutes)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Display_GMT(uint8_t hours,uint8_t minutes)
{ 
    static uint8_t m,q;
	m = hours /10 ;
	run_t.hours_two_unit_bit =	hours%10; 
	run_t.minutes_one_decade_bit= minutes/10 ;
	q=  minutes%10;
	TM1639_Write_4Bit_Time(m,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,q,0) ; //timer is default 12 hours "12:00"


}
/**********************************************************************
*
*Functin Name: void Display_Timing_Blink(uint8_t hours,uint8_t minutes)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Display_Timing_Blink(uint8_t hours,uint8_t minutes)
{ 
    static uint8_t m,q,counter;
	m = hours /10 ;
	run_t.hours_two_unit_bit =	hours%10; 
	run_t.minutes_one_decade_bit= minutes/10 ;
	q=  minutes%10;
	if(run_t.gTimer_set_timing_times < 100)
	    TM1639_Write_4Bit_Time(m,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,q,0) ; //timer is default 12 hours "12:00"
	else if(run_t.gTimer_set_timing_times > 99 && run_t.gTimer_set_timing_times < 115){
 		TM1639_Write_4Bit_Time(m,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,q,1) ; //timer is default 12 hours "12:00"

	}
	else{
		 run_t.gTimer_set_timing_times=0;
		 run_t.modify_input_timer_number++;
	    TM1639_Write_4Bit_Time(m,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,q,0);
			
     }

	if(run_t.wifi_set_timer_timing ==1){

		if(run_t.modify_input_timer_number > 2){ //run_t.temp_set_timer_timing_flag ==TIMER_TIMING
		   run_t.modify_input_timer_number=0;

		   if(run_t.timer_dispTime_hours==0){
			
		     run_t.timer_timing_define_ok =0;
			 run_t.wifi_set_timer_timing =0;
			 run_t.temp_set_timer_timing_flag =0;


		   }
		   else{
		   run_t.timer_timing_define_ok =1;
		   run_t.wifi_set_timer_timing =0;
		   run_t.temp_set_timer_timing_flag =0;

		   }

		}
	   
	}
    else if(run_t.modify_input_timer_number > 5  ){
	   run_t.modify_input_timer_number =0;
	   run_t.temp_set_timer_timing_flag =0;
	   run_t.mode_key_times =0;
	    TM1639_Write_4Bit_Time(0x00,0x00,0x00,0x00,0);


	}



}
/********************************************************************************
*
*Functin Name: void Display_Error_Digital(uint8_t errnumbers,uint8_t sel)
*Function : Timer of key be pressed handle
*Input Ref:  error digital 
*Return Ref: NO
*
********************************************************************************/
void Display_Error_Digital(uint8_t errnumbers,uint8_t sel)
{ 
    static uint8_t m,q;
	m = 0x0E;
	
	run_t.hours_two_unit_bit= 0x0d;
	
	run_t.minutes_one_decade_bit= errnumbers/10;
	q=errnumbers%10;
	TM1639_Write_4Bit_Time(m,run_t.hours_two_unit_bit,run_t.minutes_one_decade_bit,q,sel) ; //timer is default 12 hours "12:00"


}

/********************************************************************************
*
*Functin Name: static void TimeColon_Smg_Blink_Fun(void)
*Function : Timer of key be pressed handle
*Input Ref:  NO
*Return Ref: NO
*
********************************************************************************/
static void TimeColon_Smg_Blink_Fun(void)
{
    static uint8_t time_hours, minute_unit;
    if(run_t.timer_timing_define_ok == 1 || run_t.temp_set_timer_timing_flag ==TIMER_TIMING){

	 time_hours =  run_t.timer_dispTime_hours /10 ;
	 run_t.hours_two_unit_bit =run_t.timer_dispTime_hours % 10 ;
	
	  run_t.minutes_one_decade_bit =run_t.timer_dispTime_minutes /10;
	  minute_unit = run_t.timer_dispTime_minutes % 10;

	}
	else if(run_t.temp_set_timer_timing_flag !=TIMER_TIMING){

			

	 time_hours = run_t.works_dispTime_hours / 10;

	run_t.hours_two_unit_bit =run_t.works_dispTime_hours % 10;
	run_t.minutes_one_decade_bit =run_t.works_dispTime_minutes /10;
	 minute_unit = run_t.works_dispTime_minutes % 10;
     Display_GMT(run_t.works_dispTime_hours,run_t.works_dispTime_minutes);
	}

	if(run_t.gTimer_colon < 2){
		 
		  SmgBlink_Colon_Function(time_hours,run_t.hours_two_unit_bit ,run_t.minutes_one_decade_bit,minute_unit,0);
	   }
	   else if(run_t.gTimer_colon >  1	&&	run_t.gTimer_colon < 3){
		   SmgBlink_Colon_Function(time_hours,run_t.hours_two_unit_bit ,run_t.minutes_one_decade_bit,minute_unit,1);

	  }
	  else{
		 run_t.gTimer_colon =0;
		 SmgBlink_Colon_Function(time_hours,run_t.hours_two_unit_bit ,run_t.minutes_one_decade_bit,minute_unit,0);

	  }
}


void Display_TimeColon_Blink_Fun(void)
{

   if(run_t.gTimer_time_colon >1){ //10*20ms=300ms

	   run_t.gTimer_time_colon =0;
	   TimeColon_Smg_Blink_Fun();
	}
}


