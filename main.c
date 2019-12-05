
#include <main.h>
#include "Libraries/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_gpio.h"
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_sai.h>
#include <misc.h>


static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;
uint8_t clk;
/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);
void My_GPIO_Init(void);
void My_SAI_Init(void);
void HSE_config(void);
int main(void) 
{
  
  /* SysTick end of count event each 10ms */
	HSE_config();
	SystemCoreClockUpdate();
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
	RCC_GetClocksFreq(&RCC_Clocks);
	clk=RCC_GetSYSCLKSource();
	My_GPIO_Init();
	My_SAI_Init();
    
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
void HSE_config(void)
{    
  /* Deinit the RCC config, this function must be call when config PLL*/
    RCC_DeInit();
    /*Turn on HSE*/
    RCC_HSEConfig(RCC_HSE_ON);
    /*Wait HSE stable*/
    RCC_WaitForHSEStartUp();
 
      
    /*Config level of voltage to accord with frequency*/    
    PWR_MainRegulatorModeConfig(PWR_Regulator_Voltage_Scale1);
      
    /*AHB (HCLK) Config*/
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    /* APB1 (PCLK1) Config*/
    RCC_PCLK1Config(RCC_HCLK_Div4);
    /* APB2 (PCLK2) Config*/
    RCC_PCLK2Config(RCC_HCLK_Div2);
    /*Config PLL : If want to use this function, we must use RCC_Deinit() before because
    *PLL just config one time and it configed in SetSysClock() 
    */
    RCC_PLLConfig(RCC_PLLSource_HSE,4,168,2,7);
    /*Enable PLL*/
    RCC_PLLCmd(ENABLE);
    /* Wait PLL state is stable*/
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET){}
    
      /*Config the latency of flash memory */
    FLASH_PrefetchBufferCmd(ENABLE);
    FLASH_SetLatency(FLASH_Latency_5);

    /*Choose PLL as system clock*/
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /*Wait for CPU choose the source as HSE*/
    while(RCC_GetSYSCLKSource()!= 0x08) {}
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
void My_GPIO_Init(void){
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
void My_SAI_Init(void){
	RCC_AHB2PeriphClockCmd(RCC_APB2Periph_SAI1,ENABLE);
	
  SAI_InitTypeDef MSAI_Init;
	
	MSAI_Init.SAI_AudioMode=SAI_Mode_MasterTx;
	MSAI_Init.SAI_Protocol=SAI_Free_Protocol;
	MSAI_Init.SAI_DataSize=SAI_DataSize_32b;
	MSAI_Init.SAI_FirstBit=SAI_FirstBit_MSB;
	MSAI_Init.SAI_ClockStrobing=SAI_ClockStrobing_FallingEdge;
	MSAI_Init.SAI_Synchro=SAI_Asynchronous;
	MSAI_Init.SAI_SynchroExt=SAI_SyncExt_OutBlockA_Enable;
	MSAI_Init.SAI_OUTDRIV=SAI_OutputDrive_Disabled;
	MSAI_Init.SAI_FIFOThreshold=SAI_FIFOThreshold_1QuarterFull;
	SAI_Init(SAI1_Block_A,&MSAI_Init);
	//Configuration Frame
	SAI_FrameInitTypeDef MSAI_Frame;
	MSAI_Frame.SAI_FrameLength=256;					//chua biet
	MSAI_Frame.SAI_ActiveFrameLength=128;	//chua biet
	MSAI_Frame.SAI_FSDefinition=SAI_FS_StartFrame;
	MSAI_Frame.SAI_FSPolarity=SAI_FS_ActiveLow;
	MSAI_Frame.SAI_FSOffset=SAI_FS_FirstBit;
	SAI_FrameInit(SAI1_Block_A,&MSAI_Frame);
	
  
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
