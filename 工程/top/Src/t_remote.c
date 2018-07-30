#include "t_remote.h"
#include "t_moto.h"
#include "m_toggle.h"
#include "control.h"
extern float Cm3508_spd_xset[4];//x��������ٶ�
extern float Cm3508_spd_yset[4];//y��������ٶ�
extern float Cm3508_spd_set[4];//�ϳ��ٶ�
extern float Cm3505_spd_sqin[4];//�����˶�
extern float Cm3505_spd_sqin[4];//�����˶�
extern float Cm3508_spd_offset[4];
//unsigned char Cm3508_offset_state;//ȥƯ��У׼״̬λ,��ʱû�õ�

extern float Gm6623_pit_ang_set;
extern float Gm6623_pit_ang_offset;
extern float Gm6623_pit_spd_set;
extern float Gm6623_yaw_ang_set;
extern float Gm6623_yaw_spd_set;

extern short int Tk1pointang[3];
extern u16 USART_RX_STA;       //����״̬���

	
InputMode_e inputmode = REMOTE_INPUT;   //����ģʽ�趨,Ĭ��Ϊң����ģʽ
LeftsideMode_e leftsidemode=STOPSHOOT;    //��ҡ��ģʽ�趨��Ĭ��Ϊ�����˶�
MouseKeyMode_e mousekeymode=MKNORMAL;     //����ģʽѡ��Ĭ��Ϊ���Ƶ���ȫ��λ�ƶ�
MouseKeyNormode_e mousekeynormode=MKFOLLOW;

int Rub2323_spd=1500;
unsigned char Cm3508_offset_state;
float yaw_tk1_angle=4650;
float mousekey_spd[6]={3000,3000,3000,3000,-3000,3000};
KEY KeyBoard;
unsigned short shiftcount=0;
unsigned short press2count=0;
uint8_t shootflag=0;
bool usetogger=0;
bool usedtogger=0;
//����ģʽ���� 
void setinputmode(Remote *rc)
{
	if(rc->s2 == 1)
	{
		inputmode = REMOTE_INPUT;
	}
	else if(rc->s2 == 3)
	{
		inputmode = KEY_MOUSE_INPUT;
	}
	else if(rc->s2 == 2)
	{
		inputmode = KEY_MOUSE_INPUT;
	}	
}
InputMode_e getinputmode(void)
{
	return inputmode;
}
//���ҡ��ģʽ���� 
void setleftsidemode(Remote *rc)
{
	if(rc->s1 == 1)
	{
		leftsidemode = SHOOT;
	}
	else if(rc->s1 == 3)
	{
		leftsidemode = STOPSHOOT;
	}
	else if(rc->s1 == 2)
	{
		leftsidemode=UNSHOOT;
	}
}
LeftsideMode_e getlsmode(void)
{
   return leftsidemode;
}

void ctrcm(Remote *rc)
{ 
	 //���̺ϳ��˶�
	 Cm3508_spd_set[0]=Cm3508_spd_xset[0]+Cm3508_spd_yset[0]+Cm3505_spd_sqin[0];
	 Cm3508_spd_set[1]=Cm3508_spd_xset[1]+Cm3508_spd_yset[1]+Cm3505_spd_sqin[1];
	 Cm3508_spd_set[2]=Cm3508_spd_xset[2]+Cm3508_spd_yset[2]+Cm3505_spd_sqin[2];
	 Cm3508_spd_set[3]=Cm3508_spd_xset[3]+Cm3508_spd_yset[3]+Cm3505_spd_sqin[3];
}
void ctrcmsqin(Remote *rc)
{
	 
	 Cm3505_spd_sqin[0]=(rc->ch0-1024.0)/660.0*4000.0;
	 Cm3505_spd_sqin[1]=(rc->ch0-1024.0)/660.0*4000.0;
	 Cm3505_spd_sqin[2]=(rc->ch0-1024.0)/660.0*4000.0;
	 Cm3505_spd_sqin[3]=(rc->ch0-1024.0)/660.0*4000.0;
}

