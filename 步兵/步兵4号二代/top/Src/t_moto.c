#include "t_moto.h"
#include "m_moto.h"
#include "m_imu.h"
#include "pid.h"
#include "m_remote.h"
#include "t_remote.h"
#include "t_tk1.h"
#include "t_protocol.h"
#include "DJ_Protocol.h"
extern short int Tk1pointang[3];
extern float angle_set;//������
extern float var[7];//������
float Cm3508_spd_get[4]= {0,0,0,0};
float Cm3508_spd_xset[4];//x��������ٶ�
float Cm3508_spd_yset[4];//y��������ٶ�
float Cm3505_spd_sqin[4];//�����˶�
float Cm3508_spd_set[4];//�ϳ��ٶ�
float Cm3508_total_set[4];//��pid���=�����˶���pid���+�涯pid���

float Gm6623_pit_ang_set=pit6623_MiddleAngle;
float Gm6623_pit_spd_set=0;
float Gm6623_pit_ang_offset=25;//�˷�����Ӱ���ƫ��ǣ���еһ������Ҫ���½���

float Gm6623_yaw_ang_set=yaw6623_MiddleAngle;
float Gm6623_yaw_spd_set=0;
float Gm6623_yaw_ang_give=yaw6623_MiddleAngle;

//�涯����
Follow_6623_imu_fbangall_t follow_6623yaw_imu_data;
int yaw6623_AngleAdjust;
int yawimu_AngleAdjust;
int pitimu_AngleAdjust;

//�Զ������
Follow_6623_imu_fbangall_t autu_6623yaw_imu_data;
float autu_Gm6623_yaw_ang_set=0;
int autu_yaw6623_AngleAdjust;
int autu_yawimu_AngleAdjust;
float autu_Gm6623_pit_ang_set=0;
int autu_pit6623_AngleAdjust;
int autu_pitimu_AngleAdjust;
//�޷�����
int err;
int offset = yaw6623_MiddleAngle;
//�������
double km_nor_yaw_temang=0;
double km_nor_yaw_ang_fb=0;
double km_nor_yaw_spd_fb;
double km_nor_yaw_ang_set=yaw6623_MiddleAngle;
int32_t yaw_Nowangle = yaw6623_MiddleAngle;

double km_nor_pit_temang=0;
double km_nor_pit_ang_fb=0;
double km_nor_pit_spd_fb;
double km_nor_pit_ang_set=pit6623_MiddleAngle;
int32_t pit_Nowangle = pit6623_MiddleAngle;

double send_current_yaw;
double send_current_pit;

float a=0.02;
void cm_spd_current_ctr(pid_t *pid,float fbspeed, float setspeed)
{
    setspeed = a*setspeed + (1-a)*pid->spset;
    pid->spset = setspeed;
    pid_calc(pid, fbspeed, setspeed);
}
void cm_spd_ctr(void)
{
    for(int i=0; i<4; i++)
        pid_calc(&Cm3508_spd_pid[i], moto_chassis[i].speed_rpm, Cm3508_total_set[i]);

//	if(ReadData.cm_power<Limit_Power)
//	{
//		for(int i=0; i<4; i++)
//			pid_calc(&Cm3508_spd_pid[i], moto_chassis[i].speed_rpm, Cm3508_total_set[i]);
//	}
//	else if(ReadData.cm_power<Max_Power)
//	{
//		a = 0.03;
//		for(int i=0; i<4; i++)
//			cm_spd_current_ctr(&Cm3508_spd_pid[i], moto_chassis[i].speed_rpm, Cm3508_total_set[i]);
//	}
//	else
//	{
//		a = 0.01;
//		for(int i=0; i<4; i++)
//			cm_spd_current_ctr(&Cm3508_spd_pid[i], moto_chassis[i].speed_rpm, Cm3508_total_set[i]);
//	}
}

