/**
  ******************************************************************************
  * 文件名程: pid.c
  * 作    者: By Sw Young
  * 版    本: V1.0
  * 功    能:
  * 编写日期: 2018.7.6
  ******************************************************************************
  * 说明：
  * 硬件平台：TM4C123G
  *   *****
  * 软件设计说明：
  *   *****
  * Github：
  ******************************************************************************
**/

#include "pid.h"
#include "timer.h"
#include "Control/Control.h"
#include "Pwm/pwm.h"
#include "MavLink_Receive/mavlink_recieve.h"
extern uint8_t Control_Open;
extern bool calculate_Flag;
extern bool Control_Serial;

PID_K PID_X;
PID_K PID_Y;
PID_K PID_H;
PID_Data PID_Data_X;
PID_Data PID_Data_Y;
PID_Data PID_Data_H;

bool start_PID_X = false;
bool start_PID_Y = false;
bool start_PID_H = false;

int err_x = 0;
int err_y = 0;
uint16_t err_h = 0;

uint8_t get_x = CAMERA_MID_X, get_y = CAMERA_MID_Y;

void PID_Init(void)
{
    get_x = CAMERA_MID_X;
    get_y = CAMERA_MID_Y;
    PID_X.Kp           = DEFAULT_KP_X;
    PID_X.Ki           = DEFAULT_KI_X;
    PID_X.Kd           = DEFAULT_KD_X;
    PID_X.dt           = (float)(1.0/DEFAULT_PID_FREQ);
    PID_X.d_LPF        = DEFAULT_LPFITER;
    PID_X.I_MAX        = DEFAULT_I_MAX;
    PID_X.OUT_MAX      = DEFAULT_OUT_MAX;

    PID_Data_X.LastError        = 0.0;
    PID_Data_X.Error            = 0.0;
    PID_Data_X.Proportion       = 0.0;
    PID_Data_X.Integrator       = 0.0;
    PID_Data_X.Last_Derivative  = 0.0;
    PID_Data_X.Derivative       = 0.0;
    PID_Data_X.PID_OUT          = 0.0;

    PID_Y.Kp           = DEFAULT_KP_Y;
    PID_Y.Ki           = DEFAULT_KI_Y;
    PID_Y.Kd           = DEFAULT_KD_Y;
    PID_Y.dt           = (float)(1.0/DEFAULT_PID_FREQ);
    PID_Y.d_LPF        = DEFAULT_LPFITER;
    PID_Y.I_MAX        = DEFAULT_I_MAX;
    PID_Y.OUT_MAX      = DEFAULT_OUT_MAX;

    PID_Data_Y.LastError       = 0.0;
    PID_Data_Y.Error           = 0.0;
    PID_Data_Y.Proportion      = 0.0;
    PID_Data_Y.Integrator      = 0.0;
    PID_Data_Y.Last_Derivative = 0.0;
    PID_Data_Y.Derivative      = 0.0;
    PID_Data_Y.PID_OUT         = 0.0;

    PID_H.Kp           = DEFAULT_KP_H;
    PID_H.Ki           = 0;
    PID_H.Kd           = 0;
    PID_H.dt           = 0;
    PID_H.d_LPF        = 0;
    PID_H.I_MAX        = 0;
    PID_H.OUT_MAX      = 0;

    PID_Data_H.LastError       = 0.0;
    PID_Data_H.Error           = 0.0;
    PID_Data_H.Proportion      = 0.0;
    PID_Data_H.Integrator      = 0.0;
    PID_Data_H.Last_Derivative = 0.0;
    PID_Data_H.Derivative      = 0.0;
    PID_Data_H.PID_OUT         = 0.0;
}

