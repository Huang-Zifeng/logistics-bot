/**
 * @file motor.c
 * @author ������
 * @brief ���Ƶ���Ͷ���˶��ĸ��ֺ���
 * @version 0.1
 * @date 2022-08-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "motor.h"
#include "tim.h"
#include "math.h"

static float angle1=((SERVOR_INIT_1/ARR)-0.025f)*1800;
static float angle2=((SERVOR_INIT_2/ARR)-0.025f)*1800;
static float angle3=((SERVOR_INIT_3/ARR)-0.025f)*1800;
static float angle4=((SERVOR_INIT_4/ARR)-0.025f)*1800;
static float angle5=((SERVOR_INIT_5/ARR)-0.025f)*1800;
static float angle6=((SERVOR_INIT_6/ARR)-0.025f)*1800;
extern float targetVelocity1, targetVelocity2, targetVelocity3, targetVelocity4;
extern int time;
/**
 * @brief ����ת�٣������תʱ���Բ�������ٶ�
 * 
 * @param arr 
 * @param htim 
 * @param mode ��תʱ�����������������mode=0����֮mode=1
 * @return float 
 */
float Motor_Reverse(int arr, TIM_HandleTypeDef *htim, int mode)
{
	float temp, velocity;
	temp = (float)(__HAL_TIM_GET_COUNTER(htim))/(330*4*0.005);// TIM��������õ�����壬�õ����5ms����������/18��Ϊʵ��ת�ٵ�rpm	
	if((int)temp<100)
	{
		if(mode == 0)
			velocity = -temp;
		else
			velocity = temp;
	}
	else
	{
		if(mode == 0)
			velocity = (float)(arr+1)/(330*4*0.005)-temp;
		else
			velocity = temp-(float)(arr+1)/(330*4*0.005) ;
	}
	return velocity;
}
/**
 * @brief �õ����һ�����ٶ�ת��һ��ʱ��
 * 
 * @param motor_type ������
 * @param t �����תʱ�䣬��λΪms
 * @param speed ���ת��
 */
void Motor_Time(int motor_type, float t, float speed)
{
	if(time<t)
	{
		switch (motor_type)
		{
		case 1:
			targetVelocity1 = speed;
			break;
		case 2:
			targetVelocity2 = speed;
			break;
		case 3:
			targetVelocity3 = speed;
			break;
		case 4:
			targetVelocity4 = speed;
			break;
		default:
			break;
		}
	}
	else 
	{
		switch (motor_type)
		{
		case 1:
			targetVelocity1 = 0;
			break;
		case 2:
			targetVelocity2 = 0;
			break;
		case 3:
			targetVelocity3 = 0;
			break;
		case 4:
			targetVelocity4 = 0;
			break;
		default:
			break;
		}
	}
}
/**
 * @brief �û�������һ�����ٶ��˶�һ��ʱ��
 * 
 * @param t �������˶�ʱ��
 * @param x_speed x���ٶ�
 * @param y_speed y���ٶ�
 * @param z_speed z���ٶȣ����ٶ�
 */
void Motor_Move_Time(int t, float x_speed, float y_speed, float z_speed)
{
	time =0;
	while(time<t)//�޷�����ѭ������ΪOptimization��level����level-0
	{
		//�����ķ���˶����
		
		targetVelocity1=x_speed+y_speed-z_speed*(HALF_OF_H+HALF_OF_W);
		targetVelocity2=x_speed-y_speed-z_speed*(HALF_OF_H+HALF_OF_W);
		targetVelocity3=x_speed+y_speed+z_speed*(HALF_OF_H+HALF_OF_W);
		targetVelocity4=x_speed-y_speed+z_speed*(HALF_OF_H+HALF_OF_W);
	}
	targetVelocity1=0;
	targetVelocity2=0;
	targetVelocity3=0;
	targetVelocity4=0;
	time =0;
	
}
/**
 * @brief �û�������һ�����ٶ��˶�
 * 
 * @param x_speed x���ٶ�
 * @param y_speed y���ٶ�
 * @param z_speed z���ٶȣ����ٶ�
 */
void Motor_Move(float x_speed, float y_speed, float z_speed)
{
		targetVelocity1=x_speed+y_speed-z_speed*(HALF_OF_H+HALF_OF_W);
		targetVelocity2=x_speed-y_speed-z_speed*(HALF_OF_H+HALF_OF_W);
		targetVelocity3=x_speed+y_speed+z_speed*(HALF_OF_H+HALF_OF_W);
		targetVelocity4=x_speed-y_speed+z_speed*(HALF_OF_H+HALF_OF_W);
}
/**
 * @brief ��ʼ������Ͷ��
 * 
 */
