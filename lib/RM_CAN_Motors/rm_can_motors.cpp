#include "rm_can_motors.h"
#include "can.h"
#include <Arduino.h>
int16_t txhz;
namespace canMotors
{
    static int16_t CurrentList[12]; //[0] is always empty, [1] stands for 0x201
    motorAbstract *motorList[12];
    static const char taskname[] = "motor_maneger_task";//Damn vTaskGetInfo again not compiled fk

    motorType::motorType(uint16_t a, float b) : max_mechanical_position(a), Reduction_ratio(b){};
    motorType::~motorType() {}

    motorAbstract::motorAbstract(
        uint16_t _can_id,
        motorType *motor_type,
        pid *_PID_In,
        pid *_PID_Out) : can_id(_can_id), MotorType(motor_type), PID_In(_PID_In), PID_Out(_PID_Out){};
    motorAbstract::~motorAbstract(){};
    pid* motorAbstract::getInPID(){
        return PID_In;
    }
    pid* motorAbstract::getOutPID(){
        return PID_Out;
    }

    motor::motor(uint16_t _can_id,
                 motorType *motor_type,
                 pid *_PID_In,
                 pid *_PID_Out,
                 int16_t _ZeroPostion) : motorAbstract(_can_id, motor_type, _PID_In, _PID_Out),ZeroPostion(_ZeroPostion)
    {
        registerMotor(this);
    }
    bool motorAbstract::Is_Online()
    {
        // TODO:
        return true;
    }
    void motorAbstract::SetCurrent()
    {
        CurrentList[can_id - 0x200] = TargetCurrent;
    }

    motor::~motor()
    {
    }

    void motor::Position_Run()
    {
        if (PID_Out == nullptr)
        {
            ESP_LOGE("CAN Motors", "Trying to Position_Run without PID_Out");
            TargetCurrent = 0;
            SetCurrent();
            return;
        }
        int32_t err = 0;
        err = TargetPosition - RealPosition;                                                   //得到单圈误差
        err += MotorType->max_mechanical_position * (Soft_TargetPosition - Soft_RealPosition); //加上圈数位置误差
        TargetSpeed = PID_Out->pid_run(err);                                                   //位置环得到目标速度
        TargetCurrent = PID_In->pid_run(TargetSpeed - RealSpeed);
        SetCurrent();
    }

    void motor::Angle_Set(float Target_Angle)
    {
        this->Target_Angle = Target_Angle;
        RunState = RunState_t::Position_Ctl;
        if (Target_Angle > max)
            Target_Angle = max; //对目标角度限幅
        if (Target_Angle < min)
            Target_Angle = min;
        Target_Angle = Target_Angle * MotorType->Reduction_ratio / 360.0;                             //得到电机减速前目标圈数
        TargetPosition = (Target_Angle - (int32_t)Target_Angle) * MotorType->max_mechanical_position; //小数部分 减速前单圈位置
        Soft_TargetPosition = (int32_t)Target_Angle;                                                  //整数部分 即减速前圈数
    }
    float motor::GetSoftAngle(){
        return SoftAngle;
    }
    int16_t motor::GetRealSpeed(){
        return RealSpeed;
    }
    float motor::GetTargetSoftAngle(){
        return Target_Angle;
    }
    int16_t motor::GetTargetSpeed(){
        return TargetSpeed;
    }

    void motor::Safe_Set()
    {
        RunState = RunState_t::Stop;
        TargetCurrent = 0;
        SetCurrent();
    }
    void motor::Speed_Set(int16_t Speed)
    {
        RunState = RunState_t::Speed_Ctl;
        TargetSpeed = Speed;
    }

    void motor::Speed_Run()
    {
        RunState = RunState_t::Speed_Ctl;
        TargetCurrent = PID_In->pid_run(TargetSpeed - RealSpeed);
        SetCurrent();
    }

    void motor::handle()
    {
        switch (RunState)
        {
        case RunState_t::Speed_Ctl: //电机在以速度态运行
            Speed_Run();
            break;
        case RunState_t::Position_Ctl: //电机在以速度态运行
            Position_Run();
            break;
        case RunState_t::Stop: //停止状态
            Safe_Set();
            break;
        default:
            ESP_LOGE("motor-handle", "Unknow Runstate");
            Safe_Set();
            break;
        }
    }