void ctrcmallrun(Remote *rc)
{  //ȫ��λ�˶�
	 Cm3508_spd_yset[0]=(rc->ch3-1024.0)/660.0*3000.0;   //�����3000Ϊ�ɱ�����������趨ң���������ŵ���ֵ����ֵΪ16384
	 Cm3508_spd_yset[1]=-((rc->ch3-1024.0)/660.0*3000.0);//ע���������Ϊ������������ң�س���
	 Cm3508_spd_yset[2]=(rc->ch3-1024.0)/660.0*3000.0;
	 Cm3508_spd_yset[3]=-((rc->ch3-1024.0)/660.0*3000.0);

	 Cm3508_spd_xset[0]=(rc->ch2-1024.0)/660.0*3000.0;
	 Cm3508_spd_xset[1]=((rc->ch2-1024.0)/660.0*3000.0);
	 Cm3508_spd_xset[2]=-(rc->ch2-1024.0)/660.0*3000.0;
	 Cm3508_spd_xset[3]=-((rc->ch2-1024.0)/660.0*3000.0);
}

//����yawλ�û���ת���ٶ�̫��
//void ctrgy(Remote *rc)
//{
//   Gm6623_pit_ang_set=-(rc->ch3-1024.0)/660.0*600.0+2700.0;  
//	 //Gm6623_yaw_ang_set=-(rc->ch2-1024.0)/660.0*4000.0+4650.0;  
////	 if(rc->ch2>1200)
////	 {
////	     Gm6623_yaw_ang_set=Gm6623_yaw_ang_set+20;
////	 }
////	 else if(rc->ch2<900)
////	 {
////	     Gm6623_yaw_ang_set=Gm6623_yaw_ang_set-20;	     
////	 }
////	 else
////	 {}
//	 if(rc->ch2>1200)
//	 {
//	     Gm6623_yaw_ang_set=Gm6623_yaw_ang_set-40;
//	 }
//	 else if(rc->ch2<900)
//	 {
//	     Gm6623_yaw_ang_set=Gm6623_yaw_ang_set+40;	     
//	 }
//	 else
//	 {
//       	 Gm6623_yaw_ang_set=Gm6623_yaw_ang_set;
//	 }
//	 if(Gm6623_yaw_ang_set>10000)
//	 {
//	   Gm6623_yaw_ang_set=10000;
//	 }
//	 if(Gm6623_yaw_ang_set<-10000)
//	 {
//	   Gm6623_yaw_ang_set=-10000;
//	 }
//	 Cm3505_spd_sqin[0]=0;
//	 Cm3505_spd_sqin[1]=0;
//	 Cm3505_spd_sqin[2]=0;
//	 Cm3505_spd_sqin[3]=0;
//}

//����yaw�ٶȻ�
void ctrgy(Remote *rc)
{
   Gm6623_pit_spd_set=-(rc->ch1-1024.0)/660.0*350.0;  
	 Gm6623_yaw_spd_set=-(rc->ch0-1024.0)/660.0*350.0;//��500ʱ��������Ϊ360��/��
	
	 Cm3505_spd_sqin[0]=0;
	 Cm3505_spd_sqin[1]=0;
	 Cm3505_spd_sqin[2]=0;
	 Cm3505_spd_sqin[3]=0;
	
	//�Ƕ�����
	 err = moto_gym[0].angle - yaw6623_MiddleAngle;
	 err = err>0 ? err : -err;
	
	 if(err > yaw6623_AngleAdjustLimit)
	 {
			Gm6623_yaw_spd_set=0;
	 }
}

//ң��ģʽ
void remotectlprocess(Remote *rc)
{
	// pidswitch_flag[NOW]=1;
	 //pidswitch_flag[LAST]=1;
	 setleftsidemode(rc);
	 switch(getlsmode())
	 {
		 case SHOOT:
										usetogger=1;
										if(usedtogger==1)
										{
											Toggler_ang_set+=1600;
											usedtogger=0;
										}
		 
										break;
		 case STOPSHOOT:  
										usetogger=0;
										usedtogger=1;
										Toggler_ang_set=Toggler_ang_set;
										break;
		 
		 case UNSHOOT:      
										usetogger=1;
										if(usedtogger==1)
										{
											Toggler_ang_set-=1600;
											usedtogger=0;
										}
										break;

		 default:
										Toggler_ang_set=Toggler_ang_set;
										break;
	 }
}