void Motor_Init(void)
{
	/*���A,���£���ת���*/
	__HAL_TIM_SetCompare(&htim10,TIM_CHANNEL_1,0);//����ռ�ձ�,��ʱ��ת
	HAL_TIM_PWM_Start(&htim10,TIM_CHANNEL_1);
	__HAL_TIM_SetCompare(&htim11,TIM_CHANNEL_1,0);
	HAL_TIM_PWM_Start(&htim11,TIM_CHANNEL_1);
	/*���B�����ϣ���ת���*/
	__HAL_TIM_SetCompare(&htim9,TIM_CHANNEL_1,0);
	HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);
	__HAL_TIM_SetCompare(&htim9,TIM_CHANNEL_2,0);//����ռ�ձȣ���ʱ��ת
	HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_2);
	/*���C������*/
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,0);//����ռ�ձȣ���ʱ��ת
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,0);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	/*���D������*/
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_3,0);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_4,0);//����ռ�ձȣ���ʱ��ת
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	
	
	__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,SERVOR_INIT_4);//C8(��Ӧ���4),
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
	__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,SERVOR_INIT_3);//C9(��Ӧ���3),����ռ�ձȲ�����Ϊ0���߸���
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_4);
	
	__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,SERVOR_INIT_5);//C7(��Ӧ���5)
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
	__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,SERVOR_INIT_6);//C6(��Ӧ���6)
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
	__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_1,SERVOR_INIT_2);//B14(��Ӧ���2)
	HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_1);
	__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_2,SERVOR_INIT_1);//B15(��Ӧ���1)��������Ķ����������������Ϊ1��2��3��4��5��6��
	HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2);
	
	HAL_Delay(500);
	__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,750);
	HAL_Delay(500);
	__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,500);
	
	
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
}
/**
 * @brief ���ƶ�����ٵ�ת����һ���Ƕ�
 * @reference http://t.csdn.cn/8ALSs
 * @param object ������󣬴�������������1��2��3��4��5��6.
				���1���������¿�����ֵ������˳ʱ�뷽��ת
				���4����ֵ������˳ʱ�뷽��ת
 * @param angle ���ת����ָ���ĽǶȣ���Χ��0-180��
 * @note: angle=((PWM/ARR)-0.025)*1800,PWM=(int)(angle/1800+0.025)*ARR
 */
