
#include <main.h>
#include "Libraries/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_gpio.h"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_sai.h>
#include <misc.h>


static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);
void My_GPIO_Init();
void My_SAI_Init();
int main(void)
{
  
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
	My_GPIO_Init();
    
  /* Infinite loop */
  while (1)
  {
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		Delay(300);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
		Delay(300);
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}
void My_GPIO_Init(){
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Output HSE clock on MCO1 pin(PA8) ****************************************/ 
  /* Enable the GPIOA peripheral */ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  	/*Configure GPIO Output*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  /* Configure MCO1 pin(PA8) in alternate function */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//    
//  /* HSE clock selected to output on MCO1 pin(PA8)*/
//  RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
//  
//  
//  /* Output SYSCLK/4 clock on MCO2 pin(PC9) ***********************************/ 
//  /* Enable the GPIOACperipheral */ 
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//  
//  /* Configure MCO2 pin(PC9) in alternate function */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
    
  /* SYSCLK/4 clock selected to output on MCO2 pin(PC9)*/
 // RCC_MCO2Config(RCC_MCO2Source_SYSCLK, RCC_MCO2Div_4);

}
void My_SAI_Init(){
  SAI_InitTypeDef SAI_Init;
	SAI_Init.SAI_AudioMode=SAI_Mode_MasterRx;
	SAI_Init.SAI_Protocol=SAI_Free_Protocol;
	SAI_Init.SAI_DataSize=SAI_DataSize_32b;
	SAI_Init.SAI_FirstBit=SAI_FirstBit_MSB;
	SAI_Init.SAI_ClockStrobing=SAI_ClockStrobing_FallingEdge;
	SAI_Init.SAI_Synchro=SAI_Synchronous;
	SAI_Init.SAI_OUTDRIV=SAI_OutputDrive_Disabled;
	SAI_Init.SAI_FIFOThreshold=SAI_FIFOThreshold_1QuarterFull;

	
  
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