/**************************************/
//�Զ�pitch
/**************************************/
/*
void km_au_pit_ctr(void)//���λ��ʽ�Զ����
{
    float C=0.35;
    int Nowangle;
    static float angle_pidout_temp=0;
    static double pit_auto_angle_set=pit6623_MiddleAngle;//��Ӧ�е�
    float now_Tk1pointang=0;
    static float last_Tk1pointang=0;
    now_Tk1pointang=Tk1pointang[1];
    if(Tk1pointang[2]==0)
    {
        now_Tk1pointang=last_Tk1pointang;
    }
    Nowangle=moto_gym[1].angle;
    if(last_Tk1pointang!=now_Tk1pointang)
    {
        pit_auto_angle_set= Nowangle+(now_Tk1pointang-130)/100/360*8192/2;//��λ����Ƚ�����
    }
    autu_pit6623_AngleAdjust 	= (imu_data.gx)/16.40;//��λΪ��/��
    if(((pit_auto_angle_set - Nowangle)<10) && ((pit_auto_angle_set - Nowangle) > -10))
    {

        pid_calc(&Gm6623_pit_ang_pid_slow,Nowangle, pit_auto_angle_set );
        angle_pidout_temp = angle_pidout_temp*C + Gm6623_pit_ang_pid_slow.pos_out *(1-C);
        pid_calc(&Gm6623_pit_spd_pid,autu_pit6623_AngleAdjust,angle_pidout_temp);
    }
    else
    {

        pid_calc(&Gm6623_pit_ang_pid_quick,Nowangle, pit_auto_angle_set );
        angle_pidout_temp = angle_pidout_temp*C + Gm6623_pit_ang_pid_quick.pos_out *(1-C);
        pid_calc(&Gm6623_pit_spd_pid,autu_pit6623_AngleAdjust,angle_pidout_temp);
    }

    last_Tk1pointang=now_Tk1pointang;
}
*/




/**************************************/
//���Զ�pitch
/**************************************/
#define MAX_UP (4260-850)
#define MAX_DOWN (4260+240)
//��ͨ�˲�ϵ����0-1����Խ���˲�Ч��Խ�ã����ͺ�Խ��
//#define C_Filter_speed 0.0
//#define C_Filter_angle 0.0
//#define SiQu 2

float C_Filter_speed_pitch = 0.2;
float C_Filter_angle_pitch = 0.5;
short SiQu_mouse_y=2;
float k_mouse_y=3.0;
short up=0;
short down=0;
float km_nor_pit_angle_set = pit6623_MiddleAngle;
float km_nor_pit_angle_set_temp = pit6623_MiddleAngle;

float km_nor_pit_spd_set = 0;

void km_nor_pit_ctr(Mouse*mouse)
{

    //static float km_nor_pit_angle_set = pit6623_MiddleAngle;
    //static float km_nor_pit_angle_set_temp = pit6623_MiddleAngle;
    //static float k=5;
    static float NowAngle=pit6623_MiddleAngle;
    km_nor_pit_spd_fb = (imu_data.gx)/16.40;//��λΪ��/��  ,�ٶȷ���ֵ

    if(mouse->y < SiQu_mouse_y && mouse->y > (0-SiQu_mouse_y))
    {
        mouse->y = 0;
    }

    km_nor_pit_spd_set = (km_nor_pit_spd_set * C_Filter_speed_pitch) + k_mouse_y * mouse->y * (1 - C_Filter_speed_pitch);
    km_nor_pit_angle_set_temp = km_nor_pit_angle_set + km_nor_pit_spd_set;
    km_nor_pit_angle_set = km_nor_pit_angle_set * C_Filter_angle_pitch + km_nor_pit_angle_set_temp * (1 - C_Filter_angle_pitch);

    //���½Ƕ����ƣ�����km_nor_pit_angle_set��С��
    if(km_nor_pit_angle_set < (MAX_UP-up))  km_nor_pit_angle_set = MAX_UP;
    if(km_nor_pit_angle_set > (MAX_DOWN+down))  km_nor_pit_angle_set = MAX_DOWN;

    pid_calc(&Gm6623_pit_ang_pid,moto_gym[1].angle,km_nor_pit_angle_set);
    pid_calc(&Gm6623_pit_spd_pid,km_nor_pit_spd_fb,Gm6623_pit_ang_pid.pos_out);


    /*
    if(mouse->y>2||mouse->y<-2)
    {
    	pid_calc(&Gm6623_pit_spd_pid,km_nor_pit_spd_fb,km_nor_pit_spd_set);
    	NowAngle=moto_gym[1].angle;
    	Gm6623_pit_ang_pid.pos_out=0;
    	Gm6623_pit_ang_pid.iout=0;
    }
    else
    {
    	pid_calc(&Gm6623_pit_ang_pid,moto_gym[1].angle,NowAngle);
    	pid_calc(&Gm6623_pit_spd_pid,km_nor_pit_spd_fb,Gm6623_pit_ang_pid.pos_out);
    }
    */
}