MouseKeyMode_e getmousekeymode(void)
{
   return mousekeymode;
}

MouseKeyNormode_e getmknormode(void)
{
   return mousekeynormode;
}

//�Զ���׼ģʽһ���б�־λautuaim_flag,��ʽ�ã�
void autuaim(Remote *rc)
{ 	
	pidswitch_flag[NOW]=2;
	pidswitch_flag[LAST]=2;
	//�����ڽ��յ�tk1���µ����ݣ���Tk1dataprocess()�н�autuaim_flag��1��Tk1pointang������,��������
	autu_Gm6623_yaw_ang_set= Tk1pointang[angx]/100*8191/360;	
	Gm6623_pit_ang_set= 2700;	
	//Gm6623_pit_ang_set= moto_gym[1].angle-Tk1pointang[angy]/100*8191/360*autuaim_flag-Gm6623_pit_ang_offset;	

	if(USART_RX_STA&0x8000)//�����tk1���
		USART_RX_STA=0; 
	ctrcmallrun(rc);
	ctrcmsqin(rc);
	ctrcm(rc);
	//	//Ħ�����ٶ�����
	
}
////2018.3.16�Զ���׼�߼�������ȷ
////�Զ���׼ģʽһ���б�־λautuaim_flag,��ʽ�ã�
//void autuaim(void)
//{ 	
//	//�����ڽ��յ�tk1���µ����ݣ���Tk1dataprocess()�н�autuaim_flag��1��Tk1pointang������,��������
//    //Gm6623_yaw_ang_set= moto_gym[0].angle+Tk1pointang[angx]/100*8191/360*autuaim_flag;
//	//Gm6623_yaw_ang_set= moto_gym[0].angle+Tk1pointang[angx]/100*8191/360;
//	autu_Gm6623_yaw_ang_set= Tk1pointang[angx]/100*8191/360;
//	
//	Gm6623_pit_ang_set= moto_gym[1].angle-Tk1pointang[angy]/100*8191/360*autuaim_flag-Gm6623_pit_ang_offset;	
////	if(autuaim_flag==1)//���autuaim_flagΪ1��˵��tk1�Ѿ��������ݣ�������ang_set��ʹ��
////	  autuaim_flag=0;//��0���ȴ��ٴθ���
////	if(USART_RX_STA&0x8000)//�����tk1���
////		USART_RX_STA=0; 
//}
//�Զ���׼ģʽ�����ޱ�־λautuaim_flag,�����ã�
//void autuaim(void)
//{ 	
//	//�����ڽ��յ�tk1���µ����ݣ���Tk1dataprocess()�н�autuaim_flag��1��Tk1pointang������,��������
//  Gm6623_yaw_ang_set= moto_gym[0].angle+Tk1pointang[angx]/100*8191/360;
//	Gm6623_pit_ang_set= moto_gym[1].angle-Tk1pointang[angy]/100*8191/360-Gm6623_pit_ang_offset;	
//	if(autuaim_flag==1)//���autuaim_flagΪ1��˵��tk1�Ѿ��������ݣ�������ang_set��ʹ��
//	  autuaim_flag=0;//��0���ȴ��ٴθ���
//	if(USART_RX_STA&0x8000)//�����tk1���
//		USART_RX_STA=0; 
//}


////�Զ���׼ģʽ������������У׼ʹ��,Ŀ���ǵõ�Gm6623_pit_ang_offset.��̨Pitch�Ļ���ƫת��������ɵģ�
//void autuaim(void)
//{ 	
//	//�����ڽ��յ�tk1���µ����ݣ���Tk1dataprocess()�н�autuaim_flag��1��Tk1pointang������,��������
//  Gm6623_yaw_ang_set= moto_gym[0].angle;
//	Gm6623_pit_ang_set= moto_gym[1].angle-Gm6623_pit_ang_offset;	
//	if(autuaim_flag==1)//���autuaim_flagΪ1��˵��tk1�Ѿ��������ݣ�������ang_set��ʹ��
//	  autuaim_flag=0;//��0���ȴ��ٴθ���
//	if(USART_RX_STA&0x8000)//�����tk1���
//		USART_RX_STA=0; 
//	
//}
//����ɲ��ģʽ,��ʱû�õ�
//void stopprocess(void)
//{
//	 Cm3508_spd_set[0]=0;
//	 Cm3508_spd_set[1]=0;
//	 Cm3508_spd_set[2]=0;
//	 Cm3508_spd_set[3]=0;
//}

