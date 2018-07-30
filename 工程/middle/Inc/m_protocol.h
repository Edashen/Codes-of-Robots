#ifndef __M_PROTOCOL_H
#define __M_PROTOCOL_H

/**����ϵͳͨ��Э��**/
#include "usart.h"
#include "mytype.h"

/********************  ��������  ***************************/
//0-3 bits
#define Strike_0   0x00 //0��װ���棨ǰ��
#define Strike_1   0x01 //1��װ���棨��
#define Strike_2   0x02 //2��װ���棨��
#define Strike_3   0x03 //3��װ���棨�ң�
#define Strike_4   0x04 //4��װ���棨��1��
#define Strike_5   0x05 //5��װ���棨��2��
//4-7 bits
#define BC_Strike          0x00 //װ���˺����ܵ������� BloodChanged    BC
#define BC_ModuleOffline   0x01 //ģ�����߿�Ѫ
#define BC_ShootSpeed      0x02 //�ӵ����ٿ�Ѫ
#define BC_ShootFreq       0x03 //�ӵ���Ƶ��Ѫ
#define BC_ShootQLimit     0x04 //ǹ����������
#define BC_CmPLimit        0x05 //���̹��ʳ���

#define BC_CommonFoul      0x06 //��ͨ�����Ѫ
#define BC_Tarmac          0x0a //ͣ��ƺ��Ѫ
#define BC_EngineerOuto    0x0b //���̻������Զ���Ѫ


//����������״̬��0x0001��
typedef __packed struct
{
	uint8_t validFlag;//λ�ýǶ���Ϣ��Ч��־λ
	float x;
	float y;
	float z;
	float yaw;//ǹ�ڳ���Ƕ�ֵ
}position_t;

typedef __packed struct
{
	uint16_t stageRemianTime;//��ǰʣ���ʱ��
	uint8_t  gameProgress; //��ǰ�����׶�
	uint8_t  robotLevel;//�����˵ȼ�
	uint16_t remainHP;//�����˵�ǰѪ��
	uint16_t maxHP;//��������Ѫ��
  position_t position;
}extGameRobotState_t;

typedef union
{
  extGameRobotState_t extGameRobotState;
  uint8_t extGameRobotState_recvbuff[34];
}extGameRobotState_recvtype;

//�˺����ݣ�0x0002��
typedef __packed struct
{
	uint8_t armorType:4;//������ʽ�����ڽṹ�������Ķ����У���λ����ı�׼��ʽ����armorType��λ������4λ�ǻᱻ�ضϡ�
	uint8_t hurtType:4;
}exRobotHurt_t;

//ʵʱ�����Ϣ��0x0003��

typedef __packed struct
{
	uint8_t bulletType;//��������
	uint8_t bulletFreq;//������Ƶ
	float bulletSpeed;//��������
	float reseved;    //����
}extShootData_t;

//ʵʱ�����������ݣ�0x004��
typedef __packed struct
{
	  float chassisVolt;//���������ѹ
	  float chassisCurrent;//�����������
	  float chassisPower;//�����������
	  uint16_t shooterHeat0;//17mmǹ������
	  uint16_t shooterHeat1;//42mmǹ������
}extPowerHeatData_t;

typedef union
{
    extPowerHeatData_t extPowerHeatData;//��������
	  uint8_t extPowerHeatData_recvbuff[20];
}extPowerHeatData_recvtype;

//���ؽ������ݣ�0x0005��
typedef __packed struct
{
	uint8_t cardType;//������
	uint8_t cardldx;//�������ţ��������ֲ�ͬ����
}extRifddDetect_t;

//����ʤ�����ݣ�0x006��
typedef __packed struct
{
	uint8_t winner;//ʤ��
}extGameResult_t;

//Buff��ȡ���ݣ�0x0007��
typedef __packed struct
{
	uint8_t buffType;//Buff���ͣ������ӳɡ������ӳɡ���ô���������
	uint8_t buffAddition;//�ӳɰٷֱȣ���%Ϊ��λ
}extGetBuff_t;

//�������Զ�������
typedef __packed struct
{
	float data1;
	float data2;
	float data3;
	uint8_t mask;
}extShowData_t;



typedef __packed struct
{
	uint8_t SOF;          //������ʼ�ֽڣ��̶�Ϊ0xA5          
	uint16_t DataLength;  //���ݳ���
	uint8_t Seq;          //�����
	uint8_t CRC8;         //֡ͷCRCУ��
}FrameHeader_t;//֡ͷ

typedef enum //����ǰ�˳������
{
	extGameRobotState = 0x0001,   
	exRobotHurt, 
	extShootData,            
	extPowerHeatData, 
	extRifddDetect,
	extGameResult,
	extGetBuff,
	extShowData = 0x0100
}CmdID_t;

typedef __packed struct
{
	FrameHeader_t    FrameHeader;
	CmdID_t          CmdID;
	__packed union 
	{
		extGameRobotState_t       extGameRobotState;        //����������Ϣ
		extPowerHeatData_t        extPowerHeatData;       //ʵʱ������ѹ��
		extShootData_t            extShootData;       //ʵʱ�����Ϣ (0��0003)
		exRobotHurt_t             exRobotHurt;//ʵʱ��Ѫ��Ϣ
		extRifddDetect_t          extRifddDetect;
		extGameResult_t           extGameResult;
		extGetBuff_t              extGetBuff;
		extShowData_t             extShowData; //�Զ������� 
	}Data;
	uint16_t        CRC16;         //֮ǰ��������CRCУ��   ע������ݺ�֮ǰ�����ݿ��ܲ����������Բ�Ҫֱ��ʹ�ã�����Ҫֱ��ʹ�ã������ڴ˸�ֵ
}Frame_t;  //����֡


extern Frame_t* FrameData;//����֡����ָ����������������Ϣ

extern int DataAnalysis(uint8_t *pData);

#endif




