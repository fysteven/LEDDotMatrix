/*********************************************************************************************
* File:		Dotled.c
* Author:	embest
* Desc:		DotLed_Test 
* History:	
*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*	 								include files						 				    */
/*------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "2410lib.h"
#include "fonts.h"
#include "led16x16.h"

/*------------------------------------------------------------------------------------------*/
/*	 								function declare					 				    */
/*------------------------------------------------------------------------------------------*/
extern void led_char_disp(void);

/*------------------------------------------------------------------------------------------*/
/*	 								global variables						 				*/
/*------------------------------------------------------------------------------------------*/
u8 l_display_array[2*16];
u8 assic_buffer[3*16];
/*============================================================================================
l_display_array:
+-----+-----+
|     |     |     
|  D  |  E  |  
|     |     |     
+-----+-----+
A buffer data and B buffer data -->D buffer data
B buffer data and C buffer data -->E buffer data

assic_buffer:
+-----+-----+-----+
|     |     |     |
|  A  |  B  |  C  |<---- update the C buffer and move the B buffer data to the A buffer
|     |     |     |   and move the C buffer data to the B buffer data
+-----+-----+-----+

============================================================================================*/

/*********************************************************************************************
* name:		led_update
* func:		refresh the led display
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static void led_update(void) //LED更新
{
	int j = 20;	  //变量用于延迟
	while(j--)
	{
		led_char_disp();	//在LED上显示字符	
	}	
}

/*********************************************************************************************
* name:		l_display_scroll
* func:		shift the display
* para:		bits:the position str:point the buffer
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static void refresh_l_display_array(u8 bits, u8 *str) //刷新LED显示数组
{
	u32 i;
	u32 remaining_bits = 8-bits; //剩余位数 
	
	for(i=0;i<16;i++) //将16行放入相应的显示数组中
	{
		l_display_array[2*i] = (*str<<bits) |(*(str+16)>>remaining_bits); //更新左半边LED点阵数组
		l_display_array[2*(i+1)-1] = (*(str+16)<<bits) |(*(str+32)>>remaining_bits); //更新有半边LED点阵数组
		str++; //将指针指向下一个地址
	}
}

/*********************************************************************************************
* name:		l_display_scroll
* func:		scroll the display
* para:		str:point the buffer
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static void l_display_scroll ( u8 *str ) //滚动显示
{
	int i;
	for(i=0;i<8;i++)
	{
		refresh_l_display_array(i, str);   //刷新LED显示数组
		led_update(); //LED更新
	}	
}

/*********************************************************************************************
* name:		copy_data
* func:		copy data
* para:		dst:point the dest data src:points the source data
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static void copy_data(u8 *dst, u8 *src, u32 size)//复制指定数量的数据
{
	while(size--) 
		*(dst++) = *(src++);//将原地址的内容赋值给目标地址中，之后地址加1	
}

/*********************************************************************************************
* name:		refresh_assic_buffer
* func:		refresh buffer
* para:		str:points the new char
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static void refresh_assic_buffer(u8 *str) //刷新字符缓冲区函数
{
	copy_data(&assic_buffer[0], &assic_buffer[16],16); //将缓冲区字符码2复制给字符码1
	copy_data(&assic_buffer[16], &assic_buffer[32],16); //将缓冲区字符码3复制给字符码2
	copy_data(&assic_buffer[32], str,16);  //将下一个字符复制给缓冲区字符码3
	l_display_scroll(assic_buffer);	  //滚动显示缓冲区字符码
}

/*********************************************************************************************
* name:		char_out
* func:		display the chars
* para:		font:0  str:points of the chars
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void char_out(u8 font, u8 *str) //显示字符
{
	u8 *str_ptr;  //定义字符指针
	u8 glyph;	  //当前显示字符
	
	glyph = ( u8 )*str;	//将要显示的字符赋给当前显示字符
	
	while(glyph != '\0' )//当当前显示字符不是结束符时
	{
		str_ptr = fonts[font].ascii_code + ( glyph - fonts[font].ascii_beg) * fonts[font].ascii_height;
		refresh_assic_buffer(str_ptr); //将当前显示字符的首地址放入字符缓冲区
		str++;						   //字符指针地址加1
		glyph = ( u8 )*str;			   //将要显示的字符赋给当前显示字符
	}
}


/*********************************************************************************************
* name:		led_init
* func:		initial the led display
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void led_init(void)
{
	rGPBCON = rGPBCON & 0xfff0ff | 0x500;     		// GPB4,GPB5=01:Output 设置为输出方式
	rGPCCON = rGPCCON & 0xffff3ffc | 0x4001;     	// GPC0,GPC7=01:Output 设置为输出方式
	rGPDCON = rGPDCON & 0xffcfffff | 0x100000;     	// GPD10=01:Output	   设置为输出方式
	rGPGCON = rGPGCON & 0xfffcffff | 0x10000;     	// GPG8=01:Output	   设置为输出方式
}
