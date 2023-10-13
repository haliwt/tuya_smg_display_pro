#include "bsp_display_dynamic.h"
#include "run.h"
#include "smg.h"
#include "gpio.h"
#include "cmd_link.h"
#include "led.h"
#include "key.h"
#include "display.h"


uint8_t  step_state;

void (*single_ai_fun)(uint8_t cmd);
void (*single_add_fun)(void);
void (*single_buzzer_fun)(void);
void (*sendAi_usart_fun)(uint8_t senddat);
void (*dispose_key)(uint8_t dsdat);
void (*display_fan_speed_value)(uint8_t fan_level);

//static void Led_Panel_OnOff(void);

//static void RunLocal_Dht11_Data_Process(void);

static void DisplayPanel_DHT11_Value(void);
//static void Display_SetTemperature_Value(void);
static void Display_Works_Time_Fun(void);
static void WorksTime_DonotDisplay_Fun(void);
static void Timer_Timing_Donot_Display(void);

static void Smg_DisplayFan_Level_Value_Fun(uint8_t fan_level);



void Smg_DisplayFan_Speed_Level_Init(void)
{
      Smg_DisplayFan_Leve(Smg_DisplayFan_Level_Value_Fun);

}
/******************************************************************************
*
*Function Name:static void Setup_Timer_Times(void)
*Funcion:display setup timer times  //__asm("NOP");//等待1个指令周期，系统主频24M
*Iinput Ref:NO
*Return Ref:NO
*
******************************************************************************/
void Display_SmgTiming_Value(void)
{

   static uint8_t timer_display_flag, alternate_flag;
    static uint8_t input_tiimes;
    switch(run_t.timer_timing_define_ok){

	case timing_success: //0x01

	  

	 if(run_t.gTimer_Counter > 59){
	    run_t.gTimer_Counter =0;
		
		run_t.timer_dispTime_minutes -- ;
	
	    if(run_t.timer_dispTime_minutes <  0 ){
			 
		   run_t.timer_dispTime_hours -- ;
		   run_t.timer_dispTime_minutes =59;
         }

		
		//power off
		 if(run_t.timer_dispTime_hours < 0 ){
		 
			run_t.gTimer_Counter = 57 ;
			run_t.timer_dispTime_hours=0;
			run_t.timer_dispTime_minutes=0;
			run_t.timer_timing_define_ok =timing_power_off;
			
	      }
		 timer_display_flag=1;
	 	}  
       
	   if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
		   if(timer_display_flag==1 || input_tiimes < 5 ){
			   timer_display_flag=0;
			   input_tiimes++;
	   	
		    Display_GMT(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
			HAL_Delay(5);
	     	}

	  }
	  else if(run_t.ptc_warning ==1 || run_t.fan_warning ==1){

	     if(run_t.gTimer_error_digital < 60){//10ms * 60= 600

		        if(alternate_flag ==0){
			   	
                     
			     if(run_t.ptc_warning ==1){
                 
					Display_Error_Digital(0x01,0);
			     }
				 else {
			        if(run_t.fan_warning ==1){

					  
                      Display_Error_Digital(0x02,0);

			        }

				 }
			    

               }
			   else{

			      alternate_flag=2;
				   if(run_t.ptc_warning ==1 && run_t.fan_warning ==1){

					     Display_Error_Digital(0x02,0);

				   	}
				    else  if(run_t.ptc_warning ==1 && run_t.fan_warning ==0){
                       
					    Display_Error_Digital(0x01,0);
			        }
					else  if(run_t.ptc_warning ==0 && run_t.fan_warning ==1){
                       
					    Display_Error_Digital(0x02,0);
					    if(alternate_flag==2 ||alternate_flag>2 )alternate_flag=0;
			        }


			   }
			   

		   }
		   else if(run_t.gTimer_error_digital > 59 && run_t.gTimer_error_digital < 121 ){

                  alternate_flag++;
 				Display_Error_Digital(0x10,1);
 				if( alternate_flag==2) alternate_flag=0;
		   }
		   else if(run_t.gTimer_error_digital > 119){

			  run_t.gTimer_error_digital=0;


			 }


	  }

	   WorksTime_DonotDisplay_Fun();
	   
	break;


	case timing_power_off:
        SendData_PowerOnOff(0);
		HAL_Delay(5);
		
	  run_t.power_on_recoder_times++; //this is data must be change if not don't "breath led"
	  run_t.gRunCommand_label = RUN_POWER_OFF;//POWER_OFF_PROCESS; //POWER_OFF_PROCESS ;


	break;


	case timing_donot:

	   if(run_t.temp_set_timer_timing_flag == 0){
		input_tiimes=0;
        Timer_Timing_Donot_Display();
		
	    Display_Works_Time_Fun();
	  }
    break;

	}
    
 }

