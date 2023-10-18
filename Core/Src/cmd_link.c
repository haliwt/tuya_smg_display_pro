#include "cmd_link.h"
#include "bsp.h"

volatile static uint8_t transOngoingFlag; //interrupt Transmit flag bit , 1---stop,0--run
uint8_t outputBuf[8];
static uint8_t transferSize;
static uint8_t state;
uint8_t inputBuf[MAX_BUFFER_SIZE];
uint8_t parse_buf[MAX_BUFFER_BUF];


/****************************************************************************************************
**
*Function Name:static void selectLight(uint8_t index)
*Function: UART2 transmit interrupt process ---4D 58 4C 53 32 30 32 
*Input Ref: LED number 
*Return Ref:NO
*
****************************************************************************************************/
void SendData_PowerOnOff(uint8_t index)
{
	
   //crc=0x55;
	outputBuf[0]='T'; //4D //Touch Key
	outputBuf[1]='K'; //58
	outputBuf[2]='P'; //4C	// 'P' power
	outputBuf[3]=index; //53	// 'S' select light command, 'C' close all light command
	
    transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}
void SendData_Buzzer(void)
{
	
   //crc=0x55;
	outputBuf[0]='T'; //4D
	outputBuf[1]='K'; //58
	outputBuf[2]='Z'; //4C	// buzzer
	outputBuf[3]='Z'; //53	// buzzer
	
    transferSize=4;
	if(transferSize)
	{
		while(transOngoingFlag);
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send 
 * 
*********************************************************/
void SendData_Set_Wifi(uint8_t hdata)
{
	    outputBuf[0]='T'; //
		outputBuf[1]='K'; //58
		outputBuf[2]='W'; //W ->WIFI
		outputBuf[3]=hdata; 
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Set_Command(uint8_t hdata)

{
	    outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='C'; //"C" ->command
		outputBuf[3]=hdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}
	
}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Temp_Data(uint8_t tdata)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='M'; //"T"->temperature
		outputBuf[3]=tdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}
/*********************************************************
 * 
 * Function Name:void SendData_Time_Data(uint8_t tdata)
 * Function:send to set  timer timing value
 * 
*********************************************************/
void SendData_Time_Data(uint8_t tdata)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //58
		outputBuf[2]='T'; //"T"->timer timing
		outputBuf[3]=tdata; //53	//
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}
/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * 
*********************************************************/
void SendData_Works_Time(uint8_t tdata,uint8_t tdata_2)
{

        outputBuf[0]='T'; //4D
		outputBuf[1]='O'; //"T"->temperature
		outputBuf[2]=tdata; //53	//
		outputBuf[3]=tdata_2;
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

}

/*********************************************************
 * 
 * Function Name:void SendData_Temp_Data(uint8_t tdata)
 * Function:send temperature value 
 * Input Ref:
 *          0x050 ->power on ,0x51->power off 
 *          0x52 ->timer value  ,0x53 -> temperature value 
 *
*********************************************************/
void sendData_Response_Signal(uint8_t data)
{
	    outputBuf[0]='T'; //4D
		outputBuf[1]='K'; //"T"->temperature
		outputBuf[2]='R'; //response
		outputBuf[3]=data;
		
		transferSize=4;
		if(transferSize)
		{
			while(transOngoingFlag);
			transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}


}