float auto_Piych_offset=-100;
float fact_shoot = 0.5;
void km_au_pit_ctr(void)//���λ��ʽ�Զ����
{
    float C=0.35;
    int Nowangle;
    static float angle_pidout_temp=0;
    static double pit_auto_angle_set=pit6623_MiddleAngle;//��Ӧ�е�
    float now_Tk1pointang=0;
    static float last_Tk1pointang=0;
    now_Tk1pointang=Tk1pointang[1];
    if(Tk1pointang[2]==0)
    {
        now_Tk1pointang=last_Tk1pointang;
        km_nor_pit_ctr(&RC_CtrlData.mouse);
    }
    Nowangle=moto_gym[1].angle;//    /8192.0*360.0f;
    km_nor_pit_spd_fb = (imu_data.gx)/16.40;//��λΪ��/��  ,�ٶȷ���ֵ
    autu_pit6623_AngleAdjust 	= (imu_data.gx)/16.40;//��λΪ��/��

    if(last_Tk1pointang!=now_Tk1pointang)
    {
				now_Tk1pointang = last_Tk1pointang*0.2f + now_Tk1pointang	* 0.8f;
				pit_auto_angle_set= pit_auto_angle_set* 0.15 + (Nowangle+(now_Tk1pointang - auto_Piych_offset)*fact_shoot*0.113777777778)*0.85;//��λ����Ƚ�����
        km_nor_pit_angle_set = pit_auto_angle_set;

        pid_calc(&Gm6623_pit_ang_pid_quick,Nowangle,pit_auto_angle_set);
        pid_calc(&Gm6623_pit_spd_pid,km_nor_pit_spd_fb,Gm6623_pit_ang_pid_quick.pos_out);
    }


    last_Tk1pointang=now_Tk1pointang;
}



/**************************************/
//�ֶ��Զ��л�����
/**************************************/

void Gm6623_pitch_ang_ctr(void)
{
    //���µ���˳�򲻿ɱ�
    HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);
    IMU_Get_Data();
    switch(modeswitch_flag[NOW])
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        km_nor_pit_ctr(&RC_CtrlData.mouse);
        break;//�����涯ģʽ
    case 4:
        km_au_pit_ctr();
        break;//���λ���Զ���׼ģʽbreak;
    default:
        break;
    }
}