void readkey(Key *key)
{  
   if((key->v&0x0001)!=0) KeyBoard.W=1; else KeyBoard.W=0;
   if((key->v&0x0002)!=0) KeyBoard.S=1; else KeyBoard.S=0;
   if((key->v&0x0004)!=0) KeyBoard.A=1; else KeyBoard.A=0;
   if((key->v&0x0008)!=0) KeyBoard.D=1; else KeyBoard.D=0;
   if((key->v&0x0010)!=0) KeyBoard.shift=1; else KeyBoard.shift=0;
   if((key->v&0x0020)!=0) KeyBoard.ctrl=1;  else KeyBoard.ctrl=0;
	 if((key->v&0x0040)!=0) KeyBoard.Q=1; else KeyBoard.Q=0;
   if((key->v&0x0080)!=0) KeyBoard.E=1; else KeyBoard.E=0;
	 if((key->v&0x0100)!=0) KeyBoard.R=1; else KeyBoard.R=0;
   if((key->v&0x0200)!=0) KeyBoard.F=1; else KeyBoard.F=0;
   if((key->v&0x0400)!=0) KeyBoard.G=1; else KeyBoard.G=0;
   if((key->v&0x0800)!=0) KeyBoard.Z=1; else KeyBoard.Z=0;
   if((key->v&0x1000)!=0) KeyBoard.X=1; else KeyBoard.X=0;
   if((key->v&0x2000)!=0) KeyBoard.C=1; else KeyBoard.C=0;
   if((key->v&0x4000)!=0) KeyBoard.V=1; else KeyBoard.V=0;
   if((key->v&0x8000)!=0) KeyBoard.B=1; else KeyBoard.B=0;
};
void readmouse(Mouse*mouse)
{
}
/****************************************
���ܣ����̿��Ƶ���ȫ��λ�˶�
���ڣ�2018.4.1
ע�⣺�������ͷǳ���Ҫ
****************************************/
void mkallrun(KEY* keyboard)
{
//	static uint8_t spd_keysta;
//	if((mousekey_spd[GO]<MKMAXSPD)&&(mousekey_spd[GO]>MKMINSPD))
//	{
//		switch(spd_keysta)
//		{
//			case 0:
//						if(KeyBoard.Q && KeyBoard.W)
//							spd_keysta=1;
//						else if(KeyBoard.Q && KeyBoard.S)
//							spd_keysta=1;
//						break;
//						
//			case 1:
//						if(KeyBoard.Q && KeyBoard.W)
//							spd_keysta=2;
//						else if(KeyBoard.Q && KeyBoard.S)
//							spd_keysta=3;
//						else
//							spd_keysta=0;
//						break;
//						
//			case 2://����
//						if((!KeyBoard.Q))
//						{
//							mousekey_spd[GO]=mousekey_spd[BACK]=mousekey_spd[LEFT]=mousekey_spd[RIGHT]=mousekey_spd[TURNLEFT]=mousekey_spd[TURNRIGHT]=mousekey_spd[GO]+100;	
//							spd_keysta=0;
//						}
//						break;
//						
//			case 3://����
//						if((!KeyBoard.Q))
//						{
//							mousekey_spd[GO]=mousekey_spd[BACK]=mousekey_spd[LEFT]=mousekey_spd[RIGHT]=mousekey_spd[TURNLEFT]=mousekey_spd[TURNRIGHT]=mousekey_spd[GO]-100;
//							spd_keysta=0;
//						}
//						break;
//		}
//	}
	float mk_rl_spd;
	short rl_flag[2];
	if(( keyboard->A )&&( !keyboard->D ))
	{
		rl_flag[0]=-1;
		rl_flag[1]= 1;
		mk_rl_spd=mousekey_spd[TURNLEFT];
	}
	else if(( keyboard->D )&&( !keyboard->A ))
	{
		rl_flag[0]= 1;
		rl_flag[1]=-1;
		mk_rl_spd=mousekey_spd[TURNRIGHT];
	}
	else 
	{
		rl_flag[0]=rl_flag[1]=0;
		mk_rl_spd=0;
	}
	Cm3508_spd_xset[0] = mk_rl_spd * rl_flag[0];
	Cm3508_spd_xset[1] = mk_rl_spd * rl_flag[0];
	Cm3508_spd_xset[2] = mk_rl_spd * rl_flag[1];
	Cm3508_spd_xset[3] = mk_rl_spd * rl_flag[1];

	float mk_ud_spd;
	short ud_flag[2];
	if(( keyboard->W )&&( !keyboard->S ))
	{
		ud_flag[0]= 1;
		ud_flag[1]=-1;
		mk_ud_spd=mousekey_spd[GO];
	}
	else if(( keyboard->S )&&( !keyboard->W ))
	{
		ud_flag[0]=-1;
		ud_flag[1]= 1;
		mk_ud_spd=mousekey_spd[BACK];
	}
	else 
	{
		ud_flag[0]=ud_flag[1]=0;
		mk_ud_spd=0;
	}
	Cm3508_spd_yset[0] = mk_ud_spd * ud_flag[0];
	Cm3508_spd_yset[1] = mk_ud_spd * ud_flag[1];
	Cm3508_spd_yset[2] = mk_ud_spd * ud_flag[0];
	Cm3508_spd_yset[3] = mk_ud_spd * ud_flag[1];
}