/******************************************************************************
*
*Function Name:void Single_RunCmd(void)
*Funcion: handle of receive by usart data
*
*
******************************************************************************/
static void DisplayPanel_DHT11_Value(void)
{
  if(run_t.first_power_on_times==1){
      run_t.first_power_on_times++;
       Display_DHT11_Value();
  
  }
  if(run_t.gTimer_display_dht11 > 32 && run_t.set_temperature_flag==0){
	    run_t.gTimer_display_dht11=0;
       	Display_DHT11_Value();
     
	}
}



/****************************************************************
	*
	*Function Name :void Set_Timing_Temperature_Number_Value(void)
	*Function : set timer timing how many ?
	*Input Parameters :NO
	*Retrurn Parameter :NO
	*
*****************************************************************/
void Set_Timing_Temperature_Number_Value(void)
{

    
    static uint8_t set_temp_flag, counter_times;
   
	
	
	if(run_t.temp_set_timer_timing_flag == TIMER_TIMING && run_t.timer_timing_define_ok ==0){

	         
        
		Display_Timing_Blink(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);
		

 
    }
    else if(run_t.temp_set_timer_timing_flag==0){

	 if(run_t.set_temperature_flag ==SET_TEMP_VALUE_ITEM){

	  //waiting for 4 s 
	  if(run_t.gTimer_key_temp_timing > 3 && run_t.set_temperature_special_value ==0){
			set_temp_flag++;
			
			run_t.set_temperature_special_value =1;
			run_t.gTimer_set_temp_times =0; //couter time of smg blink timing 

	 }
	 //temperature of smg of LED blink .
	  if(run_t.set_temperature_special_value ==1){
	  	
	  	
		  if(run_t.gTimer_set_temp_times < 15 ){ // 4
		        TM1639_Write_2bit_SetUp_TempData(0,0,1);
          }
		  else if(run_t.gTimer_set_temp_times > 14 && run_t.gTimer_set_temp_times < 29){
		  	
			  TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);

		  }
		  else{
		  	 run_t.gTimer_set_temp_times=0;
             counter_times++ ;  

		  }


           if(counter_times > 3){
			 
			 set_temp_flag=0;
		     counter_times=0;
		      run_t.set_temperature_flag = TIMER_TIMING_DONOT_VALUE;
			  run_t.temperature_set_flag =1;
			  run_t.set_temperature_special_value =0xff;
			  run_t.set_temperature_flag= 0; //WT.EDTI 2023.09.27
			  run_t.gTimer_temp_delay = 70; //at once shut down ptc  funciton
		
			  TM1639_Write_2bit_SetUp_TempData(run_t.set_temperature_decade_value,run_t.set_temperature_unit_value,0);
	       
	       }
	     }
	 }
	}

}