//void km_nor_yaw_ctr(Mouse*mouse)
//{
//		//static int32_t Nowangle = yaw6623_MiddleAngle;
//		km_nor_yaw_ang_set 		-= mouse->x*0.1;			//ϵ��Ϊ������
//		km_nor_yaw_spd_fb 		= (imu_data.gz)/16.40;//��λΪ��/��,
//		imu_data.anglez 	+= km_nor_yaw_spd_fb*12/1000;//��λΪ��
//
////	if(mouse->x ==0 )//������Ʈ�ɿ��˶�
////	{
////		imu_data.anglez = 0;//����imu�������
////		if(Nowangle > yaw6623_AngleAdjustLimit_left)
////			Nowangle = yaw6623_AngleAdjustLimit_left;
////		else if(Nowangle < yaw6623_AngleAdjustLimit_right)
////			Nowangle = yaw6623_AngleAdjustLimit_right;
////		//λ�õ���
////		pid_calc(&pid_yaw_pos,moto_gym[0].total_angle,Nowangle);
////		send_current_yaw = -pid_yaw_pos.pos_out;
////	}
////	else
//	{
////		Nowangle = moto_gym[0].total_angle;
//		km_nor_yaw_ang_fb  = imu_data.anglez*8192/360;
//		if(km_nor_yaw_ang_set > yaw6623_AngleAdjustLimit)
//			km_nor_yaw_ang_set = yaw6623_AngleAdjustLimit;
//		else if(km_nor_yaw_ang_set < -yaw6623_AngleAdjustLimit)
//			km_nor_yaw_ang_set = -yaw6623_AngleAdjustLimit;
//
//		//λ��˫��
//		pid_calc(&Gm6623_yaw_ang_pid,km_nor_yaw_ang_fb,km_nor_yaw_ang_set);
//		pid_calc(&Gm6623_yaw_spd_pid,km_nor_yaw_spd_fb,Gm6623_yaw_ang_pid.pos_out);
//		send_current_yaw = -Gm6623_yaw_spd_pid.pos_out;
//	}
//}


/**************************************/
//���Զ�yaw
/**************************************/

float C_Filter_speed_yaw = 0.2;
float C_Filter_angle_yaw = 0.5;
short SiQu_mouse_x=2;
float k_mouse_x=8.0;

void km_nor_yaw_ctr(Mouse*mouse)
{
    static float km_nor_yaw_spd_set=0;

    Follw_3508_pid.p = 6;


    km_nor_yaw_spd_fb 	= km_nor_yaw_spd_fb * 0.6  + (imu_data.gz)/16.40 * 0.4;//��λΪ��/��,

    km_nor_yaw_ang_fb 	+= km_nor_yaw_spd_fb*12/1000;//��λΪ��
    km_nor_yaw_spd_set = k_mouse_x * (-mouse->x);

    if(mouse->x < SiQu_mouse_x && mouse->x > (0-SiQu_mouse_x))
    {
        mouse->x = 0;
    }

    km_nor_yaw_spd_set = (km_nor_yaw_spd_set * C_Filter_speed_yaw) + k_mouse_x * (-mouse->x) * (1 - C_Filter_speed_yaw);

    if(mouse->x > 3 || mouse->x < -3)
    {
        pid_calc(&Gm6623_yaw_spd_pid,km_nor_yaw_spd_fb,km_nor_yaw_spd_set);
        km_nor_yaw_ang_fb=0;
        Gm6623_yaw_ang_pid.pos_out=0;
        Gm6623_yaw_ang_pid.iout=0;
    }
    else
    {
        pid_calc(&Gm6623_yaw_ang_pid, km_nor_yaw_ang_fb, 0);
        pid_calc(&Gm6623_yaw_spd_pid, km_nor_yaw_spd_fb, Gm6623_yaw_ang_pid.pos_out);
    }
}

