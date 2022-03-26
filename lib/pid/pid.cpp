#include "pid.h"
#include <Arduino.h>
#include <cstdlib>
////*******************************************传统PID类*************************************************************************////
/**
 * @brief  传统pid构造函数
 * @param [in]   P 比例系数
 * @param [in]	 I 积分系数
 * @param [in]	 D 微分系数
 * @param [in]	 IMax 积分限幅
 * @param [in]	 PIDMax 输出限幅
 * @param [in]	 I_Time 积分时间
 *	@param [in]	 D_Time 积分时间
 * @param [in]	 I_Limited_Down 进行积分调节的误差区间限制
 * @par 日志
 *
 */
pid::pid(float P, float I, float D, float IMax, float PIDMax, uint16_t I_Time, uint16_t D_Time, int16_t I_Limited_Down, int16_t I_Limited_Up) : P(P), I(I), D(D),
                                                                                                                      IMax(IMax), PIDMax(PIDMax), I_Time(I_Time), D_Time(D_Time), I_Limited_Down(I_Limited_Down), I_Limited_Up(I_Limited_Up) {
    this->I_Limited_Down = I_Limited_Down > 0 ? this->I_Limited_Down : 0;   //确保 I_Limited_Down 不小于0
    if (this->I_Limited_Up < 0)
        return;
    if (this->I_Limited_Up < this->I_Limited_Down){ //swap up and down if they are wrong
        int16_t tmp = this->I_Limited_Up;
        this->I_Limited_Up = this->I_Limited_Down;
        this->I_Limited_Down = tmp;
    }


}

pid::~pid()
{
}

/**
 * @brief  pid运行函数
 * @param [in]   err 传入pid环的误差
 * @retval  pid的运行输出
 * @par 日志
 *				2019年12月1日15:00:00 移除积分时间不设置的兼容性改动，加入微分时间
 */
float pid::pid_run(float err)
{
    CurrentError = err;
    Pout = CurrentError * P;

    //积分分离
    if (millis() - I_start_time >= I_Time) //如果达到了时间区间的话则进行积分输出
    {
        if (fabs(CurrentError) > I_Limited_Down){
            if ((I_Limited_Up < 0) || (fabs(CurrentError) < I_Limited_Up)){
                //满足积分条件
                Iout += I * CurrentError;
            }else{
                Iout = 0;
            }
        }
        else{
            Iout = 0;                   //误差区间外边积分清0
        }
            
        I_start_time = millis(); //重新定义积分开始时间
    }

    if (Custom_Diff != nullptr) //存在自定义微分数据
        Dout_Accumulative = (*Custom_Diff) * D;
    else
        Dout_Accumulative = (CurrentError - LastError) * D;

    if (millis() - D_start_time > D_Time) //如果达到了时间区间的话则进行微分输出
    {
        Dout = Dout_Accumulative;
        Dout_Accumulative = 0;
        D_start_time = millis(); //重新定义微分开始时间
    }

    if (Iout >= IMax)
        Iout = IMax;
    if ((Iout) <= -(IMax))
        Iout = -(IMax); //积分限幅

    PIDout = Pout + Iout + Dout; // Pid输出计算
    if (PIDout >= PIDMax)
        PIDout = PIDMax;
    if (PIDout <= -(PIDMax))
        PIDout = -(PIDMax); //输出限幅

    LastError = CurrentError;
    return PIDout;
}

void pid::clearError()
{
    CurrentError = 0;
    LastError = 0;
    Iout = 0;
    Dout = 0;
    Dout_Accumulative = 0;
    return;
}

int pid::setArgs(PIDArgType type, float value)
{
    bool changed = true;
    switch (type)
    {
    case PIDArgType::kP:
        P = value;
        break;

    case PIDArgType::kI:
        I = value;
        break;

    case PIDArgType::kD:
        D = value;
        break;

    default:
    changed = false;
        break;
    }
    if (changed){
        notify();
    }
    return 0;
}

float pid::getArgs(PIDArgType type){
    switch (type)
    {
    case PIDArgType::kP:
        return P;

    case PIDArgType::kI:
        return I;

    case PIDArgType::kD:
        return D;

    default:
    return 0;
    } 
}