void mktotalrun(void)
{
	//�ϳ��˶�
	Cm3508_spd_set[0]=Cm3508_spd_xset[0]+Cm3508_spd_yset[0]+Cm3505_spd_sqin[0]+Cm3508_spd_offset[0]*Cm3508_offset_state;
	Cm3508_spd_set[1]=Cm3508_spd_xset[1]+Cm3508_spd_yset[1]+Cm3505_spd_sqin[1]+Cm3508_spd_offset[1]*Cm3508_offset_state;
	Cm3508_spd_set[2]=Cm3508_spd_xset[2]+Cm3508_spd_yset[2]+Cm3505_spd_sqin[2]+Cm3508_spd_offset[2]*Cm3508_offset_state;
	Cm3508_spd_set[3]=Cm3508_spd_xset[3]+Cm3508_spd_yset[3]+Cm3505_spd_sqin[3]+Cm3508_spd_offset[3]*Cm3508_offset_state;
}
//�������
void shoot(Mouse*mouse)
{
	static uint8_t shootkey_sta;
	switch(shootkey_sta)
	{
		case 1:
					if( mouse->press_2 == 1 )
						shootkey_sta=2;
					break;
		case 2:
					if( mouse->press_2 == 1)
						shootkey_sta=3;
					else
						shootkey_sta=1;
					break;
		case 3:
					if(( mouse->press_2 == 0)&&( KeyBoard.R ==1 ))
					{
						shootkey_sta=1;
						shootflag=1;//״̬һ����ת����
					}
					else if(( mouse->press_2 == 0)&&( KeyBoard.R ==1 ))
					{
						shootkey_sta=1;
						shootflag=2;//״̬����������ת
					}
					break;
		default:break;			
	}
	
} 
//��ͨģʽһ����̨���̸���ģʽ
void mkfollow(Mouse*mouse)
{
	Gm6623_yaw_spd_set=mouse->x/32767.0f*350.0f*mouse->press_1;//�����϶�
	Gm6623_pit_ang_set+=mouse->y;//�����ƶ��ٶȻ��֣��õ�λ��
	//Gm6623_pit_ang_set=3105.0;
//	Gm6623_pit_ang_set=mouse->x/32767.0f*22.7f*45.0f*mouse->press_1;
		
	 Cm3505_spd_sqin[0]=0;
	 Cm3505_spd_sqin[1]=0;
	 Cm3505_spd_sqin[2]=0;
	 Cm3505_spd_sqin[3]=0;
	
	//�Ƕ�����
	 err = moto_gym[0].angle - yaw6623_MiddleAngle;
	 err = err>0 ? err : -err;
	
	 if(err > yaw6623_AngleAdjustLimit)
	 {
			Gm6623_yaw_spd_set=0;
	 }
	 mkallrun(&KeyBoard);
	 mktotalrun();
}

