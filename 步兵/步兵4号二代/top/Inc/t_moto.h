#ifndef _T_MOTO_H
#define _T_MOTO_H
#include "m_remote.h"
#define ANGLE_MAX 1200//1300
#define ANGLE_MIN 200//100
//yaw��ת~gz++,ang++��pitch��ת~gx++,ang++
#define ImugzErr 20
#define yaw6623_MiddleAngle 3905
#define yaw6623_AngleAdjustLimit_left 5500
#define yaw6623_AngleAdjustLimit_right 2500
#define yaw6623_AngleAdjustLimit 1500

#define yawimu_AngleAdjustK 1

#define pit6623_MiddleAngle 4260
#define pitch6623_AngleAdjustLimit_top 3800
#define pitch6623_AngleAdjustLimit_bottom 4450


extern double send_current_yaw;
extern double send_current_pit;
extern int err;
extern int offset ;
typedef struct Follow_6623_imu_fbangall
{
	int Velocity[3];
	int Angle[3];
	
}Follow_6623_imu_fbangall_t;
//�涯
extern Follow_6623_imu_fbangall_t follow_6623yaw_imu_data;
extern int yaw6623_AngleAdjust;
extern int pitimu_AngleAdjust;
//�Զ���׼
extern Follow_6623_imu_fbangall_t autu_6623yaw_imu_data;
extern int autu_yaw6623_AngleAdjust;
extern int autu_yawimu_AngleAdjust;
extern float autu_Gm6623_yaw_ang_set;
extern int autu_pitimu_AngleAdjust;
extern float autu_Gm6623_pit_ang_set;


extern float Cm3508_spd_get[4];
extern float Cm3508_spd_xset[4];//x��������ٶ�
extern float Cm3508_spd_yset[4];//y��������ٶ�
extern float Cm3505_spd_sqin[4];//�����˶�
extern float Cm3508_spd_set[4];//�ϳ��ٶ�

extern float Gm6623_yaw_ang_set;
extern float Gm6623_yaw_ang_give;
extern float Gm6623_yaw_spd_set;

extern float Gm6623_pit_ang_set;
extern float Gm6623_pit_ang_give;
extern float Gm6623_pit_spd_set;

//�涯����
extern float Follow_6623_imu_fbang;//������̨���淴����
extern float Follow_6623_imu_lastfbang;//�ϴ���̨���淴����
extern float Cm3508_total_set[4];//��pid���=�����˶���pid���+�涯pid���

//�������
extern double km_nor_yaw_temang;
extern double km_nor_yaw_ang_fb;
extern double km_nor_yaw_ang_set;
extern double km_nor_yaw_spd_fb;

extern double km_nor_pit_temang;
extern double km_nor_pit_ang_fb;
extern double km_nor_pit_ang_set;
extern double km_nor_pit_spd_fb;

void cm_spd_ctr(void);
extern void Gm6623_pitch_ang_ctr(void);
extern void Gm6623_yaw_ang_ctr(void);
void cm_follow(void);
void cm_unfollow(void);
extern void CM_ctr(void);
extern void cm_spd_current_ctr(pid_t *pid,float fbspeed, float setspeed);


extern void km_nor_yaw_ctr(Mouse *mouse);
extern void GM6623_yaw_autu_ctr(void);//����λ��ʽ�Զ����  
extern void km_au_yaw_ctr(void);//���λ��ʽ�Զ����

extern void km_au_pit_ctr(void);
extern void km_nor_pit_ctr(Mouse *mouse);

#endif

