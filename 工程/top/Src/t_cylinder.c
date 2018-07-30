#include "t_cylinder.h"
#include "gpio.h"

void Cylinder_ctr(bool cyflag)
{	
	if(cyflag)
	{	
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_SET);	//����1���	
		HAL_Delay(CYTIME);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);	//����2���	
		HAL_Delay(CYTIME);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET);	//����1����
		HAL_Delay(CYTIME);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);	//����3���

		HAL_Delay(1000);

		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_RESET);	//����3����
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_SET);	//����1���	
		HAL_Delay(CYTIME);
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);	//����2����	
		HAL_Delay(1000);
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET);	//����1����
	}
}