//��ͨģʽ������̨���̷���
void mkseparate(KEY* keyboard)
{
	float mk_spd;
	bool flag;
	unsigned char i;
	if(( keyboard->A )&&( !keyboard->D ))
	{
		flag=1;
		mk_spd=mousekey_spd[TURNLEFT];
	}
	else if(( !keyboard->A )&&( keyboard->D ))
	{
		flag=1;
		mk_spd=mousekey_spd[TURNRIGHT];
	}
	else 
	{
		flag=0;
		mk_spd=0;
	}
	for(i=0;i<4;i++)
	{
		Cm3505_spd_sqin[i]=	mk_spd*flag;
	}
	
	mkallrun(&KeyBoard);
	mktotalrun();
}

//��ͨģʽ�����Զ���׼ģʽ
void mkautu()
{
}

//����ģʽһ����ͨģʽ��Ĭ��ģʽ��
void mknormal(Mouse *mouse, Key *key)
{
	static int norkey_sta,normode_sta;
	switch(norkey_sta)
	{
		case 0:	//no key
					if( 1 == KeyBoard.E )
						norkey_sta = 1;
					break;
		case 1: //key down wait release.
					if( 1 == KeyBoard.E )
					{
						norkey_sta = 2;
					}
					else
					{
						norkey_sta = 0;
					}
					break;
		case 2: 
					if( 0 == KeyBoard.E )
					{
						norkey_sta = 0;
						normode_sta++;
						if( normode_sta == 4 )
							normode_sta=1;
					}
					break;
		default:
					if( 1 == KeyBoard.E )
						norkey_sta = 1;
					break;
	}
	switch(normode_sta)
	{
		//����ģʽ
		case MKFOLLOW:	 mkfollow(&RC_CtrlData.mouse);
										 break;
		//����ģʽ
		case MKSEPARATE: mkseparate(&KeyBoard);
										 break;
		
		//�Զ���׼ģʽ
		case MKAUTU:		 mkautu();
										 break;
		
		default:				 mkseparate(&KeyBoard);
										 break;
	}
}

//����ģʽ�������µ�ģʽ
void mkupdown(Key *key)
{
}
void mkegg(void)
{
}

//����ģʽ
void mousekeycontrolprocess(Mouse *mouse, Key *key)
{
	readkey(key);//������ֵ
	shoot(&RC_CtrlData.mouse);//�������ѡ��
	static int mkkey_sta,mkmode_sta=1;
	switch(mkkey_sta)
	{
		case 0:	//no key
					if( 1 == KeyBoard.ctrl )
						mkkey_sta = 1;
					break;
		case 1: //key down wait release.
					if( 1 == KeyBoard.ctrl )
					{
						mkkey_sta = 2;
					}
					else
					{
						mkkey_sta = 0;
					}
					break;
		case 2: 
					if( 0 == KeyBoard.ctrl )
					{
						mkkey_sta = 0;
						mkmode_sta++;
						if( mkmode_sta == 4 )
							mkmode_sta=1;	
					}
					break;
					
		default:									
					if( 1 == KeyBoard.ctrl )
						mkkey_sta = 1;
					break;
	}
	switch(mkmode_sta)
	{
		//��ͨģʽ
		case MKNORMAL:	mknormal(&RC_CtrlData.mouse,&RC_CtrlData.key);
										break;
		//���µ�ģʽ
		case MKUPDOWN:	mkupdown(&RC_CtrlData.key);
										break;
		//����ģʽ
		case MKEGG:     mkegg();
										break;
		
		default:				mknormal(&RC_CtrlData.mouse,&RC_CtrlData.key);
										break;
	}
}

