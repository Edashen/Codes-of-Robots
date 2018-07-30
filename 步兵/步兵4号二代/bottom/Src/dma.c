#include "dma.h"


DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart6_rx;

//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_CHANNEL_0~DMA_CHANNEL_7
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx)
{ 
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
        __HAL_RCC_DMA2_CLK_ENABLE();//DMA2ʱ��ʹ��	
	}else 
	{
        __HAL_RCC_DMA1_CLK_ENABLE();//DMA1ʱ��ʹ�� 
	}
    
    __HAL_LINKDMA(&huart1,hdmarx,hdma_usart1_rx);    //��DMA��USART1��ϵ����(����DMA)

	
	 //Rx DMA����   �����ȼ��ǵ͵����ȼ��������е����ȼ������������óɺ�����ԭ����̳��������ͬ��ģʽ��
    hdma_usart1_rx.Instance=DMA_Streamx;                            //������ѡ�� DMA2_Stream2   ����2��DMA1_Stream5
    hdma_usart1_rx.Init.Channel=chx;                                //ͨ��ѡ�� DMA_CHANNEL_4    ����2��DMA_CHANNEL_4
    hdma_usart1_rx.Init.Direction=DMA_PERIPH_TO_MEMORY;             //���赽�洢��
    hdma_usart1_rx.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ
    hdma_usart1_rx.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ
    hdma_usart1_rx.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
    hdma_usart1_rx.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
    hdma_usart1_rx.Init.Mode=DMA_CIRCULAR;                          //ѭ��ģʽ
    hdma_usart1_rx.Init.Priority=DMA_PRIORITY_VERY_HIGH;            //�ǳ��ߵ����ȼ�
    hdma_usart1_rx.Init.FIFOMode=DMA_FIFOMODE_DISABLE;              
    hdma_usart1_rx.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_1QUARTERFULL;     
    hdma_usart1_rx.Init.MemBurst=DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
    hdma_usart1_rx.Init.PeriphBurst=DMA_PBURST_SINGLE;              //����ͻ�����δ���
	  if(HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }
		 
    HAL_DMA_DeInit(&hdma_usart1_rx);   
    HAL_DMA_Init(&hdma_usart1_rx);
		
	  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
		
} 


//����һ��DMA����
//huart:���ھ��
//pData�����������ָ��
//Size:�����������
void MYDMA_USART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    HAL_DMA_Start(huart->hdmatx, (u32)pData, (uint32_t)&huart->Instance->DR, Size);//����DMA����
    
    huart->Instance->CR3 |= USART_CR3_DMAT;//ʹ�ܴ���DMA����
}	  

void DMA2_Stream2_IRQHandler(void)
{

  HAL_DMA_IRQHandler(&hdma_usart1_rx);
	
} 

////����һ��DMA����
////DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
////ndtr:���ݴ�����  
//void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
//{
// 
//	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA���� 
//	
//	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����  
//		
//	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //���ݴ�����  
// 
//	DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA���� 
//}	  



void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
