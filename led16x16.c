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
static void led_update(void) //LED����
{
	int j = 20;	  //���������ӳ�
	while(j--)
	{
		led_char_disp();	//��LED����ʾ�ַ�	
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
static void refresh_l_display_array(u8 bits, u8 *str) //ˢ��LED��ʾ����
{
	u32 i;
	u32 remaining_bits = 8-bits; //ʣ��λ�� 
	
	for(i=0;i<16;i++) //��16�з�����Ӧ����ʾ������
	{
		l_display_array[2*i] = (*str<<bits) |(*(str+16)>>remaining_bits); //��������LED��������
		l_display_array[2*(i+1)-1] = (*(str+16)<<bits) |(*(str+32)>>remaining_bits); //�����а��LED��������
		str++; //��ָ��ָ����һ����ַ
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
static void l_display_scroll ( u8 *str ) //������ʾ
{
	int i;
	for(i=0;i<8;i++)
	{
		refresh_l_display_array(i, str);   //ˢ��LED��ʾ����
		led_update(); //LED����
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
static void copy_data(u8 *dst, u8 *src, u32 size)//����ָ������������
{
	while(size--) 
		*(dst++) = *(src++);//��ԭ��ַ�����ݸ�ֵ��Ŀ���ַ�У�֮���ַ��1	
}

/*********************************************************************************************
* name:		refresh_assic_buffer
* func:		refresh buffer
* para:		str:points the new char
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
static void refresh_assic_buffer(u8 *str) //ˢ���ַ�����������
{
	copy_data(&assic_buffer[0], &assic_buffer[16],16); //���������ַ���2���Ƹ��ַ���1
	copy_data(&assic_buffer[16], &assic_buffer[32],16); //���������ַ���3���Ƹ��ַ���2
	copy_data(&assic_buffer[32], str,16);  //����һ���ַ����Ƹ��������ַ���3
	l_display_scroll(assic_buffer);	  //������ʾ�������ַ���
}

/*********************************************************************************************
* name:		char_out
* func:		display the chars
* para:		font:0  str:points of the chars
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void char_out(u8 font, u8 *str) //��ʾ�ַ�
{
	u8 *str_ptr;  //�����ַ�ָ��
	u8 glyph;	  //��ǰ��ʾ�ַ�
	
	glyph = ( u8 )*str;	//��Ҫ��ʾ���ַ�������ǰ��ʾ�ַ�
	
	while(glyph != '\0' )//����ǰ��ʾ�ַ����ǽ�����ʱ
	{
		str_ptr = fonts[font].ascii_code + ( glyph - fonts[font].ascii_beg) * fonts[font].ascii_height;
		refresh_assic_buffer(str_ptr); //����ǰ��ʾ�ַ����׵�ַ�����ַ�������
		str++;						   //�ַ�ָ���ַ��1
		glyph = ( u8 )*str;			   //��Ҫ��ʾ���ַ�������ǰ��ʾ�ַ�
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
	rGPBCON = rGPBCON & 0xfff0ff | 0x500;     		// GPB4,GPB5=01:Output ����Ϊ�����ʽ
	rGPCCON = rGPCCON & 0xffff3ffc | 0x4001;     	// GPC0,GPC7=01:Output ����Ϊ�����ʽ
	rGPDCON = rGPDCON & 0xffcfffff | 0x100000;     	// GPD10=01:Output	   ����Ϊ�����ʽ
	rGPGCON = rGPGCON & 0xfffcffff | 0x10000;     	// GPG8=01:Output	   ����Ϊ�����ʽ
}
