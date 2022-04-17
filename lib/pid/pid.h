#pragma once
#include <stdint.h>
#include <observerPatten.h>

enum class PIDArgType{kP, kI, kD};
class pid:public Subject
{
// private:
public:
    /* data */
    float *Custom_Diff = nullptr; //!<自定义的外界浮点型微分数值 常用于路程环的微分环节(即速度值)、陀螺仪反馈值效果一般比编码器好
    float P;
    float I;
    float D;
    float IMax;
    float PIDMax;
    uint16_t I_Time;              
    uint16_t D_Time;              
    int16_t I_Limited_Down;           
    int16_t I_Limited_Up;           //!<当误差满足 I_Limited_Up>fabs(error)>I_Limited_Down 时才进行I积分. -1 代表无上限
    float Pout;
    float Iout;
    float Dout;
    float Dout_Accumulative; //因为有微分时间重定义,因此加入一个变量真正用于输出D,源Dout用作累积误差
    float PIDout;
    float CurrentError;
    float LastError;
    uint32_t I_start_time; //!<积分开始时间戳，用于带时间参数的pid   plus专属
    uint32_t D_start_time; //!<微分开始时间戳，用于带时间参数的pid
// public: 
    pid(float P, float I, float D, float IMax, float PIDMax, uint16_t I_Time = 1, uint16_t D_Time = 1, int16_t I_Limited_Down = 0, int16_t I_Limited_Up = -1); //传统pid构造函数
    ~pid();
    float pid_run(float err);
    void clearError();
    int setArgs(PIDArgType type, float value);
    float getArgs(PIDArgType type);
};


