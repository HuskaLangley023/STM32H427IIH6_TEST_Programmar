#include"M3508_Motor.h";

M3508_Motor::M3508_Motor():ratio_(19),angle_(0),delta_angle_(0),ecd_angle_(0),last_ecd_angle_(0),delta_ecd_angle_(0),rotate_speed_(0),current_(0),temp_(0)
{}

float M3508_Motor::linearMapping(int in, int in_min, int in_max, float out_min, float out_max) {
    float out,k,b;
    k=(out_max-out_min)/(in_max-in_min);
    b=out_max-k*in_max;
    out=k*in+b;
    return out;
}

//Task3
void M3508_Motor::canRxMsgCallback_v1(uint8_t rx_data[8]) {
    ecd_angle_=linearMapping(rx_data[0]<<8|rx_data[1],0,8191,0,360);
    rotate_speed_=rx_data[2]<<8|rx_data[3];
    current_=linearMapping(rx_data[4]<<8|rx_data[5],-16384,16384,-20,20);
    temp_=rx_data[6];

    canRxMsgCallback_v1(rx_data);
    delta_ecd_angle_=ecd_angle_-last_ecd_angle_;
    if(delta_ecd_angle_>180) delta_ecd_angle_-=360;
    else if(delta_ecd_angle_<-180) delta_ecd_angle_+=360;
    last_ecd_angle_=ecd_angle_;

    canRxMsgCallback_v2(rx_data);
    delta_angle_=delta_ecd_angle_/ratio_;
    angle_+=delta_angle_;
}


//Task4
void M3508_Motor::canRxMsgCallback_v2(uint8_t rx_data[8]) {
    canRxMsgCallback_v1(rx_data);
    delta_ecd_angle_=ecd_angle_-last_ecd_angle_;
    if(delta_ecd_angle_>180) delta_ecd_angle_-=360;
    else if(delta_ecd_angle_<-180) delta_ecd_angle_+=360;
    last_ecd_angle_=ecd_angle_;
}

//Task5
void M3508_Motor::canRxMsgCallback_v3(uint8_t rx_data[8]) {
    canRxMsgCallback_v2(rx_data);
    delta_angle_=delta_ecd_angle_/ratio_;
    angle_+=delta_angle_;
}


//下为Task6
//第一个函数用于每一次收发报文后，计算角度的总误差累计值；第二个函数为修改后结果）

//Function1 用未映射的uint16_t型来计算间接计算负载累计转过的角度，只对首尾两时刻的总转角进行线性映射，从而消除每次旋转时float型带来的误差
void M3508_Motor::cal_error_time(uint8_t rx_data[8]){
    canRxMsgCallback_v3(rx_data);
    uint16_ecd_angle_=rx_data[0]<<8|rx_data[1];
    uint16_delta_ecd_angle_=uint16_last_ecd_angle_-uint16_ecd_angle_;
    uint16_last_ecd_angle_=uint16_ecd_angle_;//以上三行是该次报文读取后，对未映射前的转子角度数据进行更新
    abs_sum_delta_angle_+=abs(delta_angle_);  //累加值，反映负载总转过角度
    uint16_abs_sum_delta_ecd_angle_+=abs(uint16_delta_ecd_angle_);  //累加值，反映未映射前的转子总转过角度
    error_angle=linearMapping(uint16_abs_sum_delta_ecd_angle_,0,8191,0,360)/19-abs_sum_delta_angle_;
    //将未映射前的转子总转过角度进行线性映射，并考虑减速比，得到更精准的负载总转过度数，将之与float型总度数相减，得误差值
    error_time=error_angle*60/(482*360);  //M3508空载最高转速482rpm
    if(error_angle>=10){cerr<<"角度误差累积超过10度，所用时间："<<error_time<<endl;}
}//若角度误差超过10度，程序紧急终止，且返回所花时间

void M3508_Motor::canRxMsgCallback_v4(uint8_t rx_data[8]) {
    rotate_speed_=rx_data[2]<<8|rx_data[3];
    current_=linearMapping(rx_data[4]<<8|rx_data[5],-16384,16384,-20,20);
    temp_=rx_data[6];
    uint16_ecd_angle_=rx_data[0]<<8|rx_data[1];
    uint16_delta_ecd_angle_=uint16_last_ecd_angle_-uint16_ecd_angle_;
    uint16_last_ecd_angle_=uint16_ecd_angle_;//以上三行是该次报文读取后，对未映射前的转子角度数据进行更新
    uint16_sum_ecd_angle_+=uint16_delta_ecd_angle_;//对uint8总转动量更新，此值映射后除以19即对象变量中的angle_成员
    angle_=linearMapping(uint16_sum_ecd_angle_,0,8191,0,360)/19;
}


