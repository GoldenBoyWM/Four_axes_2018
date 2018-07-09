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

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    uint8_t t;
    OLED_Init();            //��ʼ��OLED
    OLED_Clear();

    t=' ';
    OLED_ShowCHinese(12,0,0);//��
    OLED_ShowCHinese(30,0,1);//ʾ
    OLED_ShowCHinese(48,0,2);//��
    OLED_ShowCHinese(66,0,3);//��
    OLED_ShowCHinese(84,0,4);//ʵ
    OLED_ShowCHinese(102,0,5);//��
    while(1)
    {
        OLED_Clear();
        OLED_ShowCHinese(12,0,0);//��
        OLED_ShowCHinese(30,0,1);//ʾ
        OLED_ShowCHinese(48,0,2);//��
        OLED_ShowCHinese(66,0,3);//��
        OLED_ShowCHinese(84,0,4);//ʵ
        OLED_ShowCHinese(102,0,5);//��

        OLED_ShowString(6,3,"0.96' OLED TEST",16);
        //OLED_ShowString(8,2,"ZHONGJINGYUAN");
        //OLED_ShowString(20,4,"2014/05/01");
        OLED_ShowString(0,6,"ASCII:",16);
        OLED_ShowString(63,6,"CODE:",16);
        OLED_ShowChar(48,6,t,16);       //��ʾASCII�ַ�
        t++;
        if(t>'~')t=' ';
        OLED_ShowNum(103,6,t,3,16);     //��ʾASCII�ַ�����ֵ

        Delay_ms(1500);
        OLED_DrawBMP(0,0,128,8,BMP1);   //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
        Delay_ms(1500);
        OLED_DrawBMP(0,0,128,8,BMP2);
        Delay_ms(1500);
        OLED_DrawBMP(0,0,128,8,BMP3);
        Delay_ms(1500);

    }
}
