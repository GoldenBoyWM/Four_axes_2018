/**
  ******************************************************************************
  * �ļ�����: contral.c
  * ��    ��: By Sw Young
  * ��    ��: V1.0
  * ��    ��:
  * ��д����: 2018.7.6
  ******************************************************************************
  * ˵����
  * Ӳ��ƽ̨��TM4C123G
  *   *****
  * ������˵����
  *   *****
  * Github��
  ******************************************************************************
**/
#include <0.96'OLED/OLED.h>
#include <0.96'OLED/OLED.h>
#include "head.h"
#include "colorful_LED/colorful_LED.h"
#include "Pwm/pwm.h"
#include "Timer/Timer.h"
#include "delay/delay.h"
#include "uart/uart.h"
#include "Beep/Beep.h"
#include "Control.h"

/*
 * ������ʼ��
 */
//�߶�:��λ MM
uint16_t Goal_Distance = 700;//Ĭ�϶���ֵ800mm
float volatile Real_Distance = 0,Last_Real_Distance = 0;
extern int int_distance;
uint16_t Error_Distance = 0;

extern int16_t int_pitch ,int_roll ,int_yaw;


int16_t RealAttitude_roll;
int16_t RealAttitude_pitch;
int16_t RealAttitude_yaw;

uint16_t err_roll=0,err_pitch=0;

uint16_t Goal_XCoordinate,Goal_YCoordinate;
uint16_t Real_XCoordinate,Real_YCoordinate;
extern volatile uint8_t get_x, get_y;

uint8_t Control_Open = 0;
bool Control_Serial = true;

/*ң����У׼ֵ
 *CH1 1100-1950
 *CH2 1100-1950
 *CH3 1100-1950
 *CH4 1100-1950
 *CH5 1100(����)-1520(����)-1950(����)
 */
/**
  * �� �� ��:UnlockPixhawk
  * ��������: Pixhawk��������
  * �������:
  * �� �� ֵ: ��
  * ˵    ��: ��
  *   By Sw Young
  *   2017.7.6
  */
void UnlockPixhawk(void)
{
    PwmControl_1(1520);
    PwmControl_2(1520);
    PwmControl_3(1100);
    PwmControl_4(1520);
    PwmControl_5(1100); //����ģʽ
    Delay_ms(1000);//��ʱ�ȴ�
    PwmControl_3(1100);
    PwmControl_4(1950);//����
    Delay_ms(1000);
    Delay_ms(1000);
    Delay_ms(1000);
    PwmControl_4(1520);

    PwmControl_1(1520);
    PwmControl_2(1520);
    PwmControl_3(1100);
    PwmControl_4(1520);

    PwmControl_5(1520); //����ģʽ

}
/**
  * �� �� ��:LockPixhawk
  * ��������: Pixhawk������������
  * �������:
  * �� �� ֵ: ��
  * ˵    ��: ��
  *   By Sw Young
  *   2017.7.6
  */
void LockPixhawk(void)
{
    PwmControl_3(1100);
    PwmControl_4(1100);//����
    Delay_ms(1000);
    Delay_ms(1000);
    Delay_ms(1000);
    PwmControl_4(1520);
}
/**
  * �� �� ��:LandMode
  * ��������: ����������ģʽ����
  * �������:
  * �� �� ֵ: ��
  * ˵    ��: ��
  *   By Sw Young
  *   2017.7.6
  */
void LandMode(void)
{
    PwmControl_5(1950); //����ģʽ
    PwmControl_1(1520);
    PwmControl_2(1520);
    PwmControl_3(1100);
    PwmControl_4(1520);
}
/**
  * �� �� ��:Led_Twinkle
  * ��������:   LED��˸����
  * �������: ��ɫ����˸����
  * �� �� ֵ: ��
  * ˵    ��: ��
  *   By Sw Young
  *   2017.7.6
  */
void Get_Coordinate(void)
{
    get_x = Real_XCoordinate;
    get_y = Real_YCoordinate;
}
/**
  * �� �� ��:Get_Distance
  * ��������: ��ȡ�������߶Ⱥ���
  * �������:
  * �� �� ֵ: ��
  * ˵    ��: ��
  *   By Sw Young
  *   2017.7.6
  */
