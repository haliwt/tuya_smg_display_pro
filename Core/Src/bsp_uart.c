#include "bsp_uart.h"
#include "bsp.h"

usart_rx_tx_t usart_t;


#define BSP_TIMEOUT		1		/* �������ʱʱ��, ��λms */
#define NUM			1			/* ѭ�����ʹ��� */
uint8_t ADD_NUM ;
/**********************************************************************************************************
*	�� �� ��: uint8_t Power_ReadParam_OnOff(uint8_t reg)
*	����˵��: ��������. ͨ������01Hָ��ʵ�֣�����֮�󣬵ȴ��ӻ�Ӧ��
*	��    ��: ��
*	�� �� ֵ: 1 ��ʾ�ɹ���0 ��ʾʧ�ܣ�ͨ�ų�ʱ�򱻾ܾ���
**********************************************************************************************************/
uint8_t Power_ReadParam_OnOff(uint8_t reg)
{	
	uint8_t i,j;
	
	for (i = 0; i < (NUM+ADD_NUM); i++)
	{
		SendData_PowerOnOff(reg);		  /* �������� */
		//time1 = bsp_GetRunTime();	/* ��¼����͵�ʱ�� */
		usart_t.gTimer_receive_times =0;
		
		while (1)				/* �ȴ�Ӧ��,��ʱ����յ�Ӧ����break  */
		{
			bsp_Idle();

			if (usart_t.gTimer_receive_times > BSP_TIMEOUT)		
			{
				break;		/* ͨ�ų�ʱ�� */
			}
			
			if (reg == 1 )
			{
               if(run_t.response_power_on ==1)
				break;		/* ���յ�Ӧ�� */
			}
			else{
				if(run_t.response_power_off ==1)
				break;		/* ���յ�Ӧ�� */

			}
		}
		
		if (reg == 1 )
		{
           if(run_t.response_power_on ==1)
			break;		/* ���յ�Ӧ�� */
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
			break;		/* ���յ�Ӧ�� */
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
		return 1 ;/* ���յ�Ӧ�� */
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
*	�� �� ��: uint8_t Power_ReadParam_OnOff(uint8_t reg)
*	����˵��: ��������. ͨ������01Hָ��ʵ�֣�����֮�󣬵ȴ��ӻ�Ӧ��
*	��    ��: ��
*	�� �� ֵ: 1 ��ʾ�ɹ���0 ��ʾʧ�ܣ�ͨ�ų�ʱ�򱻾ܾ���
**********************************************************************************************************/
uint8_t Wifi_LoginParam_On(void)
{	
	uint8_t i;
	
	for (i = 0; i < (NUM+ADD_NUM); i++)
	{
		 SendData_Set_Wifi(0x01);		  /* �������� */
		//time1 = bsp_GetRunTime();	/* ��¼����͵�ʱ�� */
		usart_t.gTimer_receive_times =0;
		
		while (1)				/* �ȴ�Ӧ��,��ʱ����յ�Ӧ����break  */
		{
			bsp_Idle();

			if (usart_t.gTimer_receive_times > BSP_TIMEOUT)		
			{
				break;		/* ͨ�ų�ʱ�� */
			}
			
			
            if(run_t.wifi_led_fast_blink_flag ==1){
				break;		/* ���յ�Ӧ�� */
			}
			

			
		}
		
		
        if(run_t.wifi_led_fast_blink_flag ==1){
			break;		/* ���յ�Ӧ�� */
		 
		}
		else{
			
		   ADD_NUM++;
        }
	}
	

    if(run_t.wifi_led_fast_blink_flag ==1)
		return 1 ;/* ���յ�Ӧ�� */
	else
	   return 0;
	
	
}


