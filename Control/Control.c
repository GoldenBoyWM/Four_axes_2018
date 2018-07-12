/*
 * Control.c
 *
 *  Created on: 2018��7��10��
 *      Author: Sw Young
 */
#include "head.h"
#include "colorful_LED/colorful_LED.h"
#include "Pwm/pwm.h"
#include "Timer/Timer.h"
#include "oled.h"
#include "delay/delay.h"
#include "uart/uart.h"
#include "Beep/Beep.h"
#include "9.6'OLED/OLED.h"
#include "Control.h"

/*
 * ������ʼ��
 */
//�߶�:��λ MM
uint16_t Goal_Distance = 800;//Ĭ�϶���ֵ800mm
uint16_t Real_Distance = 0;
uint16_t Error_Distance = 0;

uint16_t Goal_XCoordinate,Goal_YCoordinate;
uint16_t Real_XCoordinate,Real_YCoordinate;
extern volatile uint8_t get_x, get_y;

uint8_t Control_Open = 0;

/*ң����У׼ֵ
 *CH1 1100-1950
 *CH2 1100-1950
 *CH3 1100-1950
 *CH4 1100-1950
 *CH5 1100(����)-1520(����)-1950(����)
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
void LockPixhawk(void)
{
    PwmControl_3(1100);
    PwmControl_4(1950);//����
    Delay_ms(1000);
    Delay_ms(1000);
    Delay_ms(1000);
    PwmControl_4(1520);
}
void LandMode(void)
{
    PwmControl_5(1950); //����ģʽ
    PwmControl_1(1520);
    PwmControl_2(1520);
    PwmControl_3(1100);
    PwmControl_4(1520);
}
void Get_Coordinate(void)
{
    get_x = Real_XCoordinate;
    get_y = Real_YCoordinate;
}
void Get_Distance(void)
{
    Real_Distance = GetAverageDistance();
}
void AltitudeHold(void)
{
    Real_Distance = GetAverageDistance();
    if(Real_Distance>Goal_Distance)
        Error_Distance = Real_Distance-Goal_Distance;
    else if(Goal_Distance>Real_Distance)
        Error_Distance = Goal_Distance-Real_Distance;


}
