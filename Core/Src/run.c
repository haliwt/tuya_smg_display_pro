#include "run.h"
#include "smg.h"
#include "gpio.h"
#include "cmd_link.h"
#include "led.h"
#include "display.h"
#include "bsp_display_dynamic.h"
#include "key.h"
#include "bsp.h"

RUN_T run_t;

uint8_t power_on_off_flag;

void Power_Off(void);



/******************************************************************************
	*
	*Function Name:void RunPocess_Command_Handler(void)
	*Funcion: display pannel run of process 
	*Input Ref: NO
	*Return Ref:NO
	*
******************************************************************************/
void RunPocess_Command_Handler(void)
{
   static uint8_t power_off_set_flag;
   static uint8_t step_state;

   switch(run_t.gRunCommand_label){

      case RUN_POWER_ON:
	  		
	  	    run_t.step_run_power_off_tag=0;
        
	       run_t.set_temperature_decade_value=40;
		   run_t.gPower_On=RUN_POWER_ON;
           
            run_t.step_run_power_off_tag=0;
            run_t.power_on_send_to_mb_times=36;
			run_t.timer_timing_define_ok=0;
			Power_On_Fun();
			run_t.gRunCommand_label= UPDATE_DATA;
		break;

	  case RUN_POWER_OFF://2
	    
	  	 run_t.step_run_power_on_tag=0;
		 run_t.temp_set_timer_timing_flag=0;
		 
         run_t.power_off_send_to_mb_times= 20;
		 run_t.gPower_On=RUN_POWER_OFF;
			 
		run_t.gFan_RunContinue =1;
		run_t.gTimer_fan_continue=0;
		Power_Off_Fun();
		run_t.gRunCommand_label =POWER_OFF_PROCESS;
      break;
	 

	  case UPDATE_DATA: //3
            if(run_t.gPower_On==RUN_POWER_ON){
	        switch(step_state){

					case 0:
					 switch(run_t.gFan){
                       case 0:
						Led_Panel_OnOff();
						break;
						case 1:
							display_fan_speed_value(run_t.gFan_level);
						break;

					 }
					    step_state=1;
					break;

					case 1:
					 switch(run_t.gFan){
                       case 0:
               			RunLocal_Dht11_Data_Process();
					  break;
					    case 1:
							display_fan_speed_value(run_t.gFan_level);
						break;

					 }
				        step_state=2;
	                    
				   break;

					case 2:
					 switch(run_t.gFan){
                       case 0:
                      Set_Timing_Temperature_Number_Value();
					  break;
					   case 1:
							display_fan_speed_value(run_t.gFan_level);
						break;

					 }

                      step_state=3;
					break;

					case 3:
                        switch(run_t.gFan){

                        case 0:
					       Display_SmgTiming_Value();
                        break;

                        case 1:
							display_fan_speed_value(run_t.gFan_level);
						break;


                        }
					step_state=4;

					break;

					case 4:
       					switch(run_t.gFan){

                        case 0:
	                   		Compare_SetTemperature_Value(); 
					    break;
					     case 1:
							display_fan_speed_value(run_t.gFan_level);
						break;

					 }
					step_state=5;

					break;

					case 5:
						switch(run_t.gFan){

							case 0:
								if(run_t.ptc_warning ==0 && run_t.fan_warning ==0){
								if(run_t.temp_set_timer_timing_flag != TIMER_TIMING){
									Display_TimeColon_Blink_Fun();
								}
								}
							break;

							case 1:

							display_fan_speed_value(run_t.gFan_level);
							

							break;
						}
                    step_state=0;
                    break;
				   }
               
           }
     break;

	  case POWER_OFF_PROCESS://4

	   if(run_t.gPower_On ==RUN_POWER_OFF && POWER_KEY_VALUE()  ==KEY_UP ){

	          if(power_off_set_flag==0){
					power_off_set_flag++;
                   Power_Off_Fun();
				
	   	      }
	   	       PowerOff_Bug_Led();
			  

			  if(run_t.first_power_on_times==1)run_t.gTimer_fan_continue =0;
            
			 	if(run_t.gTimer_fan_continue < 61 && run_t.gTimer_fan_continue ==1 && run_t.fan_warning==0){
                   
					LED_FAN_ON() ;
				 }
				 else if(run_t.gTimer_fan_continue > 59){
                    run_t.gTimer_fan_continue =67;
				   LED_FAN_OFF() ;
				   run_t.gFan_RunContinue =0;

				 }
                 

			 
                
				Breath_Led();
		 
		
       }

	  break;

  }
}




/**********************************************************************
*
*Functin Name: void Display_DHT11_Value(void)
*Function : Timer of key be pressed handle
*Input Ref:  key of value
*Return Ref: NO
*
**********************************************************************/
void Power_Off(void)
{
    	 if(run_t.gPower_On ==RUN_POWER_OFF){

			Smg_AllOff();
            SMG_POWER_OFF()	;
			run_t.gPlasma=0;
			run_t.gDry=0;
			run_t.gFan =0;
			
		}
	}


/**********************************************************************
*
*Functin Name: void Receive_ManiBoard_Cmd(uint8_t cmd)
*Function :  wifi recieve data
*Input Ref:  receive wifi send order
*Return Ref: NO
*
**********************************************************************/
void Power_On_Fun(void)
{
                
		run_t.gPlasma=1;
		run_t.gDry =1;
		run_t.gBug =1;
		run_t.ai_model_flag =AI_MODE;

  
    

    run_t.time_led_flag=1;
	Power_ON_Led();

	run_t.fan_warning=0;
	run_t.ptc_warning=0;

	run_t.works_dispTime_hours=0;
	run_t.works_dispTime_minutes=0;
	run_t.gTimes_time_seconds =0;
	
    run_t.gTimer_Counter=0;
	run_t.timer_dispTime_hours =0;
	run_t.timer_dispTime_minutes=0;
		
        

	 
	  
	   SMG_POWER_ON(); //WT.EDIT 2023.03.02
      


	 
      
	 TM1639_Write_4Bit_Time(0,0,0,0,0);
     Display_DHT11_Value();
    
}


/************************************************************************
	*
	*Function Name: static void Power_Off_Fun(void)
	*
	*
	*
	*
************************************************************************/
void Power_Off_Fun(void)
{
	
 
		run_t.gPlasma=0;
		run_t.gDry =0;
		run_t.gBug =0;
		
        run_t.gPower_On =RUN_POWER_OFF;
		run_t.gTimer_set_temp_times=0; //conflict with send temperatur value 
       Power_Off_Led_Off();

  
} 

