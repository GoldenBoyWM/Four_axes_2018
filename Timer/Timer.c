/**
  ******************************************************************************
  * �ļ�����: Timer.c
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
#include "timer.h"


char Time_Flag = 0;
uint32_t Counter = 0;
uint8_t Beep_Flag = 0;
uint32_t Beep_Counter = 0;
uint32_t Beep_Fre = 40;

/**
  * �� �� ��:MotorContolTimer.c
  * ��������: �����ʱ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��:
  *   By Sw Young
  *   2018.03.29
  */
void Timer0_Config(void)
{
       //
       // Enable the peripherals used by this example.
       //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
       //
       // Enable processor interrupts.
       //
       //����Ƶ
       //����Ϊ���ϼ���ģʽ
        TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
       //
       //�Ѽ���ֵװ��
       TimerLoadSet(TIMER0_BASE, TIMER_A,0xFFFFFFFF);//2
       //
       //***************************�ж�ʹ��*******************************************
       //Ĭ�ϲ��ж�
       //ʹ��TIME0B�ڻ�������ʱ�ж�
       //TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
       //ʹ��TIMER0B�ж�
       //IntEnable(INT_TIMER0A);
       IntDisable(INT_TIMER0A);//2
       //��̬ע��
       //TimerIntRegister(ui32Base, ui32Timer, pfnHandler)
       //ʹ��TIMER0B,��ʼ����
       //TimerEnable(TIMER0_BASE, TIMER_B);
       TimerEnable(TIMER0_BASE, TIMER_A);//2
       //ʹ�ܴ������ж�
       IntMasterEnable();
}
/**
  * �� �� ��:Timer0IntHandler.c
  * ��������: �����ʱ���ж�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��:
  *   By Sw Young
  *   2018.03.29
  */
uint8_t k=0;
void Timer0IntHandler(void)
{
    uint32_t ui32IntStatus;
    ui32IntStatus = TimerIntStatus(TIMER0_BASE, true);
    TimerIntClear(TIMER0_BASE, ui32IntStatus);//����жϱ�־λ
    if((GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0) & GPIO_PIN_0)  != GPIO_PIN_0)
    {
    KeyPress0=(1+KeyPress0)%2;
    }

    if(k)
        {k=0;GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, GPIO_PIN_2);}
    else
        {k=1;GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, 0);}
}
void Timer1_Config(void)
{
       //
       // Enable the peripherals used by this example.
       //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
       //
       // Enable processor interrupts.
       //
        IntMasterEnable();

       //
       // Configure the two 32-bit periodic timers.
       //
        TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

        TimerLoadSet(TIMER1_BASE, TIMER_A,  800000); //Fre = ��Ƶ/800000 = 100HZ

       //
       // Setup the interrupts for the timer timeouts.
       //
        IntEnable(INT_TIMER1A);

        TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
       //
       // Enable the timers.
       //
        TimerEnable(TIMER1_BASE, TIMER_A);
}
/**
  * �� �� ��:Timer1IntHandler.c
  * ��������: ���ڲ������Ͷ�ʱ���ж�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��:
  *   By Sw Young
  *   2018.03.29
  */
/*
 * ��ʱ��1���жϷ�������PID.C��
 */
//uint8_t t=0;
//void Timer1IntHandler(void)
//{
//    //
//    // Clear the timer interrupt.
//    //
//    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
//
//    //
//    // Update the interrupt status on the display.
//    //
///*
// * test
// */
////    t =~ t;
////    if(t)
////        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, GPIO_PIN_1);
////    else
////        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, 0);
//
//    err_x = (int)(sonar_height * (get_x - CAMERA_MID_X));
//    err_y = (int)(sonar_height * (get_y - CAMERA_MID_Y));
//    PID_Data_X.Error = err_x;
//    PID_Data_Y.Error = err_y;
//    Position_PID();
//    if(start_PID_X)
//        set_ppm((channel_val_MID - 5 + PID_Data_X.PID_OUT),0,0,0,0,0);
//    if(start_PID_Y)
//        set_ppm(0,(channel_val_MID + PID_Data_Y.PID_OUT),0,0,0,0);
//    if(start_PID_H)
//    {
//        if(sonar_height * 100  < (dest_height - 10))
//        {
//            set_ppm(0,0,channel_percent(61),0,0,0);
//        }
//        if(sonar_height * 100  > dest_height)
//        {
//            set_ppm(0,0,channel_percent(50),0,0,0);
//        }
//        if(sonar_height * 100  > (dest_height + 5))
//        {
//            set_ppm(0,0,channel_percent(39),0,0,0);
//        }
//    }
//}
