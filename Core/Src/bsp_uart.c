#include "bsp_uart.h"
#include "bsp.h"

usart_rx_tx_t usart_t;


#define BSP_TIMEOUT		1		/* 接收命令超时时间, 单位ms */
#define NUM			1			/* 循环发送次数 */
uint8_t ADD_NUM ;
/**********************************************************************************************************
*	函 数 名: uint8_t Power_ReadParam_OnOff(uint8_t reg)
*	功能说明: 单个参数. 通过发送01H指令实现，发送之后，等待从机应答。
*	形    参: 无
*	返 回 值: 1 表示成功。0 表示失败（通信超时或被拒绝）
**********************************************************************************************************/
uint8_t Power_ReadParam_OnOff(uint8_t reg)
{	
	uint8_t i,j;
	
	for (i = 0; i < (NUM+ADD_NUM); i++)
	{
		SendData_PowerOnOff(reg);		  /* 发送命令 */
		//time1 = bsp_GetRunTime();	/* 记录命令发送的时刻 */
		usart_t.gTimer_receive_times =0;
		
		while (1)				/* 等待应答,超时或接收到应答则break  */
		{
			bsp_Idle();

			if (usart_t.gTimer_receive_times > BSP_TIMEOUT)		
			{
				break;		/* 通信超时了 */
			}
			
			if (reg == 1 )
			{
               if(run_t.response_power_on ==1)
				break;		/* 接收到应答 */
			}
			else{
				if(run_t.response_power_off ==1)
				break;		/* 接收到应答 */

			}
		}
		
		if (reg == 1 )
		{
           if(run_t.response_power_on ==1)
			break;		/* 接收到应答 */
		   else{
		   	  if(j==0){
			  	j++;
		      ADD_NUM++;

		   	  }
			  else 
			  	break;

		   }
		}
		else{
			if(run_t.response_power_off ==1)
			break;		/* 接收到应答 */
			else{
				if(j==0){
			  	j++;
		      ADD_NUM++;

		   	  }
			  else 
			  	break;

			}

		}
	}
	
	if (reg == 1 )
	{
       if(run_t.response_power_on ==1)
		return 1 ;/* 接收到应答 */
	   else
	   	return 0;
	}
	else{
		
		if(run_t.response_power_off ==1)
			return 1;
        else 
			return 0;
	}
}



/**********************************************************************************************************
*	函 数 名: uint8_t Power_ReadParam_OnOff(uint8_t reg)
*	功能说明: 单个参数. 通过发送01H指令实现，发送之后，等待从机应答。
*	形    参: 无
*	返 回 值: 1 表示成功。0 表示失败（通信超时或被拒绝）
**********************************************************************************************************/
uint8_t Wifi_LoginParam_On(void)
{	
	uint8_t i;
	
	for (i = 0; i < (NUM+ADD_NUM); i++)
	{
		 SendData_Set_Wifi(0x01);		  /* 发送命令 */
		//time1 = bsp_GetRunTime();	/* 记录命令发送的时刻 */
		usart_t.gTimer_receive_times =0;
		
		while (1)				/* 等待应答,超时或接收到应答则break  */
		{
			bsp_Idle();

			if (usart_t.gTimer_receive_times > BSP_TIMEOUT)		
			{
				break;		/* 通信超时了 */
			}
			
			
            if(run_t.wifi_led_fast_blink_flag ==1){
				break;		/* 接收到应答 */
			}
			

			
		}
		
		
        if(run_t.wifi_led_fast_blink_flag ==1){
			break;		/* 接收到应答 */
		 
		}
		else{
			
		   ADD_NUM++;
        }
	}
	

    if(run_t.wifi_led_fast_blink_flag ==1)
		return 1 ;/* 接收到应答 */
	else
	   return 0;
	
	
}


