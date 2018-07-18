/**
  ******************************************************************************
  * �ļ�����: main.c
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
#include "math.h"

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
#include "delay/delay.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"
#include "uart/uart.h"
#include "uart/uartstdio.h"
#include "Beep/Beep.h"
#include "head.h"
#include "Control/Control.h"
#include "sonar.h"
#include "key/key.h"
#include "MavLink_Receive/mavlink_recieve.h"
#include "0.96'OLED/OLED.h"

/**
  * �� �� ��:HardwareConfig
  * ��������: Ӳ����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  *   By Sw Young
  *   2017.7.6
  */
void HardwareConfig(void)
{
    Uart0Iint();        //����0��ʼ��
    Uart1Iint();        //����1��ʼ��
    UART3Iint();        //����3��ʼ��

    PwmConfig();            //��ʼ��PWM

    LED_Config();    //LED��ʼ��
    LED_Set(BLUE);

    OLED_Init();            //��ʼ��OLED
    OLED_Clear();
    Delay_ms(5);            //��ʱ�ȴ�OLED��ʼ��
/*
 * ����
 */
    Sonar_Configure();      //��������ʼ��
    Sonar_GPIOA_Configure();
    Sonar_GPIOA_Interrupt();

    //Timer0_Config();    //��ʱ����ʼ��
    Timer1_Config();
    Timer2_Config();


    Key_Configure();    //������ʼ��
    Key_Interrupt();    //�����ж�

    Mavlink_DateInit();
    IntMasterEnable();
    PID_Init();

}

extern uint16_t Real_XCoordinate,Real_YCoordinate;//��������
extern uint16_t Goal_Distance;
extern float volatile Real_Distance;
extern int16_t  RealAttitude_roll,\
                RealAttitude_pitch,\
                RealAttitude_yaw;//��������
extern uint16_t err_roll,err_pitch;

extern uint8_t Control_Open;
extern bool start_PID_X;
extern bool start_PID_Y;
extern bool start_PID_H;
/**
  * �� �� ��:Main
  * ��������: ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  *   By Sw Young
  *   2017.7.6
  */
int main(void)

{
    bool Control_Open_Flag = true;//ϵͳ���Ʊ�־λ��ͨ�������򿪣�һ����Ч

    FPUEnable();        //������������
    FPULazyStackingEnable();

    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //��Ƶ����80M
    HardwareConfig();   //Ӳ����ʼ��
    Delay_ms(10);       //��ʱ�ȴ�Ӳ����ʼ��
    LED_Set(RED);       //����LEDָʾ��
    OledDisplayInit();  //��ʼ��OLED��ʾ����
    while(1)
    {
        if(Control_Open&&Control_Open_Flag)
        {
            Control_Open_Flag = false;
            UnlockPixhawk();
            start_PID_H = true;
            LED_Set(GREEN);
        }
//        Get_Distance();//��ȡ�߶�
//        Get_Attitude();//��ȡ��̬
        Display();//��ʾ����
        UARTprintf("RealDistance: %d\n",(int)Real_Distance);
        UARTprintf("GoalDistance: %d\n",Goal_Distance);
        UARTprintf("x=%d,y=%d\n",Real_XCoordinate,Real_YCoordinate);
        UARTprintf("Roll: %d;pitch: %d;yaw:%d\n",RealAttitude_roll,RealAttitude_pitch,RealAttitude_yaw);
        UARTprintf("err_roll:%d;err_pitch%d;\n",err_roll=fabs(RealAttitude_roll),err_pitch=fabs(RealAttitude_pitch));
        Delay_ms(500);
    }
}
