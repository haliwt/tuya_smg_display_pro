#ifndef __RUN_H_
#define __RUN_H_
#include "main.h"

typedef enum _Signal{
    POWER_OFF= 0xFD,
	POWER_ON=0x01,
    PANEL_DATA=0x02,
	WIFI_INFO=0x03,
	WIFI_TIME=0x04,
	WIFI_TEMP,
	WIFI_CMD,
	WIFI_BEIJING_TIME,
	WIFI_SET_TIMING,
	WIFI_REF_DATA,
	WIFI_FAN_DATA
}signal_t;

typedef enum WIFI_STATE_T{
   
    WIFI_POWER_ON = 0x80,
    WIFI_POWER_OFF=0X81,
    WIFI_MODE_1=0X08,   //state ->normal works
    WIFI_MODE_2=0X18,   //state->sleeping works
    WIFI_KILL_ON=0x04,  //Anion(plasma)
    WIFI_KILL_OFF=0x14,
    WIFI_PTC_ON = 0x02, 
    WIFI_PTC_OFF = 0x12,
    WIFI_SONIC_ON = 0x01,       //ultrasonic
    WIFI_SONIC_OFF = 0x11,
    WIFI_TEMPERATURE = 0xA0,
	
    WIFI_POWER_ON_NORMAL= 0xB0,
	WIFI_POWER_OFF_NORMAL = 0xB1,

	PTC_WARNING= 0xE1,
	FAN_WARNING = 0xE2,

	FAN_REMOVE_WARNING = 0xF2
    


}wifi_state_t;


typedef enum TIMING_T{

   timing_donot,
   timing_success ,
   timing_power_off,
   timing_null
}timing_t;




typedef enum _WIFI_POWER_STATE_{
	WIFI_POWER_ON_ITEM = 0x01,
	WIFI_POWER_OFF_ITEM,
	WIFI_POWER_NULL

}wifi_power_state;



typedef struct __RUN{

   uint8_t keyvalue;
   uint8_t temp_key_pressed_flag;
 


   uint8_t power_key;
   uint8_t gTemperature_timer_flag;
   uint8_t gPower_On;
   uint8_t gRun_flag;
   uint8_t power_times;
 
   
   uint8_t keyAdd_continue;
   uint8_t keyDec_continue;
   uint8_t gRunCommand_label;
  
   uint8_t gTimer_power_key_pressed;


   uint8_t gTemperature;
 
   uint8_t gTimer_Counter;

   //smg
   uint8_t hours_two_bit ;
   uint8_t minutes_one_bit ;
   uint8_t modify_input_timer_number;
   
  
  
  uint8_t gTimer_setup_zero;

  uint8_t gFan_RunContinue;

  uint8_t temperature_flag;
  

   uint8_t decodeFlag;
   uint8_t time_led_flag;

   uint8_t dec_key_times;
   uint8_t add_key_times;
   
   uint8_t  gTimer_Cmd;
   uint8_t  gPlasma;
 
   uint8_t  gDry;
   uint8_t  gBug;
   uint8_t  gUltrasonic;


   uint8_t  ptc_warning;
   uint8_t  fan_warning;


   uint8_t  gFan;
   uint8_t  power_on_send_to_mb_times;
   uint8_t step_run_power_on_tag;
   uint8_t step_run_power_off_tag;
   uint8_t power_off_send_to_mb_times;
  
   //key ref
  
 
   uint8_t gTimer_mode_key_counter;
   //fan ref
   uint8_t gFan_level;
   uint8_t gTimer_display_fan_level;
 
 
   uint8_t fan_stop_flag;


   //dry ref 
   uint8_t  dry_key;
   uint8_t  manual_dry_turn_off;



   uint8_t gTimes_minutes_temp;
   uint8_t set_up_temp_flag;
   uint8_t gSet_up_times;
   uint8_t auto_model_shut_off_ptc_flag;

  
   

   uint8_t gTimer_display_dht11;
  

   uint8_t first_power_on_times;
   
 

   uint8_t Timer_model_flag;
   uint8_t temperature_set_flag;

   uint8_t gTimes_time_seconds;
   volatile uint8_t set_temperature_flag;
  
	
   uint8_t gReal_humtemp[2];
   uint8_t rx_mb_data_tag;
   uint8_t rx_mb_answer_tag ;

   //timer ref
   uint8_t timer_timing_define_ok ;
   uint8_t timer_timing_define_flag;
   uint16_t gTimer_set_timing_times;
   uint8_t mode_key_times;

  //usart ref
    uint8_t process_run_guarantee_flag;

	
	//WIFI 

 
	 uint8_t wifi_led_fast_blink_flag;
   uint8_t gTimer_wifi_slowly; 
   uint8_t wifi_link_cloud_flag;
	 uint8_t gTimer_wifi_connect_counter;
    uint8_t wifi_orderByMainboard_label;

	uint8_t wifiCmd[1];
	uint8_t wifi_set_timer_timing ;

  /*********wifi end ***********/
  //timing
  uint8_t dispTime_hours;
  uint8_t dispTime_minutes;
  uint8_t dispTime_seconds;
  uint8_t gTimer_disp_works_times;
  uint8_t set_special_temperature_value ;
   
  
   /***/
   uint8_t 	gTimer_numbers_one_two_blink;
   uint8_t gTimer_temp_delay;
   volatile uint8_t set_temperature_decade_value;
   volatile   uint8_t set_temperature_unit_value;
   
 
   uint8_t ai_model_flag;

   volatile uint8_t temp_set_timer_timing_flag;
  
   uint8_t power_on_recoder_times;

   //timer timing 
   uint8_t gTimer_set_temp_times;
  
   
   uint8_t gTimer_temperature;
   volatile uint8_t gTimer_key_temp_timing;

   uint8_t send_app_wokes_minutes_two;
   uint8_t send_app_wokes_minutes_one;

   /***answering response****/
   uint8_t response_power_on;
   uint8_t response_power_off;


   //

   uint8_t send_app_timer_minutes_one;
   uint8_t send_app_timer_minutes_two;
   uint8_t define_initialization_timer_time_hours;
   uint8_t receive_app_timer_time_flag ;
   uint8_t works_dispTime_minutes;
   uint8_t works_dispTime_hours;

   volatile  uint8_t gTimer_colon;
 
    
  
   uint8_t set_timer_special_value;

  

   uint8_t gTimer_fan_continue;
 
 
   
  uint8_t gTimer_dogfood_times;

  int8_t timer_dispTime_hours;
  int8_t timer_dispTime_minutes;
	

	uint16_t gTimer_usart_error;
	uint16_t gTimer_error_digital ;
   
  


   
 }RUN_T;

extern RUN_T run_t;


void Power_Off(void);
void SplitDispose_Key(uint8_t value);







void Power_Off_Fun(void);




void Power_On_Fun(void);

void RunPocess_Command_Handler(void);

#endif 