/*******************************************************
	*
	*Function Name: static void RunLocal_Dht11_Data_Process(void)
	*Function : display pannel display conetent
	*
	*
	*
*******************************************************/
void RunLocal_Dht11_Data_Process(void)
{
   DisplayPanel_DHT11_Value();

}
void Led_Panel_OnOff(void)
{
	Panel_Led_OnOff_Function() ;
}
/*******************************************************
	*
	*Function Name: static void RunLocal_Dht11_Data_Process(void)
	*Function : display pannel display conetent
	*
	*
	*
*******************************************************/
void Compare_SetTemperature_Value(void)
{
	
    static uint8_t SET_TEMP_VALUE_ITEM;
    if(run_t.temperature_set_flag ==1 && run_t.gTimer_temp_delay >60){
               run_t.gTimer_temp_delay =0;
		 
		  SET_TEMP_VALUE_ITEM= run_t.set_temperature_decade_value*10+ run_t.set_temperature_unit_value;
		  if(SET_TEMP_VALUE_ITEM <= run_t.gReal_humtemp[1] || run_t.gReal_humtemp[1] >40){//envirment temperature
	  
				run_t.gDry = 0;
                
		        SendData_Set_Command(DRY_OFF_NO_BUZZER);//PTC turn off
			    
			    
                
		  }
		  else if((SET_TEMP_VALUE_ITEM -3) >= run_t.gReal_humtemp[1]){

                run_t.gDry = 1;
                SendData_Set_Command(DRY_ON_NO_BUZZER); //PTC turn On



		  }
	  
	    
	}
    else if(run_t.temperature_set_flag ==0){ //no define set up temperature value 
		if(run_t.gReal_humtemp[1] >40 && run_t.gTimer_temp_delay >119){//envirment temperature
			run_t.gTimer_temp_delay =0;
			run_t.gDry = 0;
			run_t.auto_model_shut_off_ptc_flag =1;
			SendData_Set_Command(DRY_OFF_NO_BUZZER);

		}
        else if(run_t.gReal_humtemp[1] < 38 && run_t.auto_model_shut_off_ptc_flag ==1 &&  run_t.gTimer_temp_delay >119){
			run_t.gTimer_temp_delay =0;
			if(run_t.ai_model_flag == AI_MODE){
			run_t.gDry = 1;
			SendData_Set_Command(DRY_ON_NO_BUZZER); //PTC turn On

           }
             
          }
			    
      }

}

/******************************************************************************
* 
* Function Name: static void Timer_Timing_Donot_Display(void)
* Function :function of pointer 
* Input Ref:NO
* Return Ref:NO
* 
*******************************************************************************/
static void Display_Works_Time_Fun(void)
{
     static uint8_t works_timing_flag,alternate_flag;

	 if(run_t.ptc_warning ==0 && run_t.fan_warning ==0 && run_t.timer_timing_define_ok==0){
     if(run_t.gTimes_time_seconds > 59 ){
            run_t.gTimes_time_seconds=0;
            works_timing_flag =1;
			run_t.works_dispTime_minutes++; //1 minute 
		
            run_t.send_app_wokes_minutes_two++;
			if(run_t.works_dispTime_minutes> 59){ //1 hour
			run_t.works_dispTime_minutes=0;
			run_t.works_dispTime_hours++;
			if(run_t.works_dispTime_hours > 99){ //WT.edit times over 99hours 2023.09.20
			run_t.works_dispTime_hours =0;
			}
        
            }

     	}
       
	       if(works_timing_flag==1  ){
	          works_timing_flag=0;
			
			 Display_GMT(run_t.works_dispTime_hours,run_t.works_dispTime_minutes);
		  
	        }
		    
        }
		else if(run_t.ptc_warning ==1 || run_t.fan_warning ==1){

		    if(run_t.gTimer_error_digital < 60){//10ms * 51= 510

		      
               if(alternate_flag ==0){
			   	  
                     
			     if(run_t.ptc_warning ==1){
                 
					Display_Error_Digital(0x01,0);
			     }
				 else {
			        if(run_t.fan_warning ==1){

					  
                      Display_Error_Digital(0x02,0);

			        }

				 }
			    

               }
			   else{

			      alternate_flag=2;
				   if(run_t.ptc_warning ==1 && run_t.fan_warning ==1){

					     Display_Error_Digital(0x02,0);

				   	}
				    else  if(run_t.ptc_warning ==1 && run_t.fan_warning ==0){
                       
					    Display_Error_Digital(0x01,0);
			        }
					else  if(run_t.ptc_warning ==0 && run_t.fan_warning ==1){
                       
					    Display_Error_Digital(0x02,0);
			        }


			   }
			   

		   }
		   else if(run_t.gTimer_error_digital > 59 && run_t.gTimer_error_digital  < 121 ){
		   		alternate_flag++;
 				Display_Error_Digital(0x10,1);
 				if(alternate_flag==2 ||alternate_flag>2 )alternate_flag=0;
		   }
		    else if(run_t.gTimer_error_digital > 119){

			  run_t.gTimer_error_digital=0;


			 }

		}



}

