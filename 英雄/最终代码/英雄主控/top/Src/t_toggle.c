#include "t_toggle.h"
#include "m_toggle.h"
#include  "stdint.h"
 short shootnum=0;
unsigned short toggle_heat_ctr(uint8_t rlgrade,float rlspd,uint8_t bulletfreq,uint8_t bullettype,uint16_t heat1)
{ 
		int limshootnum[2];
		int allownum[2];
		float limheat[2];
		//float max_freq[2]={2,2};
		static float res_spd=15;

		switch(rlgrade)
		{
				case 0:limheat[Bullet42]=80; break;//rtheat��msΪ��λ
				case 1:limheat[Bullet42]=160;break;
				case 2:limheat[Bullet42]=320;break;
				default:break;
		}
		//42����
		if(limheat[Bullet42] < heat1)
			allownum[Bullet42]=0;
		else
		{
			if(rlspd)
				res_spd=rlspd;
			rlspd=res_spd;
			limshootnum[Bullet42]=(limheat[Bullet42]-heat1)/rlspd;//���ɷ�����
			if(limshootnum[Bullet42]<1)
				allownum[Bullet42]=0;
			else
				allownum[Bullet42]=1;
				
		}
		return	allownum[Bullet42];		
}
