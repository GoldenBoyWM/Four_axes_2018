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
void Timer0IntHandler(void)
{
    uint32_t ui32IntStatus;
    ui32IntStatus = TimerIntStatus(TIMER0_BASE, true);
    TimerIntClear(TIMER0_BASE, ui32IntStatus);//����жϱ�־λ
    if((GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0) & GPIO_PIN_0)  != GPIO_PIN_0)
    {
    KeyPress0=(1+KeyPress0)%2;
    }

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

        TimerLoadSet(TIMER1_BASE, TIMER_A,  SysCtlClockGet() ); //5HZ

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
void Timer1IntHandler(void)
{
//    //
//    // Clear the timer interrupt.
//    //
//    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
//
//    //
//    // Update the interrupt status on the display.
//    //
//    if(MotorOrderDirection==0||MotorOrderDirection==1)
//    {
//        UARTprintf("Dis%d",(Counter*20)/6400);
//    }
//    else if (MotorOrderDirection==2||MotorOrderDirection==3)
//    {
//        UARTprintf("Ang%d",(int)(Counter/89.3));
//    }
}