/****************************************************************
 * 
 * Function Name: static void Timer_Timing_Donot_Display(void)
 * Function :function of pointer 
 * Input Ref:NO
 *
 * 
*****************************************************************/
static void Timer_Timing_Donot_Display(void)
{
     
 
     if(run_t.gTimer_Counter > 59 && run_t.timer_timing_define_ok==1){
	    run_t.gTimer_Counter =0;
		run_t.timer_dispTime_minutes -- ;
	
	    if(run_t.timer_dispTime_minutes <  0 ){
			 
		   run_t.timer_dispTime_hours -- ;
		   run_t.timer_dispTime_minutes =59;
         }

		
		
		if(run_t.timer_dispTime_hours <0){ 
				run_t.gTimer_Counter = 57 ;
				run_t.timer_dispTime_hours=0;
				run_t.timer_dispTime_minutes=0;
			
			}
			
        }



}

/****************************************************************
 * 
 * Function Name: static void WorksTime_DonotDisplay_Fun(void)
 * Function :function of pointer 
 * 
 *
 * 
****************************************************************/
static void WorksTime_DonotDisplay_Fun(void)
{
//send to APP works times every minute onece
   if(run_t.gTimes_time_seconds > 59 && run_t.timer_timing_define_ok==1){
		   run_t.gTimes_time_seconds=0;
		 
		   run_t.works_dispTime_minutes++; //1 minute 
		
		   run_t.send_app_wokes_minutes_two++;
		   if(run_t.works_dispTime_minutes> 59){ //1 hour
		   run_t.works_dispTime_minutes=0;
		   run_t.works_dispTime_hours++;
		   if(run_t.works_dispTime_hours > 99){
		   run_t.works_dispTime_hours =0;
		   }
	       }
  }
//	while(run_t.send_works_times_to_app==1){
//		   run_t.send_works_times_to_app=0;
//	   SendData_Works_Time(run_t.send_app_wokes_minutes_one ,run_t.send_app_wokes_minutes_two);
//	   }
}

static void Smg_DisplayFan_Level_Value_Fun(uint8_t fan_level)
{

    static uint8_t fan_max=0xff,fan_min=0xff;

    if(fan_level ==fan_speed_max){
    
    if(fan_max != run_t.fan_key_max){
       fan_max = run_t.fan_key_max;
      SendData_Set_Command(FAN_LEVEL_MAX);

     }
   }
   else{

       if(fan_min != run_t.fan_key_min){
          fan_min = run_t.fan_key_min;
          SendData_Set_Command(FAN_LEVEL_MIN);
   
        }
   }
    
    TM1639_Write_4Bit_Fan_Level(fan_level);
    if(run_t.gTimer_display_fan_level > 2){
        run_t.gTimer_display_fan_level=0;
        run_t.gFan =0;
        Display_GMT(run_t.timer_dispTime_hours,run_t.timer_dispTime_minutes);

    }
 

}
/****************************************************************
 * 
 * Function Name:
 * Function :function of pointer 
 * 
 * 
****************************************************************/
void Single_Add_RunCmd(void(*addHandler)(void))
{
    single_add_fun = addHandler;   

}

void Single_SendBuzzer_RunCmd(void(*buzzerHandler)(void))
{
	single_buzzer_fun = buzzerHandler;

}
void Single_SendAi_Usart_RunCmd(void(*sendaiHandler)(uint8_t seddat))
{
    sendAi_usart_fun = sendaiHandler;

}

void SplitDispose_Key_RunCmd(void(*keyHandler)(uint8_t dat))
{

  dispose_key = keyHandler;

}

void Smg_DisplayFan_Leve(void(*fandisplayHandler)(uint8_t fan_level))
{
   display_fan_speed_value = fandisplayHandler;

}




