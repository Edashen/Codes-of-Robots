#ifndef __T_PROTOCOL_H
#define __T_PROTOCOL_H

#include "mytype.h"


#define POWER_MAX 80//Ӣ�۳���120�������������Ƶ��̹���
#define POWER_SET 75//

//float HEAT_MAX[3]={1600,3000,6000};//��˼������⣺Ϊʲô����ͻᱨ��������������������������������������
//float HEAT_SET[3]={1400,2800,5800};

#define SHOOT17_SPD_MAX 30//  30m/s  //Ӣ�۳��У�#define SHOOT42_SPD_MAX 16.5//  16.5m/s

extern u8 level;
extern void PowerHeat_ctr(void);


#endif