void Get_Distance(void)
{
    Real_Distance = int_distance*10;//ת����mm
    if(fabs(Real_Distance-Last_Real_Distance)>500)
        Real_Distance = Last_Real_Distance;
    Last_Real_Distance = Real_Distance;
}
/**
  * �� �� ��:Get_Attitude
  * ��������: ��ȡ��̬
  * �������:
  * �� �� ֵ: ��
  * ˵    ��:
  *   By Sw Young
  *   2017.7.6
  */
void Get_Attitude(void)
{
    RealAttitude_pitch = int_pitch;
    RealAttitude_roll = int_roll;
    RealAttitude_yaw = int_yaw;
}
/**
  * �� �� ��:AltitudeHold
  * ��������: ����ģʽ����
  * �������:
  * �� �� ֵ: ��
  * ˵    ��:
  *   By Sw Young
  *   2017.7.6
  */
void AltitudeHold(void)
{
    //����Ŀ��߶�
    if(Goal_Distance-(int)Real_Distance > 100&&Goal_Distance-(int)Real_Distance <300)
    {
       PwmControl_3(1645);
    }
    else if(Goal_Distance-(int)Real_Distance > 300&&Goal_Distance-(int)Real_Distance <500)
    {
       PwmControl_3(1660);
    }
    else if(Goal_Distance-(int)Real_Distance > 500)
    {
       PwmControl_3(1690);
    }
    //����Ŀ��߶�
    else if((int)Real_Distance - Goal_Distance > 100&&(int)Real_Distance - Goal_Distance < 300)
    {
       PwmControl_3(1435);
    }
    else if((int)Real_Distance - Goal_Distance > 300&&(int)Real_Distance - Goal_Distance < 500)
    {
       PwmControl_3(1380);
    }
    else if((int)Real_Distance - Goal_Distance > 500)
    {
       PwmControl_3(1350);
    }
    //������
    else if((((int)Real_Distance-Goal_Distance<100)&&((int)Real_Distance-Goal_Distance>0))\
           ||((Goal_Distance-(int)Real_Distance<100)&&Goal_Distance-(int)Real_Distance>0))//�������� -100 ~ +100
    {
       PwmControl_3(channel_val_MID);
    }
}
/**
  * �� �� ��:OledDisplayInit
  * ��������: OLED��ʾ��ʼ������
  * �������:
  * �� �� ֵ: ��
  * ˵    ��: ��
  *   By Sw Young
  *   2017.7.6
  */
void OledDisplayInit(void)
{
    OLED_Clear();
    OLED_ShowString(16,0,"FourAxes2018",16);
    OLED_ShowString(6,3,"AxesState:",16);
    OLED_ShowString(90,3,"OFF",16);
    OLED_ShowString(6,6,"GoalDis:",16);
    OLED_ShowNum(50,6,Goal_Distance,4,16);
    OLED_ShowString(80,88,"MM",16);
}
/**
  * �� �� ��:Display
  * ��������: OLED��ʾ����
  * �������:
  * �� �� ֵ: ��
  * ˵    ��: ��
  *   By Sw Young
  *   2017.7.6
  */
void Display(void)
{
    OLED_ShowString(6,3,"AxesState:",16);
    if(Control_Open)
        OLED_ShowString(90,3,"ON ",16);
    else
        OLED_ShowString(90,3,"OFF",16);
    OLED_ShowString(6,6,"GoalDis:",16);
    OLED_ShowNum(56,6,Goal_Distance,4,16);
    OLED_ShowString(88,6,"MM",16);
}
/**
  * �� �� ��:AttitudeProtection
  * ��������: ��̬��������
  * �������:
  * �� �� ֵ: ��
  * ˵    ��: ��
  *   By Sw Young
  *   2017.7.6
  */
void AttitudeProtection(void)
{
    err_roll=fabs(RealAttitude_roll);
    err_pitch=fabs(RealAttitude_pitch);
    if(err_pitch>38||err_roll>38)
    {
        Control_Open = false;
        LandMode();
    }
}