/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

   	uint32_t isrflags   = READ_REG(USART1->ISR);
	uint32_t cr1its     = READ_REG(USART1->CR1);
	uint32_t cr3its     = READ_REG(USART1->CR3);
   
	if(huart==&huart1) // Motor Board receive data (filter)
	{
		switch(state)
		{
		case 0:  //#0
			if(inputBuf[0]=='M')  //hex :4D - "M" -fixed mainboard
				state=1; //=1
			break;
		case 1: //#1
			if(inputBuf[0]=='A') //hex : 41 -'A'  -fixed master
			{
				state=2; 
			}
			else
				state=0; 
			break;
		case 2://#2
			if(inputBuf[0]=='D' || inputBuf[0]=='W'   || inputBuf[0]=='P' ||inputBuf[0] =='C' || inputBuf[0] == 'B' \
			  || inputBuf[0]=='T' || inputBuf[0]=='R' || inputBuf[0]=='S' ) //'D'->data , 'W' ->wifi
			{
				
				if(inputBuf[0]=='D') run_t.wifi_orderByMainboard_label=PANEL_DATA; //receive data is single data
                else if(inputBuf[0]=='W') run_t.wifi_orderByMainboard_label = WIFI_INFO; //wifi data
                else if(inputBuf[0]=='P') run_t.wifi_orderByMainboard_label = WIFI_TEMP;//temperature 
				else if(inputBuf[0]=='C') run_t.wifi_orderByMainboard_label = WIFI_CMD; //command 
				else if(inputBuf[0]=='B') run_t.wifi_orderByMainboard_label = WIFI_BEIJING_TIME;
				else if(inputBuf[0]=='T') run_t.wifi_orderByMainboard_label = WIFI_SET_TIMING;
				else if(inputBuf[0]=='R') run_t.wifi_orderByMainboard_label = WIFI_REF_DATA;
				else if(inputBuf[0]=='S') run_t.wifi_orderByMainboard_label = WIFI_FAN_DATA;
			    state=3;
			}
			else
				state=0;
			break;
            
        case 3:

            switch(run_t.wifi_orderByMainboard_label){
             case PANEL_DATA: //3
                 run_t.gReal_humtemp[0]=inputBuf[0]; //Humidity value 
                 state = 4;  
            break;
            case WIFI_INFO : //03

                 switch(inputBuf[0]){

                  case 0x01:
                 
                    run_t.wifi_link_cloud_flag =WIFI_CLOUD_SUCCESS;
                    state=0;
                    //run_t.decodeFlag=1;

                 break;

                 case 0x0:
                  
                    run_t.wifi_link_cloud_flag =WIFI_CLOUD_FAIL;
                   
                    state=0;
                   // run_t.decodeFlag=1;

                 break;

                 case 0x52: // link tencent cloud receive return data flag
				
					//run_t.wifi_receive_led_fast_led_flag =1;
                    run_t.wifi_led_fast_blink_flag=1;
                    state=0;
		            run_t.decodeFlag=0;
                    
				  
                 break;

                 case 0x54 : //power on return confirm flag
                
                    
                      run_t.response_power_on =1;
                       state=0;
		            run_t.decodeFlag=1;

                 break;

                 case 0x53: //power off return confirm flag
              
                    
                    run_t.response_power_off =1;
                     state=0;
		            run_t.decodeFlag=1;

                  
                 break;

                 

                 }
             
            break;

            case WIFI_TEMP ://4 //wifi setup temperature of value
            
				 run_t.set_temperature_decade_value =inputBuf[0]/10;
				 run_t.set_temperature_unit_value =inputBuf[0]%10;
				 state=0;
                 run_t.decodeFlag=1;
            break;

            case WIFI_CMD: //
                 run_t.wifiCmd[0] =inputBuf[0];
                 state=0;
                 run_t.decodeFlag=1; 
            break;

			 case WIFI_BEIJING_TIME:
			 	
			  	 run_t.dispTime_hours  = inputBuf[0];
                 state = 4; 
             break;

             case WIFI_SET_TIMING:
             	run_t.dispTime_hours  = inputBuf[0];
			
			 
             	state=0;
                run_t.decodeFlag=1; 
 
             break;

			 case WIFI_REF_DATA:

			    run_t.gDry = inputBuf[0];
				state = 4; 
				 
			break;

			case WIFI_FAN_DATA:
				 run_t.wifiCmd[0] =inputBuf[0];
                 state=0;
                 run_t.decodeFlag=1; 

			break;
			 
			 

         	}

            
        break;
        
		case 4: //

		switch(run_t.wifi_orderByMainboard_label){

		case WIFI_BEIJING_TIME:

		 	  run_t.dispTime_minutes = inputBuf[0];
				state =5;
		 
		 break;

		 case PANEL_DATA:
		
              run_t.gReal_humtemp[1]=inputBuf[0]; //temperature value
			
		     state=0;
		     run_t.decodeFlag=1;
         break;

		 case WIFI_REF_DATA :
		
			run_t.gPlasma = inputBuf[0];
			 state = 5; 


		 break;
		}

		 break;
           
        case 5: 
		if(run_t.wifi_orderByMainboard_label == WIFI_BEIJING_TIME){
				 run_t.dispTime_seconds = inputBuf[0];
				// run_t.send_app_timer_total_minutes_data = run_t.dispTime_seconds* 60;
				 run_t.decodeFlag=1;
			    state=0;
		 }
		 else if(run_t.wifi_orderByMainboard_label ==WIFI_REF_DATA ){

            
		     run_t.gUltrasonic = inputBuf[0];
			  state=0;
             run_t.decodeFlag=1; 


		 }
		 
            
        break;

		default:
			
		break;

		}

		/* Çå³ýÖÐ¶Ï±êÖ¾ */
	SET_BIT(USART1->ICR, UART_CLEAR_PEF);
	SET_BIT(USART1->ICR, UART_CLEAR_FEF);
	SET_BIT(USART1->ICR, UART_CLEAR_NEF);
	SET_BIT(USART1->ICR, UART_CLEAR_OREF);
	SET_BIT(USART1->ICR, UART_CLEAR_IDLEF);
	SET_BIT(USART1->ICR, UART_CLEAR_TCF);
	SET_BIT(USART1->ICR, UART_CLEAR_LBDF);
	SET_BIT(USART1->ICR, UART_CLEAR_CTSF);
	SET_BIT(USART1->ICR, UART_CLEAR_CMF);
	SET_BIT(USART1->ICR, UART_CLEAR_WUF);
	SET_BIT(USART1->ICR, UART_CLEAR_TXFECF);
	HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
	}
}
void USART1_Cmd_Error_Handler(void)
{
   uint32_t temp;
   

	

	  if(run_t.gTimer_usart_error >6){
	  	run_t.gTimer_usart_error=0;
	
           __HAL_UART_CLEAR_OREFLAG(&huart1);
        //    __HAL_UART_CLEAR_NEFLAG(&huart1);
        //    __HAL_UART_CLEAR_FEFLAG(&huart1);
           
          
          temp=USART1->ISR;
          temp = USART1->RDR;
		  
     
		  UART_Start_Receive_IT(&huart1,inputBuf,1);
       
		  
          
         }
}

        
/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart1)
	{
		transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
	}
	
}

