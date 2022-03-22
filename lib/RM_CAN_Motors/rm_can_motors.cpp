#include "rm_can_motors.h"
#include "can.h"
#include <Arduino.h>

namespace canMotors
{
    std::vector<motor *> motorList;
    
    motorType::motorType(uint16_t a, float b) : max_mechanical_position(a), Reduction_ratio(b){};

    motor::motor(uint8_t can_num,
                 uint16_t _can_id,
                 motorType *motor_type,
                 pid *_PID_In,
                 pid *_PID_Out)
    {
    }

    motor::~motor()
    {
    }

    void motor::handle()
    {
    }

    void motor::update(uint8_t *data)
    {
        LastSpeed = RealSpeed;
        LastPosition = RealPosition;
        LastRealCurrent = RealCurrent; //保存转矩电流
        RealPosition = data[0] << 8 | data[1];
        RealSpeed = data[2] << 8 | data[3];
        RealCurrent = data[4] << 8 | data[5];
        Temperature = data[6];
        if (RealCurrent != LastRealCurrent)                                    //前后转矩电流不同才算作有效数据
            LastUpdateTime = millis();                                         //更新本次电机数据最后更新的时间
        RealAngle = RealPosition * 360.f / MotorType->max_mechanical_position; //根据机械角计算出的真实角度
    }

    void softmotor::update(uint8_t *data)
    {
        motor::update(data); //调用父类motor的通用update函数
        if (boot_flag == 0)
        {
            LastPosition = RealPosition;
            boot_flag = 1;
        }
        if (RealPosition - LastPosition > 4096) //圈数累计
            Soft_RealPosition--;
        else if (RealPosition - LastPosition < -4096) //圈数累计
            Soft_RealPosition++;
        RealAngle = RealPosition * 360.f / MotorType->max_mechanical_position;                                                                                                        //根据机械角计算出的真实角度
        SoftAngle = /*圈数对应角度*/ (Soft_RealPosition) / (1.0 * MotorType->Reduction_ratio) * 360 + /*单圈内角度*/ 1.0f * RealPosition / (8192 * MotorType->Reduction_ratio) * 360; //转换为角度
    }

    void update(can_message_t& msg){
        if(msg.identifier<=0x20B && msg.identifier>=0x201)//&&消息长度是规定值，防止数组越界 是标准的大疆数据帧
        {
            if (!motorList.empty())
            {
                std::vector<motor *>::iterator it;
                for (it = motorList.begin(); it != motorList.end(); it++){
                    (*it)->update(msg.data);
                }
            }
        }
    }

}
