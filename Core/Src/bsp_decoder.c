#include "bsp_decoder.h"
#include "bsp.h"




static void Receive_Wifi_Cmd(uint8_t cmd);



/**********************************************************************************************************
**
*Function Name:static void notifyStatusToHost(uint8_t lightNum,uint8_t filterNum,uint8_t unionNum)
*Function : 
*Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
*Return Ref:NO
*
*********************************************************************************************************/
void Decode_Handler(void)
{
   if(run_t.decodeFlag ==1){
   run_t.decodeFlag =0;
  
   Receive_MainBoard_Data_Handler(run_t.wifi_orderByMainboard_label);

   }
  
}


/**********************************************************************
*
*Functin Name: void Receive_MainBoard_Data_Handler(run_t.single_data)
*Function : run order from main command 
*Input Ref:  run_t.single_dat is command 
*Return Ref: NO
*
**********************************************************************/
void Receive_MainBoard_Data_Handler(uint8_t cmd)
{
	static uint8_t m,n,p,q;
	static uint8_t hum1,hum2,temp1,temp2; 
	
    switch(cmd){


	 case WIFI_CMD: //0x06
	 	 
	    Receive_Wifi_Cmd(run_t.wifiCmd[0]);
	 
	 break;

	 case WIFI_TEMP: //5->set temperature value
	       if(run_t.gPower_On ==1){
		   	   run_t.set_temperature_flag=1;
			  
			   
           	run_t.gTimer_key_temp_timing=0;
        	 m= run_t.wifi_set_temperature /10 ;
			 n= run_t.wifi_set_temperature %10;
		   
	        TM1639_Write_2bit_SetUp_TempData(m,n,0);
			}
    
	 break;

	 case PANEL_DATA://0X02
	   
        if(run_t.gPower_On ==1){
        hum1 =  run_t.gReal_humtemp[0]/10 %10;  //Humidity 
        hum2 =  run_t.gReal_humtemp[0]%10;
        
        temp1 = run_t.gReal_humtemp[1]/10 %10;  // temperature
        temp2 = run_t.gReal_humtemp[1]%10;

         //temperature 
          TM1639_Write_2bit_TempData(temp1,temp2);
	      TM1639_Write_2bit_HumData(hum1,hum2);
	     HAL_Delay(2);
        }
	  

      break;

       case WIFI_BEIJING_TIME: 
         if(run_t.wifi_link_cloud_flag ==1 && run_t.gPower_On==1){
          
               
			 m=(run_t.dispTime_hours ) /10;
	        n = (run_t.dispTime_hours ) %10;;
			
            p= (run_t.dispTime_minutes )/10;
            q = (run_t.dispTime_minutes )%10;
			run_t.hours_two_bit = n;
            run_t.minutes_one_bit = p;
	       TM1639_Write_4Bit_Time(m,run_t.hours_two_bit,run_t.minutes_one_bit,q,0) ; // timer   mode  "H0: xx"
			
		 }
         
		    
       
 
      break;

      case WIFI_SET_TIMING:
        
       
           
            run_t.timer_dispTime_hours= run_t.dispTime_hours;
            run_t.timer_dispTime_minutes = 0;
	  
	        run_t.temp_set_timer_timing_flag = TIMER_TIMING ;
	        run_t.timer_timing_define_ok =0;
		
			run_t.gTimer_set_timing_times=0;
			run_t.wifi_set_timer_timing =1;

		
             m=(  run_t.timer_dispTime_hours ) /10;
	       

			 n= (  run_t.timer_dispTime_hours ) %10;;
			
	         run_t.hours_two_bit = n;
			 run_t.minutes_one_bit = 0;
			 
             TM1639_Write_4Bit_Time(m,run_t.hours_two_bit,run_t.minutes_one_bit,0,0) ; // timer   mode  "H0: xx"
            
	   		 

      break;

	  case WIFI_FAN_DATA:


	      if(run_t.wifiCmd[0] > 50)
               run_t.gFan_level=fan_speed_max;
		  else
		  	 run_t.gFan_level=fan_speed_min;

	  break;

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
static void Receive_Wifi_Cmd(uint8_t cmd)
{
	switch(cmd){



          case WIFI_POWER_ON_NORMAL: //0xB0

                run_t.wifi_link_cloud_flag =WIFI_CLOUD_SUCCESS;
				
		        run_t.gRunCommand_label = RUN_POWER_ON;
	            run_t.gPower_On=RUN_POWER_ON;
           
				
                run_t.response_power_on = 1;
                run_t.response_power_off =0;
			break;


             case WIFI_POWER_OFF_NORMAL: //0xB1 //WT.EDIT 2023.08.21

    
			   run_t.gRunCommand_label = RUN_POWER_OFF; //RUN_POWER_OFF; //WT.EDIT 2023.08-16
			   run_t.gPower_On=RUN_POWER_OFF;
			
			   run_t.power_on_recoder_times++;
             
          
			   run_t.wifi_link_cloud_flag =WIFI_CLOUD_SUCCESS;
			   run_t.response_power_off =1;
               run_t.response_power_on =0;
			break;

			
			case WIFI_POWER_ON:

			
		
		        run_t.gRunCommand_label = RUN_POWER_ON;
             
				run_t.wifi_link_cloud_flag =WIFI_CLOUD_SUCCESS;
                run_t.response_power_on = 1;
                run_t.response_power_off =0;

			break;
			 
			 

			 case WIFI_POWER_OFF: //turn off 
                
		
			   run_t.gRunCommand_label = POWER_OFF_PROCESS; //RUN_POWER_OFF; //WT.EDIT 2023.08-16
			 
			   run_t.power_on_recoder_times++;
             
         
			   run_t.wifi_link_cloud_flag =WIFI_CLOUD_SUCCESS;
			   run_t.response_power_off =1;
               run_t.response_power_on =0;
            

			 break;

			 case WIFI_KILL_ON: //kill turn on plasma
			  if(run_t.gPower_On==1){
               	run_t.gPlasma = 1;
			        
             } 
			 break;

			 case WIFI_KILL_OFF: //kill turn off
                if(run_t.gPower_On==1){
			 	  run_t.gPlasma =0;
				  
		          
                }
			 break;

			 case WIFI_PTC_ON://dry turn on
                if(run_t.gPower_On==1){
			        run_t.gDry =1;
                    
                 
                }
			 break;

			 case WIFI_PTC_OFF: //dry turn off
               
			 	if(run_t.gPower_On==1){
					run_t.gDry=0;
                 
		           
			 	}

			 break;

			 case WIFI_SONIC_ON:  //drive bug
		
				 if(run_t.gPower_On==1){		   
				  run_t.gUltrasonic =1; //turn on 
			
				 
			    }

			 break;

			 case WIFI_SONIC_OFF: //drive bug turn off
			 	if(run_t.gPower_On==1){
				    run_t.gUltrasonic=0;
					
			   }
			 break;



	         default :
                  cmd =0;
			 break;

			 
        }
   
}


