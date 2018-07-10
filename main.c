/*******************************************
��������:CCSv7.0
������:TIVA C Launchpad(TM4C123GH6PM)
������:16λ��ʱ�������ζ�ʱģʽ�������Զ�ʱģʽ
����˵��:
�����: Young sw
********************************************/
#include <stdint.h>
#include <stdbool.h>

/**
  ******************************************************************************
  * �ļ�����: main.c
  * ��    ��: By Sw Young
  * ��    ��: V1.0
  * ��    ��:
  * ��д����: 2018.3.29
  ******************************************************************************
  * ˵����
  * Ӳ��ƽ̨��
  *   MCUc:TM4C123��2�����߲��������DRV8825���������WiFi
  * ������˵����
  *   ͨ�����߾�ȷ����С����ǰ�������˾��룻��ת��ת�Ƕȡ�
  * Github��https://github.com/youngsw/Remote_Control_Car_PointRace_3_Car
  ******************************************************************************
**/
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "colorful_LED/colorful_LED.h"
#include "Pwm/pwm.h"
#include "Timer/Timer.h"
#include "oled.h"
#include "bmp.h"
#include "delay/delay.h"
#include "uart/uart.h"
#include "Beep/Beep.h"
#include "9.6'OLED/OLED.h"
#include "head.h"
#include "Control/Control.h"
void HardwareConfig(void)
{
    PwmConfig();            //��ʼ��PWM
    OLED_Init();            //��ʼ��OLED
    OLED_Clear();
    OLED_DrawBMP(0,0,128,8,BMP3);

}
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //��Ƶ����80M
    HardwareConfig();
    UnlockPixhawk();
    while(1)
    {

    }
}
