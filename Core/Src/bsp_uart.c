#include "bsp_uart.h"
#include "bsp.h"

usart_rx_tx_t usart_t;


#define TIMEOUT		0		/* �������ʱʱ��, ��λms */
#define NUM			1			/* ѭ�����ʹ��� */
uint8_t ADD_NUM ;
/*
*********************************************************************************************************
*	�� �� ��: MODH_ReadParam_01H
*	����˵��: ��������. ͨ������01Hָ��ʵ�֣�����֮�󣬵ȴ��ӻ�Ӧ��
*	��    ��: ��
*	�� �� ֵ: 1 ��ʾ�ɹ���0 ��ʾʧ�ܣ�ͨ�ų�ʱ�򱻾ܾ���
*********************************************************************************************************
*/
uint8_t Power_ReadParam_OnOff(uint8_t reg)
{	
	uint8_t i;
	
	for (i = 0; i < (NUM+ADD_NUM); i++)
	{
		SendData_PowerOnOff(reg);		  /* �������� */
		//time1 = bsp_GetRunTime();	/* ��¼����͵�ʱ�� */
		usart_t.gTimer_receive_times =0;
		
		while (1)				/* �ȴ�Ӧ��,��ʱ����յ�Ӧ����break  */
		{
			bsp_Idle();

			if (usart_t.gTimer_receive_times > TIMEOUT)		
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
		   else
		   ADD_NUM++;
		}
		else{
			if(run_t.response_power_off ==1)
			break;		/* ���յ�Ӧ�� */
			else
				ADD_NUM++;

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



