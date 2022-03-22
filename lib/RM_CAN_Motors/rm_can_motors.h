#pragma once
#include <cstdint>
#include <pid.h>
#include "driver/can.h"
#include <vector>
namespace canMotors
{
    enum class RunState
    {
        Stop = 0x00U,         //!<电机停止工作
        Speed_Ctl = 0x01U,    //!<电机使用速度环工作
        Position_Ctl = 0x02U, //!<电机使用路程环工作
        ///在子类需要拓展运行模式时需要在这里加
    };

    class motorType //设定某种类型，如3508,6020.设定他们的减速比之类的设置
    {
    public:
        uint16_t max_mechanical_position = 8192;//!<转一圈的编码器值,默认0x2000
        float Reduction_ratio = 19.203;
        motorType(uint16_t a,float b);
        motorType(/* args */);
        ~motorType();
    };

    class motorAbstract
    {
    public:
        motorAbstract();
        ~motorAbstract();
        virtual void handle() = 0;
        virtual void update(uint8_t *data) = 0;
        virtual bool Is_Online();

    protected:
        int16_t RealCurrent;     //编码器反馈转矩电流
        int16_t LastRealCurrent; //编码器反馈转矩电流
        float RealAngle;         //!<根据机械角计算出的真实角度
        int16_t TargetCurrent;   //!<目标电流值
        int16_t RealPosition;    //!<真实位置(编码器)
        int16_t TargetPosition;  //!<目标位置
        int16_t RealSpeed;       //!<实际速度(编码器)
        int16_t TargetSpeed;     //!<目标速度
        int8_t Temperature;       //编码器反馈转矩电流
        motorType *MotorType;    //!<电机(电调)参数
        pid *PID_In;	  //!< PID内环
        pid *PID_Out;	  //!< PID外环
        int16_t LastPosition; //!<上次位置
        int16_t LastSpeed;	  //!<上次速度
        uint32_t LastUpdateTime;
    };

    class motor : public motorAbstract //电机基础实例
    {
    protected:
        virtual void Speed_Run(void);
    private:
        /* data */
    public:
        virtual void handle();
        virtual void update(uint8_t *data);
        motor(uint8_t can_num,
              uint16_t _can_id,
              motorType *motor_type,
              pid *_PID_In,
              pid *_PID_Out = nullptr);
        ~motor();
    };

    class softmotor : public motor
    { //软路程电机
    protected:
        int32_t Soft_RealPosition=0;//!<软真实路程，这里实际意义是轮子转过的圈数
		int32_t Soft_TargetPosition;//!<软目标路程，实际意义为轮子要转的圈数
        float SoftAngle; //!<软角度，根据转过圈数来进行计算 RealAngle现在是单圈内角度了
    private:
        /* data */
        bool boot_flag = 0; //0 代表刚开机, 用来保证第一次的软路程不突变 因为开机时LastPosition肯定是0可能会导致一圈外环
    public:
        virtual void handle();
        virtual void update(uint8_t *data);
        softmotor(/* args */);
        ~softmotor();
    };


    int registerMotor();
    extern std::vector<motor *> motorList;
    void update(can_message_t& msg);

}