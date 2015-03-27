/*********************************************************************************************
* File��	main.c
* Author:	embest
* Desc��	c main entry
* History:	
*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*	 								include files						 				    */
/*------------------------------------------------------------------------------------------*/
#include "2410lib.h"
#include "sys_init.h"
#include "fonts.h"
#include "led16x16.h"
#include "uart_test.h"

char *string;
/*********************************************************************************************
* name:		main
* func:		c code entry
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
int main(void)
{
    sys_init();						// ϵͳ��ʼ��

	uart_printf("\n Please Look At The 16X16 LEDS\n");
	led_init(); 					// ��ʼ��LED��ʵ
	char_out(0, "LOG");
	for(;;)
	{
		char_out(0," WELCOME TO EMBEST EDUKIT IV ");
		char_out(0,"^_^");
		//uart1_test();
		if (string == "��Ң")
			;
		//char_out(0, string);
	}	

}


