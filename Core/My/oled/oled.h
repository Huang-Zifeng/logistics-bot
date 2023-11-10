#ifndef __OLED_H
#define __OLED_H			  	 

#include "main.h"
#include <stdint.h>

//Oled port macro definition
//OLED�˿ں궨��
#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_RESET);   //RST
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_SET);   //RST

#define OLED_RS_Clr() HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_RESET);    //DC
#define OLED_RS_Set() HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_SET);     //DC

#define OLED_SCLK_Clr()  HAL_GPIO_WritePin(OLED_SCL_GPIO_Port,OLED_SCL_Pin,GPIO_PIN_RESET); //SCL
#define OLED_SCLK_Set()  HAL_GPIO_WritePin(OLED_SCL_GPIO_Port,OLED_SCL_Pin,GPIO_PIN_SET);   //SCL

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_RESET);   //SDA
#define OLED_SDIN_Set()  HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_SET);    //SDA
#define OLED_CMD  0	//Command //д����
#define OLED_DATA 1	//Data //д����

//Oled control function
//OLED�����ú���
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   				   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNumber(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const char *p);
void OLED_Set_Pos(unsigned char x, unsigned char y);
#endif  
	 