/**************************************/
//�Զ�yaw
/**************************************/
/*
void km_au_yaw_ctr_yuanshi(void)//���λ��ʽ�Զ����
{
    float C=0.35;
    static float angle_pidout_temp=0;
    int Nowangle;
    static float yaw_auto_angle_set=yaw6623_MiddleAngle;//��Ӧ�е�
    float now_Tk1pointang=0;
    static float last_Tk1pointang=0;
    now_Tk1pointang=Tk1pointang[0];
    if(Tk1pointang[2]==0)
    {
        now_Tk1pointang=last_Tk1pointang;
    }
    Nowangle=moto_gym[0].angle/8192.0*360.0f/0.012f;
    if(last_Tk1pointang!=now_Tk1pointang)
    {
        yaw_auto_angle_set= Nowangle-(now_Tk1pointang-200)/100*8192/360/2;//��λ����Ƚ�����
    }
    autu_yaw6623_AngleAdjust 	= (imu_data.gz)/16.40;//��λΪ��/��

    if(((yaw_auto_angle_set - Nowangle)<10) && ((yaw_auto_angle_set - Nowangle) > -10))
    {

        pid_calc(&Gm6623_yaw_ang_pid_slow,Nowangle, yaw_auto_angle_set );
        angle_pidout_temp = angle_pidout_temp*C + Gm6623_yaw_ang_pid_slow.pos_out *(1-C);
        pid_calc(&Gm6623_yaw_spd_pid,autu_yaw6623_AngleAdjust,angle_pidout_temp);
    }
    else
    {
        pid_calc(&Gm6623_yaw_ang_pid_quick,Nowangle, yaw_auto_angle_set );
        angle_pidout_temp = angle_pidout_temp*C + Gm6623_yaw_ang_pid_quick.pos_out *(1-C);
        pid_calc(&Gm6623_yaw_spd_pid,autu_yaw6623_AngleAdjust,angle_pidout_temp);
    }
    last_Tk1pointang=now_Tk1pointang;

}
*/

/**************************************/
//�Զ�yaw
/**************************************/
float auto_yaw_offset=300;
float yaw_auto_angle_set=yaw6623_MiddleAngle/8192.0*360.0f;//��Ӧ�е�
void km_au_yaw_ctr(void)
{
    static float km_nor_yaw_spd_set=0;

    float C=0.35;
    static float angle_pidout_temp=0;
    int Nowangle;

    float now_Tk1pointang=0;
    static float last_Tk1pointang=0;

    Follw_3508_pid.p = 1;

    now_Tk1pointang=Tk1pointang[0];
    km_nor_yaw_spd_fb 	= km_nor_yaw_spd_fb * 0.6  + (imu_data.gz)/16.40 * 0.4;//��λΪ��/��,

    if(Tk1pointang[2]==0)
    {
        now_Tk1pointang=last_Tk1pointang;
        km_nor_yaw_ctr(&RC_CtrlData.mouse);
    }
    Nowangle=moto_gym[0].angle/8192.0*360.0f;

    if(last_Tk1pointang!=now_Tk1pointang)
    {
        //yaw_auto_angle_set= Nowangle-(now_Tk1pointang-200)/100*8192/360/2;//��λ����Ƚ�����
        yaw_auto_angle_set= Nowangle-(now_Tk1pointang-auto_yaw_offset)*0.01;//��λ����Ƚ�����
        pid_calc(&Gm6623_yaw_ang_pid_quick, Nowangle, yaw_auto_angle_set);
        pid_calc(&Gm6623_yaw_spd_pid, km_nor_yaw_spd_fb, Gm6623_yaw_ang_pid_quick.pos_out);
    }




    last_Tk1pointang=now_Tk1pointang;

}

/**************************************/
//ģʽ�л�����yaw
/**************************************/
void Gm6623_yaw_ang_ctr(void)
{
    HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);
    IMU_Get_Data();
    switch(modeswitch_flag[NOW])
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        km_nor_yaw_ctr(&RC_CtrlData.mouse);
        break;//�����涯ģʽ
    case 4:
        km_au_yaw_ctr();
        break;//���λ���Զ���׼ģʽbreak;
    default:
        break;
    }
}
//�涯ģʽ

float JiaJiao_now=0;
float JiaJiao_last=0;
float w_JiaJiao=0; //��̨����̼нǱ仯�ٶȡ�

void cm_follow(void)
{
    bool quitfollow_flag;

    JiaJiao_now = yaw6623_MiddleAngle - moto_gym[0].angle;
    w_JiaJiao = w_JiaJiao * 0.5f + (JiaJiao_now - JiaJiao_last) * 0.5f;
    JiaJiao_last = JiaJiao_now;

    pid_follow_calc(&Follw_3508_pid, moto_gym[0].total_angle, yaw6623_MiddleAngle);//�ǶȻ�
    //pid_follow_calc(&Follw_3508_pid_speed, w_JiaJiao, Follw_3508_pid.pos_out);//���ٶȻ�
//	 if(abs(moto_gym[0].total_angle-yaw6623_MiddleAngle)<200)
//		 quitfollow_flag=0;
//	 else
    quitfollow_flag=1;

    Cm3508_total_set[0] = quitfollow_flag*Follw_3508_pid.pos_out+Cm3508_spd_set[0];
    Cm3508_total_set[1] = quitfollow_flag*Follw_3508_pid.pos_out+Cm3508_spd_set[1];
    Cm3508_total_set[2] = quitfollow_flag*Follw_3508_pid.pos_out+Cm3508_spd_set[2];
    Cm3508_total_set[3] = quitfollow_flag*Follw_3508_pid.pos_out+Cm3508_spd_set[3];

//	   Cm3508_total_set[0] = quitfollow_flag*Follw_3508_pid_speed.pos_out+Cm3508_spd_set[0];
//    Cm3508_total_set[1] = quitfollow_flag*Follw_3508_pid_speed.pos_out+Cm3508_spd_set[1];
//    Cm3508_total_set[2] = quitfollow_flag*Follw_3508_pid_speed.pos_out+Cm3508_spd_set[2];
//    Cm3508_total_set[3] = quitfollow_flag*Follw_3508_pid_speed.pos_out+Cm3508_spd_set[3];
}

/**************************************/
//���̿���
/**************************************/
//����ģʽ
void cm_unfollow(void)
{
    Cm3508_total_set[0] = Cm3508_spd_set[0];
    Cm3508_total_set[1] = Cm3508_spd_set[1];
    Cm3508_total_set[2] = Cm3508_spd_set[2];
    Cm3508_total_set[3] = Cm3508_spd_set[3];
}
void CM_ctr(void)
{
    if(follow_flag)
        cm_follow();
    else
        cm_unfollow();
    cm_spd_ctr();
}

/**************************************/
//δʹ��
/**************************************/
void GM6623_yaw_autu_ctr(void)//����λ��ʽ�Զ����
{
    autu_6623yaw_imu_data.Velocity[NOW] = (imu_data.gz - ImugzErr)/25 ;//����ʽȥ�²�˲�
    autu_6623yaw_imu_data.Angle[NOW] 	= autu_6623yaw_imu_data.Velocity[NOW] + autu_6623yaw_imu_data.Angle[LAST];
    //autu_yaw6623_AngleAdjust = moto_gym[0].angle - yaw6623_MiddleAngle;
    autu_yawimu_AngleAdjust 	= autu_6623yaw_imu_data.Angle[NOW];
    //Gm6623_yaw_ang_give = Gm6623_yaw_ang_set - yawimu_AngleAdjust * yawimu_AngleAdjustK;
    autu_pid_calc_yaw(&autu_Gm6623_yaw_ang_pid, autu_yawimu_AngleAdjust, autu_Gm6623_yaw_ang_set+autu_yawimu_AngleAdjust);//�ı�Gm6623_yaw_ang_set�����̸�����̨����ʹGm6623_yaw_ang_give������4650��+-5��

    autu_6623yaw_imu_data.Velocity[LAST] = autu_6623yaw_imu_data.Velocity[NOW];
    autu_6623yaw_imu_data.Angle[LAST] 	 = autu_6623yaw_imu_data.Angle[NOW];

//		if(autu_Gm6623_yaw_ang_set-autu_yawimu_AngleAdjust<autuang_limit)
//		  autuaim_flag=0;
//		else
//			autuaim_flag=1;

}


