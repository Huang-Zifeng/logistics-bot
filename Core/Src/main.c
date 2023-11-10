/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pid.h"
#include "motor.h"
#include "openmv.h"
#include "oled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KP 14000.0 // �ر�Ti,Td,��Kp����ֱ������һ�γ�����һ���𵴣�Ϊֹ
#define TI 50 //Ti������̬����˵��Ki����̫��
#define TD 1 //����֮���������ˣ����
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
float currentVelocity1;//��õ��Aת��
float targetVelocity1;//Ŀ����Aת��

float currentVelocity2;//��õ��Bת��
float targetVelocity2;//Ŀ����Bת��

float currentVelocity3;//��õ��Cת��
float targetVelocity3;//Ŀ����Cת��

float currentVelocity4;//��õ��Dת��
float targetVelocity4;//Ŀ����Dת��

int time = 0;
int key_state=0;//��ť״̬
uint8_t usart2_rxbuff;

uint8_t cx=0,cy=0,cw=0,ch=0,state=0,sequency=0;//��openmv���յ���6������
int seq_arr[3]={0};//����ͬ��sequency��Ӧ�ɲ�ͬ��ץȡ˳��
int location=0;//С������λ��,ȡֵΪ0��1��2��0��ʾ�ڻ�����1
int move_distance=0;//С������Ŀ��λ����Ҫ�ƶ��ľ���
int color[3];//ÿ��λ������Ӧ��color,��openmv��ȡ
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void JustFloat_Send(float* send);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*JustFloatЭ��ͨ��(vofa)*/
void JustFloat_Send(float* send)
{
	uint8_t temp[8];
	memcpy(temp, send, sizeof(float));
	temp[4] = 0x00;
	temp[5] = 0x00;
	temp[6] = 0x80;
	temp[7] = 0x7f;
	HAL_UART_Transmit(&huart1,temp,sizeof(temp),2);
}
/*printf�ض���*/
int fputc(int ch, FILE *f)
{
	uint8_t temp[1] = {ch};
	HAL_UART_Transmit(&huart1, temp, 1, 2);
	return ch;
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	//uint8_t temp = 0x12;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_TIM9_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_USART1_UART_Init();
  MX_TIM12_Init();
  MX_USART2_UART_Init();
  MX_TIM8_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  Motor_Init();
  HAL_TIM_Base_Start_IT(&htim7);// ʹ�ܶ�ʱ��7�ж�
  OLED_Init();
  OLED_Display_On();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  while (1)
  {
		//printf("%f,%f,%f,%f,%f,%d\n",currentVelocity1, currentVelocity2, currentVelocity3, currentVelocity4, TARGET_VELOCITY, time);
		HAL_UART_Receive_IT(&huart2,(void *)&usart2_rxbuff,1);
		OLED_Refresh_Gram();
		OLED_ShowNumber(0,0,cx,3,12);
		OLED_ShowNumber(0,15,cy,3,12);
		OLED_ShowNumber(0,30,cw,3,12);
		OLED_ShowNumber(0,45,ch,3,12);
		OLED_ShowNumber(30,0,state,1,12);	
		OLED_ShowNumber(30,15,sequency,1,12);
		OLED_ShowNumber(30,30,seq_arr[0],1,12);
		OLED_ShowNumber(45,30,seq_arr[1],1,12);
		OLED_ShowNumber(60,30,seq_arr[2],1,12);
		if(!HAL_GPIO_ReadPin(BUTTON_GPIO_Port,BUTTON_Pin))
		{
			HAL_Delay(50);
			while(!HAL_GPIO_ReadPin(BUTTON_GPIO_Port,BUTTON_Pin))
				;
			HAL_Delay(50);
			if(key_state==0)
			{
				key_state=1;
			}
			else
			{
				key_state=0;
			}
				
		}
		if(key_state)
		{
			while(sequency==0);
			if(sequency==1)
			{
				seq_arr[0]=1;
				seq_arr[1]=2;
				seq_arr[2]=3;
			}
			else if(sequency==2)
			{
				seq_arr[0]=1;
				seq_arr[1]=3;
				seq_arr[2]=2;
			}
			else if(sequency==3)
			{
				seq_arr[0]=3;
				seq_arr[1]=2;
				seq_arr[2]=1;
			}
			else if(sequency==4)
			{
				seq_arr[0]=3;
				seq_arr[1]=1;
				seq_arr[2]=2;
			}
			else if(sequency==5)
			{
				seq_arr[0]=2;
				seq_arr[1]=1;
				seq_arr[2]=3;
			}
			else if(sequency==6)
			{
				seq_arr[0]=2;
				seq_arr[1]=3;
				seq_arr[2]=1;
			}
			else
			{
				OLED_ShowString(0,0,"Enter a number between");
				OLED_ShowString(0,20,"1 and 6 !");
			}
			HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
		 	Motor_Move_Time(1250,0,0,0.005);//ת90
			HAL_Delay(500);
		 	Motor_Move_Time(1000,0,1,0);//���������λ��1
		 	color[0]=state;
		 	if(color[0]==seq_arr[0])
		 	{
				Motor_Calibration(cx+cw,100,0.3,3);
				Servo_Grab();
				Servo_Put_Down();
		 	}
		// 	Motor_Move_Time(1250,0,1,0);
		// 	location++;
		// 	color[1]=state;
		// 	if(color[1]==seq_arr[0])
		// 	{
		// 	/*ץȡ1����code*/
		// 	}
		// 	Motor_Move_Time(1250,0,1,0);
		// 	location++;
		// 	color[2]=state;
		// 	if(color[2]==seq_arr[0])
		// 	{
		// 	/*ץȡ1����code*/
		// 	}
		// 	/*ץȡ1������ץȡ2��ʼ*/
		// 	if(color[0]==seq_arr[1])
		// 	{
		// 		move_distance=location;
			
		// 		/*ץȡ2����code*/
		// 	}
		// 	else if(color[1]==seq_arr[1])
		// 	{
		// 		move_distance=location-1;
				
		// 		/*ץȡ2����code*/
		// 	}
		// 	else if(color[2]==seq_arr[1])
		// 	{
		// 		move_distance=location-2;
				
		// 	}
		// 	location-=move_distance;
		// /*ץȡ2������ץȡ3��ʼ*/
		// 	if(color[0]==seq_arr[2])
		// 	{
		// 		move_distance=location;
		// 		/*ץȡ3����code*/
				
		// 	}
		// 	else if(color[1]==seq_arr[2])
		// 	{
		// 		move_distance=location-1;
		// 		/*ץȡ3����code*/
		// 	}
		// 	else if(color[2]==seq_arr[2])
		// 	{
		// 		move_distance=location-2;
		// 		/*ץȡ2����code*/
		// 	}
			key_state=0;
		}
		
		//HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_SET);
		//printf("Velocity1:%.2f  Velocity2:%.2f  Velocity3:%.2f  Velocity4:%.2f\n",currentVelocity1, currentVelocity2, currentVelocity3, currentVelocity4);
    /* USER CODE END WHILE */
	}
    /* USER CODE BEGIN 3 */
  
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
    if (htim == (&htim7))
    {
        int output1, output2, output3, output4;
		
		//��ȡ���A�ٶ�
		currentVelocity1 = Motor_Reverse(65535, &htim2, 0);	//A�����תʱ���������modeȡ0
		__HAL_TIM_SET_COUNTER(&htim2,0);  // ����������

		//���Ƶ��A�ٶ�
		output1 = pidController1(targetVelocity1, currentVelocity1, KP, TI, TD);
		if(output1>0)
		{	
			__HAL_TIM_SetCompare(&htim10, TIM_CHANNEL_1, output1);
			__HAL_TIM_SetCompare(&htim11,TIM_CHANNEL_1,0);
		}
		else
		{
			__HAL_TIM_SetCompare(&htim11, TIM_CHANNEL_1, -output1);
			__HAL_TIM_SetCompare(&htim10,TIM_CHANNEL_1,0);
		}
	
		//��ȡ���B�ٶ�
        currentVelocity2 = Motor_Reverse(65535, &htim3, 0); //B�����תʱ���������modeȡ0
        __HAL_TIM_SET_COUNTER(&htim3,0);  // ����������
      
		//���Ƶ��B�ٶ�
		output2 = pidController2(targetVelocity2, currentVelocity2, KP, TI, TD);
		if(output2>0)
		{	
			__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_2, output2);
			__HAL_TIM_SetCompare(&htim9,TIM_CHANNEL_1,0);
		}
		else
		{
			__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, -output2);
			__HAL_TIM_SetCompare(&htim9,TIM_CHANNEL_2,0);
		}
			
		//��ȡ���C�ٶ�
        currentVelocity3 = Motor_Reverse(65535, &htim4, 1); 
        __HAL_TIM_SET_COUNTER(&htim4,0);  // ����������
       
		//���Ƶ��C�ٶ�
		output3 = pidController3(targetVelocity3, currentVelocity3, KP, TI, TD);
		if(output3>0)
		{	
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, output3);
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);
		}
		else
		{
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, -output3);
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,0);
		}
		//��ȡ���D�ٶ�
        currentVelocity4 = Motor_Reverse(65535, &htim5, 1); 
        __HAL_TIM_SET_COUNTER(&htim5,0);  // ����������
				
		//���Ƶ��D�ٶ�
		output4 = pidController4(targetVelocity4, currentVelocity4, KP, TI, TD);
		if(output4>0)
		{	
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, output4);
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_3,0);
		}
		else
		{
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, -output4);
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,0);
		}
		time += 5;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint16_t tem;
	if(huart->Instance==USART2)
	{
		tem=usart2_rxbuff;
		OpenMV_Data_Receive(tem);
	}	
	HAL_UART_Receive_IT(&huart2,(void *)&usart2_rxbuff,1);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