void Servo_Angle(int object, float angle,int time)
{
	int temp1 = (int)((angle/1800.0f+0.025f)*ARR);
	int temp2;
	switch (object)
	{
	case 1:
		//__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_2,temp1);
		 temp2 = (int)((angle1/1800.0f+0.025f)*ARR);
		if(temp1>temp2)
		{
			int counter = (temp1-temp2)/STEP_SIZE;
			int bias = (temp1-temp2)%STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2+=STEP_SIZE;
				__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_2,temp2);
				HAL_Delay(time);			
			}
			temp2+=bias;
			__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_2,temp2);
		}
		else
		{
			int counter = (temp2-temp1)/STEP_SIZE;
			int bias = (temp2-temp1)%STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2-=STEP_SIZE;
				__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_2,temp2);
				HAL_Delay(time);
			}
			temp2-=bias;
			__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_2,temp2);
		}
		angle1=angle;
		break;
	case 2:
		temp2 = (int)((angle2/1800.0f+0.025f)*ARR);
		//__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_1,temp1);
		if(temp1>temp2)
		{
			int counter = (temp1-temp2)/STEP_SIZE;
			int bias = (temp1-temp2)%STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2+=STEP_SIZE;
				__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_1,temp2);
				HAL_Delay(time);			
			}
			temp2+=bias;
			__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_1,temp2);
		}
		else
		{
			int counter = (temp2-temp1)/STEP_SIZE;
			int bias = (temp2-temp1)%STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2-=STEP_SIZE;
				__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_1,temp2);
				HAL_Delay(time);
			}
			temp2-=bias;
			__HAL_TIM_SetCompare(&htim12,TIM_CHANNEL_1,temp2);
		}
		angle2=angle;
		break;
	case 3:
		temp2 = (int)((angle3/1800.0f+0.025f)*ARR);
		//__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,temp1);
		if(temp1>temp2)
		{
			int counter = (temp1-temp2)/STEP_SIZE;
			int bias = (temp1-temp2)%STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2+=STEP_SIZE;
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,temp2);
				HAL_Delay(time);			
			}
			temp2+=bias;
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,temp2);
		}
		else
		{
			int counter = (temp2-temp1)/STEP_SIZE;
			int bias = (temp2-temp1)%STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2-=STEP_SIZE;
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,temp2);
				HAL_Delay(time);
			}
			temp2-=bias;
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_4,temp2);
		}
		angle3=angle;
		break;	
	case 4:
		temp2 = (int)((angle4/1800.0f+0.025f)*ARR);
		 //__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,temp1);
		if(temp1>temp2)
		{
			int counter = (temp1-temp2)/STEP_SIZE;
			int bias = (temp1-temp2)%STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2+=STEP_SIZE;
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,temp2);
				HAL_Delay(time);			
			}
			temp2+=bias;
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,temp2);
		}
		else
		{
			int counter = (temp2-temp1)/STEP_SIZE;
			int bias = (temp2-temp1)%STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2-=STEP_SIZE;
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,temp2);
				HAL_Delay(time);
			}
			temp2-=bias;
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_3,temp2);
		}
		angle4=angle;
		break;
	case 5:
		temp2 = (int)((angle5/1800.0f+0.025f)*ARR);
		//__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,temp1);
		if(temp1>temp2)
		{
			int counter = (temp1-temp2)/STEP_SIZE;
			int bias = (temp1-temp2)%STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2+=STEP_SIZE;
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,temp2);
				HAL_Delay(time);			
			}
			temp2+=bias;
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,temp2);
		}
		else
		{
			int counter = (temp2-temp1)/STEP_SIZE;
			int bias = (temp2-temp1)%STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2-=STEP_SIZE;
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,temp2);
				HAL_Delay(time);
			}
			temp2-=bias;
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,temp2);
		}
		angle5=angle;
		break;
	case 6:
		temp2 = (int)((angle6/1800.0f+0.025f)*ARR);
		//__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,temp2);
		if(temp1>temp2)
		{
			int counter = (temp1-temp2)/STEP_SIZE;
			int bias = (temp1-temp2)%STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2+=STEP_SIZE;
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,temp2);
				HAL_Delay(time);			
			}
			temp2+=bias;
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,temp2);
		}
		else
		{
			int counter = (temp2-temp1)/STEP_SIZE;
			int bias = (temp2-temp1)%STEP_SIZE;
			int i;
			for(i=0;i<counter;i++)
			{
				temp2-=STEP_SIZE;
				__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,temp2);
				HAL_Delay(time);
			}
			temp2-=bias;
			__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,temp2);
		}
		angle6=angle;
		break;
	default:
		break;
	}
}
/**
 * @brief ͨ��openmv���������������ݽ���λ��У׼���Զ�׼��ץȡ������
 * 
 * @param location ������ͼ���е�ʵʱλ��
 * @param target ������ͼ���е�Ŀ��λ��
 * @param speed ������У׼�����еĵ���ٶ�
 * @param count У׼����
 * @shortage ʹ���˱Ƚϵͼ��Ŀ����㷨�����ڿɸ�Ϊpid
 */
void Motor_Calibration(int location, int target, float speed, int count)
{
	for(int i=0;i<count;i++)
	{
		if(location>target)
			while(location>target)
				Motor_Move(0,speed,0);
		else if(location<target)
			while(location<target)
				Motor_Move(0,-speed,0);
		Motor_Move(0,0,0);
	}
}
/**
 * @brief ���ƶ�����»�е�۲�ץȡ����
 * 
 */
void Servo_Grab(void)
{
	Servo_Angle(4,15,50);
	Servo_Angle(2,35,50);
	HAL_Delay(1500);
	Servo_Angle(4,30,100);
	Servo_Angle(3,35,100);
	HAL_Delay(1500);
	Servo_Angle(6,20,100);
	Servo_Angle(5,10,100);
	HAL_Delay(1500);
}
/**
 * @brief ���ƶ����ץȡ���������
 * 
 */
void Servo_Put_Down(void)
{
	Servo_Angle(2,160,100);
	Servo_Angle(4,135,100);
	HAL_Delay(1000);
	Servo_Angle(1,180,100);
	Servo_Angle(2,90,100);
	HAL_Delay(1000);
	Servo_Angle(6,0,100);
	HAL_Delay(1000);
	Servo_Angle(3,90,100);
	Servo_Angle(1,90,100);
	Servo_Angle(3,10,100);
	Motor_Init();
}
/**
 * @brief ���ƻ����˽����ƶ�
 * 
 * @param move_distance �ƶ��ľ��뵥λ
 * @param speed �ƶ��ٶ�
 */
void Motor_Move_Distance(int move_distance,int speed)
{
	const int STEP=1500;
	if(move_distance<0)
		speed = -speed;
	time = move_distance*STEP;
	Motor_Move_Time(time,0,speed,0);
}
