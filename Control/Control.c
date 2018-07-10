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

/*ң����У׼ֵ
 *CH1 1100-1950
 *CH2 1100-1950
 *CH3 1100-1950
 *CH4 1100-1950
 *CH5 1100(����)-1520(����)-1950(����)
 */
void UnlockPixhawk(void)
{
    PwmControl_5(1100); //����ģʽ
    Delay_ms(1000);//��ʱ�ȴ�
    PwmControl_3(1100);
    PwmControl_4(1950);//����
    Delay_ms(3000);
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
    Delay_ms(3000);
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

