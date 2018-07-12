/*******************************************
��������:CCSv7.0
������:TIVA C Launchpad(TM4C123GH6PM)
������:�������������
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
  * ��д����: 2018.7.6
  ******************************************************************************
  * ˵����
  * Ӳ��ƽ̨��
  *   MCUc:TM4C123��Pixhawk�����ߴ��ڡ���ˢ���
  * ������˵����
  *     ��ߡ���ȡͼ�������ڶ�ʱ������ɣ���߰���������ʱ�����ܣ��ұ߰����رն�ʱ�����ܡ�
  * Github��
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
#include "key/key.h"


void HardwareConfig(void)
{
    Uart0Iint();        //����0��ʼ��
    Uart1Iint();        //����1��ʼ��
    Uart2Iint();        //����2��ʼ��

    PwmConfig();            //��ʼ��PWM

    LED_Config();    //LED��ʼ��
    LED_Set(BLUE);

    OLED_Init();            //��ʼ��OLED
    OLED_Clear();
    Delay_ms(5);            //��ʱ�ȴ�OLED��ʼ��
    OLED_DrawBMP(0,0,128,8,BMP3);

    Sonar_Configure();      //��������ʼ��
    Sonar_GPIOA_Configure();
    Sonar_GPIOA_Interrupt();

    Timer0_Config();    //��ʱ����ʼ��
    Timer1_Config();

    Key_Configure();    //������ʼ��
    Key_Interrupt();    //�����ж�
}

extern uint16_t Real_XCoordinate,Real_YCoordinate;//��������
extern uint16_t Real_Distance ;//�����߶�
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //��Ƶ����80M
    HardwareConfig();//Ӳ����ʼ��
    Delay_ms(10);
    LED_Set(RED);
    UnlockPixhawk();
    LED_Set(GREEN);
    while(1)
    {
       //UARTprintf("Hello");//������
       // Real_Distance = GetAverageDistance();//��ȡ�߶��ڶ�ʱ����
        UARTprintf("Distance: %d\n",Real_Distance);
        UARTprintf("x=%d,y=%d\n",Real_XCoordinate,Real_YCoordinate);
        Delay_ms(1000);
    }
}
