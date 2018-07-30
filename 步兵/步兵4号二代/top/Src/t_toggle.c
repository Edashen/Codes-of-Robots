#include "t_toggle.h"
#include "m_toggle.h"
#include  "stdint.h"
 short shootnum=0;
unsigned short toggle_heat_ctr(unsigned short robot_type, uint8_t rlgrade,float rlspd,uint8_t bulletfreq,uint8_t bullettype,uint16_t heat0,uint16_t heat1)
{
		int limshootnum[2];
		int allownum[2];
		float limheat[2];
		float max_freq[2]={2,2};
		static float res_spd=15;
		
		switch(robot_type)
		{
			case Infantry:
											switch(rlgrade)
											{
												case 0:limheat[Bullet17]=90-20; break;//rtheat��msΪ��λ
												case 1:limheat[Bullet17]=180-20;break;
				 								case 2:limheat[Bullet17]=270-20;break;
												default:break;
											}
											if(limheat[Bullet17] < heat0)
												allownum[Bullet17]=0;
											else
											{
												if(rlspd)
													res_spd=rlspd;
												rlspd=res_spd;
												limshootnum[Bullet17]=(limheat[Bullet17]-heat0)/rlspd;//���ɷ�����
												if(limshootnum[Bullet17]<1)
													allownum[Bullet17]=0;
												else
													allownum[Bullet17]=1;
												//allownum[Bullet17]=limshootnum[Bullet17]/max_freq[Bullet17];//��������
											}
											return	allownum[Bullet17];		
											
			case Hero:			
											switch(rlgrade)
											{
												case 0:limheat[Bullet17]=90; limheat[Bullet42]=80; break;//rtheat��msΪ��λ
												case 1:limheat[Bullet17]=180;limheat[Bullet42]=160;break;
												case 2:limheat[Bullet17]=270;limheat[Bullet42]=320;break;
												default:break;
											}
	 										//17����
											if(limheat[Bullet17] < heat0)
												allownum[Bullet17]=0;
											else
											{
												limshootnum[Bullet17]=(limheat[Bullet17]-heat0)/rlspd;//���ɷ�����
												allownum[Bullet17]=limshootnum[Bullet17]/max_freq[Bullet17];//��������
											}
											//42����
											if(limheat[Bullet42] < heat0)
												allownum[Bullet42]=0;
											else
											{
												limshootnum[Bullet42]=(limheat[Bullet42]-heat0)/rlspd;//���ɷ�����
												allownum[Bullet42]=limshootnum[Bullet42]/max_freq[Bullet42];//��������
											}
											break;
			default:break;
		}
		return	allownum[Bullet17];		
}