void Position_PID(void)
{
    PID_Data_X.Proportion = PID_X.Kp * PID_Data_X.Error;

    // calculate integrator constrain in i_max.
    if((PID_X.Ki != 0) && (PID_X.dt != 0))
    {
        PID_Data_X.Integrator += PID_X.Ki * PID_Data_X.Error * PID_X.dt;
        if (PID_Data_X.Integrator < -(PID_X.I_MAX))
        {
            PID_Data_X.Integrator = -(PID_X.I_MAX);
        }
        else if (PID_Data_X.Integrator > PID_X.I_MAX)
        {
            PID_Data_X.Integrator = PID_X.I_MAX;
        }
    }
    else
    {
        PID_Data_X.Integrator = 0;
    }

    // calculate instantaneous Derivative.
    if((PID_X.Kd != 0) && (PID_X.dt != 0))
    {
        PID_Data_X.Derivative = PID_X.Kd * (PID_Data_X.Error - PID_Data_X.LastError) / PID_X.dt;
        // discrete low pass filter, cuts out the
        // high frequency noise that can drive the controller crazy.
        PID_Data_X.Derivative = PID_Data_X.Last_Derivative + (PID_X.dt / ( PID_X.d_LPF + PID_X.dt)) * (PID_Data_X.Derivative - PID_Data_X.Last_Derivative);
        // update state
        PID_Data_X.LastError = PID_Data_X.Error;
        PID_Data_X.Last_Derivative = PID_Data_X.Derivative;
    }

    PID_Data_X.PID_OUT = PID_Data_X.Proportion + PID_Data_X.Integrator + PID_Data_X.Derivative;
    if (PID_Data_X.PID_OUT > PID_X.OUT_MAX)
    {
        PID_Data_X.PID_OUT = PID_X.OUT_MAX;
    }
    else if (PID_Data_X.PID_OUT < -(PID_X.OUT_MAX))
    {
        PID_Data_X.PID_OUT = -(PID_X.OUT_MAX);
    }

    PID_Data_Y.Proportion = PID_Y.Kp * PID_Data_Y.Error;

    // calculate integrator constrain in i_max.
    if((PID_Y.Ki != 0) && (PID_Y.dt != 0))
    {
        PID_Data_Y.Integrator += PID_Y.Ki * PID_Data_Y.Error * PID_Y.dt;
        if (PID_Data_Y.Integrator < -(PID_Y.I_MAX))
        {
            PID_Data_Y.Integrator = -(PID_Y.I_MAX);
        }
        else if (PID_Data_Y.Integrator > PID_Y.I_MAX)
        {
            PID_Data_Y.Integrator = PID_Y.I_MAX;
        }
    }
    else
    {
        PID_Data_Y.Integrator = 0;
    }

    // calculate instantaneous Derivative.
    if((PID_Y.Kd != 0) && (PID_Y.dt != 0))
    {
        PID_Data_Y.Derivative = PID_Y.Kd * (PID_Data_Y.Error - PID_Data_Y.LastError) / PID_Y.dt;
        // discrete low pass filter, cuts out the
        // high frequency noise that can drive the controller crazy.
        PID_Data_Y.Derivative = PID_Data_Y.Last_Derivative + (PID_Y.dt / ( PID_Y.d_LPF + PID_Y.dt)) * (PID_Data_Y.Derivative - PID_Data_Y.Last_Derivative);
        // update state
        PID_Data_Y.LastError = PID_Data_Y.Error;
        PID_Data_Y.Last_Derivative = PID_Data_Y.Derivative;
    }

    PID_Data_Y.PID_OUT = PID_Data_Y.Proportion + PID_Data_Y.Integrator + PID_Data_Y.Derivative;
    if (PID_Data_Y.PID_OUT > PID_Y.OUT_MAX)
    {
        PID_Data_Y.PID_OUT = PID_Y.OUT_MAX;
    }
    else if (PID_Data_Y.PID_OUT < -(PID_Y.OUT_MAX))
    {
        PID_Data_Y.PID_OUT = -(PID_Y.OUT_MAX);
    }
}
uint8_t t;
uint8_t PrintNump[6];
void Timer1IntHandler(void)
{
    //
    // Clear the timer interrupt.
    //
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Update the interrupt status on the display.
    //
    TimerIntDisable(TIMER1_BASE, TIMER_A);
    if(Control_Open&&Control_Serial)
    {
    /*
        * test
        */
   //        t =~ t;
   //        if(t)
   //            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, GPIO_PIN_1);
   //        else
   //            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, 0);

       calculate_test();//获取高度(放在main中)
       AttitudeProtection();
//       if(calculate_Flag)
//           UARTprintf("RealDistance:%d\n",Real_Distance);
       UARTprintf("RealDistance:%d\n",Real_Distance);
       Get_Coordinate();//获取坐标值
       UARTprintf("get_x:%d get_y:%d\n",get_x,get_y);
       err_x = (int)(Real_Distance/100 * (get_x - CAMERA_MID_X));
       err_y = (int)(Real_Distance/100 * (get_y - CAMERA_MID_Y));
       UARTprintf("Err_x:%d Err_y:%d\n",err_x,err_y);

       PID_Data_X.Error = err_x;
       PID_Data_Y.Error = err_y;
       Position_PID();
       if(start_PID_X)
           PwmControl_1((int)(channel_val_MID+(int)PID_Data_X.PID_OUT));
       if(start_PID_Y)
           PwmControl_2((int)(channel_val_MID+(int)PID_Data_Y.PID_OUT));
       UARTprintf("roll:%d\n pitch:%d\n",channel_val_MID+(int)PID_Data_X.PID_OUT,channel_val_MID+(int)PID_Data_Y.PID_OUT);
       if(start_PID_H)
       {
           if(Real_Distance < (Goal_Distance - 100))
           {
               err_h = Goal_Distance-Real_Distance;
               //PwmControl_3(1600);//油门量60%//无pid调节
               PwmControl_3((int)(channel_val_MID+(int)(PID_H.Kp*err_h)));
               UARTprintf("Throttle:%d\n",(channel_val_MID+(int)(PID_H.Kp*err_h)));
               UARTprintf("err_h:%d\n",err_h);
               UARTprintf("kp:%d\n",(int)(1000*PID_H.Kp));

           }
           else if(Real_Distance > (Goal_Distance + 100))
           {
               err_h = Real_Distance-Goal_Distance;
               //PwmControl_3(1435);//油门量39%//无pid调节
               PwmControl_3((int)(channel_val_MID-(int)(PID_H.Kp*err_h)));
               UARTprintf("Throttle:%d\n",channel_val_MID-(int)(PID_H.Kp*err_h));
               UARTprintf("err_h:%d\n",err_h);
               UARTprintf("kp:%d\n",(int)(1000*PID_H.Kp));
           }
           else if((Real_Distance-Goal_Distance<100)||(Goal_Distance-Real_Distance<100))//调节死区 -100 ~ +100
           {
               //set_ppm(0,0,channel_percent(50),0,0,0);
               PwmControl_3(channel_val_MID);
               UARTprintf("Throttle:%d\n",channel_val_MID+(int)(PID_H.Kp*0));
               UARTprintf("err_h:%d\n",err_h);
               UARTprintf("kp:%d\n",(int)(1000*PID_H.Kp));
           }
       }
    }

    TimerIntEnable(TIMER1_BASE, TIMER_A);

}