    void motor::update(uint8_t *data)
    {
        if (boot_flag == 0)
        {
            LastPosition = RealPosition;
            boot_flag = 1;
        }
        LastSpeed = RealSpeed;
        LastPosition = RealPosition;
        LastRealCurrent = RealCurrent; //保存转矩电流
        // Finished processing Last vars
        RealPosition = data[0] << 8 | data[1];
        RealSpeed = data[2] << 8 | data[3];
        RealCurrent = data[4] << 8 | data[5];
        Temperature = data[6];
        OriginalPosition = RealPosition; //原始数据
        //机械角的真实值需要做一个平移 以保证真正的0位置是软件0位置
        if(RealPosition-ZeroPostion<-4096)RealPosition = RealPosition-ZeroPostion+8192;
        else if(RealPosition-ZeroPostion>4096)RealPosition = RealPosition-ZeroPostion-8192;
        else RealPosition = RealPosition-ZeroPostion;
        if (RealCurrent != LastRealCurrent)                                    //前后转矩电流不同才算作有效数据
            LastUpdateTime = millis();                                         //更新本次电机数据最后更新的时间
        RealAngle = RealPosition * 360.f / MotorType->max_mechanical_position; //根据机械角计算出的真实角度

        if (RealPosition - LastPosition > 4096) //圈数累计
            Soft_RealPosition--;
        else if (RealPosition - LastPosition < -4096)                                                                                                                                 //圈数累计
            Soft_RealPosition++;                                                                                                                                                      //根据机械角计算出的真实角度
        SoftAngle = /*圈数对应角度*/ (Soft_RealPosition) / (1.0 * MotorType->Reduction_ratio) * 360 + /*单圈内角度*/ 1.0f * RealPosition / (8192 * MotorType->Reduction_ratio) * 360; //转换为角度
    }

    void update(can_message_t &msg)
    {
        if (msg.identifier <= 0x20B && msg.identifier >= 0x201) // TODO:&&消息长度是规定值，防止数组越界 是标准的大疆数据帧
        {
            for (motorAbstract *i : motorList)
            {
                if (i != nullptr)
                {
                    i->update(msg.data);
                }
            }
        }
    }

    static esp_err_t send()
    { // Send CurrentList to can bus
        can_message_t tx_msg = {.flags = CAN_MSG_FLAG_NONE,
                                .identifier = 0x200,
                                .data_length_code = 8};
        tx_msg.data[0] = CurrentList[1] >> 8;
        tx_msg.data[1] = CurrentList[1];
        tx_msg.data[2] = CurrentList[2] >> 8;
        tx_msg.data[3] = CurrentList[2];
        tx_msg.data[4] = CurrentList[3] >> 8;
        tx_msg.data[5] = CurrentList[3];
        tx_msg.data[6] = CurrentList[4] >> 8;
        tx_msg.data[7] = CurrentList[4];
        return can_transmit(&tx_msg, portMAX_DELAY);
        // TODO add send for more than 0x201-0x204
    }

    void motor_manager_task(void *n)
    {
        log_i("Manager Task Start Running");
        TickType_t xLastWakeTime;
        const TickType_t xFrequency = 1;
        // xLastWakeTime = xTaskGetTickCount();
        // static const TickType_t MaxInterval = 5;
        // unsigned long entryTime;
        // unsigned long maxExecTime = 1000; //in us
        // int entryCoreID;
        static uint32_t lasttime = 0;
        static int16_t count = 0;
        lasttime = millis();
        
        while (1)
        {
            // debug -----
            count++;
            if (millis() - lasttime > 1000)
            {
                txhz = (count * 1000.0) / ((float)(millis() - lasttime));
                lasttime = millis();
                count = 0;
            }
            // debug -----
            // entryCoreID = xPortGetCoreID();
            // entryTime = micros();
            // if (xTaskGetTickCount() - xLastWakeTime > MaxInterval){
            //     log_w("Task %s run interval longer than expectation:%d",taskname,xTaskGetTickCount() - xLastWakeTime);
            // }
            // xLastWakeTime = xTaskGetTickCount();
            // tskTestInit(true);
            // tskTestBegin(true);
            for (motorAbstract *i : motorList)
            {
                if (i == nullptr)
                    continue;
                if (i->Is_Online())
                    i->handle();
            }
            send();
            // tskTestBegin(true);
            // if (tskTestResult(true)!=0){
            //     log_e("UnExpected Scheduling happend!!!");
            // }
            // if (entryCoreID!=xPortGetCoreID()){
            //     log_w("Core Change on %s EntryCore:%d NowCore:%d",taskname,entryCoreID,xPortGetCoreID());
            // }
            // if (micros() - entryTime>maxExecTime){
            //     log_w("Task %s running too slow %luus. EntryCore:%d NowCore:%d",taskname,micros() - entryTime,entryCoreID,xPortGetCoreID());
            // }
            vTaskDelayUntil(&xLastWakeTime, xFrequency);
        }
    }

    static TaskHandle_t motor_manager_handle;
    esp_err_t manager_init()
    {
        if (pdPASS == xTaskCreatePinnedToCore(motor_manager_task, taskname, 4096, NULL, 14, &motor_manager_handle, tskNO_AFFINITY))
        {
            return ESP_OK;
        }

        return ESP_FAIL;
    }
    esp_err_t registerMotor(motorAbstract *motor)
    {
        if ((motor->can_id) > 0x200 && (motor->can_id) < 0x20C)
            motorList[motor->can_id - 0x200] = motor;
        else
        {
            ESP_LOGE("CAN Motors", "motor id register error %x", motor->can_id);
            return ESP_ERR_INVALID_ARG;
        }
        return ESP_OK;
    }

    esp_err_t update(uint32_t id, uint8_t *data)
    {
        if ((id) > 0x200 && (id) < 0x20C)
        {
            if (motorList[id - 0x200] != nullptr)
            {
                motorList[id - 0x200]->update(data);
                return ESP_OK;
            }
            return ESP_ERR_INVALID_STATE;
        }
        return ESP_ERR_INVALID_ARG;
    }

}
