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
#include "stdio.h"

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
#include "utils/uartstdio.h"
#include "driverlib/uart.h"
#include "uart/uart.h"
#include "uart/uartstdio.h"
#include "Beep/Beep.h"
#include "9.6'OLED/OLED.h"
#include "head.h"
#include "Control/Control.h"
#include "sonar.h"


void HardwareConfig(void)
{
    Uart0Iint();        //����0��ʼ��
    Uart1Iint();        //����1��ʼ��

    PwmConfig();            //��ʼ��PWM

    LED_ColorInit();    //LED��ʼ��
    LED_Color(160, 0, 2);

    OLED_Init();            //��ʼ��OLED
    OLED_Clear();
    OLED_DrawBMP(0,0,128,8,BMP3);

    Sonar_Configure();
    Sonar_GPIOA_Configure();
    Sonar_GPIOA_Interrupt();

    Timer0_Config();

}

uint16_t Disetence ;
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //��Ƶ����80M
    HardwareConfig();
    LED_Color(255,0,0);
    UnlockPixhawk();

    while(1)
    {
        //UARTprintf("Hello");
        Disetence = GetAverageDistance();
        UARTprintf("Distance: %d\n",Disetence);
        Delay_ms(1000);
    }
}
