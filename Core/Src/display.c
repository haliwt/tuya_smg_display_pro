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
void Display_DHT11_Temperature_Value(void)
{
    

  static uint8_t temp1,temp2;

	//hum1 =  run_t.gReal_humtemp[0]/10;  //Humidity 
	//hum2 =  run_t.gReal_humtemp[0]%10;

	temp1 = run_t.gReal_humtemp[1]/10 ;  // temperature
	temp2 = run_t.gReal_humtemp[1]%10;

   
	TM1639_Write_2bit_TempData(temp1,temp2);

    
	


}  

void Display_DHT11_Humidity_Value(void)
{
	  static uint8_t hum1,hum2; 
	  hum1 =  run_t.gReal_humtemp[0]/10;  //Humidity 
	  hum2 =  run_t.gReal_humtemp[0]%10;
	  
	  
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
    static uint8_t m,n,p,q;
	m = hours /10 %10 ;
	n=	hours%10; 
	
	p = minutes/10%10 ;
	q=  minutes%10;
	TM1639_Write_4Bit_Time(m,n,p,q,0) ; //timer is default 12 hours "12:00"


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
    static uint8_t m,n,p,q;
	m = hours /10 ;
	n =	hours%10; 
	p= minutes/10 ;
	q=  minutes%10;
	if(run_t.gTimer_set_timing_times < 30)
	    TM1639_Write_4Bit_Time(m,n,p,q,0) ; //timer is default 12 hours "12:00"
	else if(run_t.gTimer_set_timing_times > 29 && run_t.gTimer_set_timing_times < 59){
 		TM1639_Write_4Bit_Time(m,n,p,q,1) ; //timer is default 12 hours "12:00"

	}
	else{
		 run_t.gTimer_set_timing_times=0;
		 run_t.modify_input_timer_number++;
	   
			
     }

	if(run_t.wifi_set_timer_timing ==1){

		if(run_t.modify_input_timer_number > 3){ //run_t.set_timer_value_flag ==TIMER_TIMING
		   run_t.modify_input_timer_number=0;

		   if(run_t.timer_dispTime_hours==0){
			
		     run_t.timer_timing_define_ok =0;
			 run_t.wifi_set_timer_timing =0;
			 run_t.set_timer_value_flag =0;
			 Display_DHT11_Temperature_Value(); //WTI.EDIT 

		   }
		   else{
		   run_t.timer_timing_define_ok =1;
		   run_t.wifi_set_timer_timing =0;
		   run_t.set_timer_value_flag =0;
		   Display_DHT11_Temperature_Value(); //WT.EDIT

		   }

		}
	   
	}
    else if(run_t.modify_input_timer_number > 5  ){
	   run_t.modify_input_timer_number =0;
	   run_t.set_timer_value_flag =0;
	
	   run_t.mode_key_times =0;
	    TM1639_Write_4Bit_Time(0x00,0x00,0x00,0x00,0);
		Display_DHT11_Temperature_Value(); //WT.EDIT 


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
    static uint8_t m,n,p,q;
	m = 0x0E;
	
	n= 0x0d;
	
	p= errnumbers/10;
	q=errnumbers%10;
	TM1639_Write_4Bit_Time(m,n,p,q,sel) ; //timer is default 12 hours "12:00"


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
    static uint8_t hours_1,hours_2, minute_1,minute_2;

    
	
	
		if(run_t.timer_timing_define_ok == 1 || run_t.set_timer_value_flag ==TIMER_TIMING){

		hours_1 =  run_t.timer_dispTime_hours /10 ;
		hours_2 =run_t.timer_dispTime_hours % 10 ;
		
		minute_1 =run_t.timer_dispTime_minutes /10;
		minute_2 = run_t.timer_dispTime_minutes % 10;
		

		}
		else if(run_t.set_timer_value_flag !=TIMER_TIMING){

				
		//hours 
		hours_1 =run_t.works_dispTime_hours / 10;

		hours_2 =run_t.works_dispTime_hours % 10;
		//minutes
		minute_1 =run_t.works_dispTime_minutes /10;
		minute_2 = run_t.works_dispTime_minutes % 10;
		
		
		}

	 
		if(run_t.gTimer_colon < 2){
			 SmgBlink_Colon_Function(hours_1,hours_2 ,minute_1,minute_2,0);

			
		}
		else if(run_t.gTimer_colon > 1	&&	run_t.gTimer_colon < 4){
			 

		     SmgBlink_Colon_Function(hours_1,hours_2 ,minute_1,minute_2,1);

				
		}
		else{
			 run_t.gTimer_colon =0;
			

		}
    
	
}


void Display_TimeColon_Blink_Fun(void)
{
 TimeColon_Smg_Blink_Fun();
	
}


