/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define __debug_111_			0

#define AP_APIB_DATA0_PORT		GPIOA
#define AP_APIB_DATA0_PIN		GPIO_PIN_10

#define AP_APIB_DATA1_PORT		GPIOA
#define AP_APIB_DATA1_PIN		GPIO_PIN_9

#define AP_APIB_DATA2_PORT		GPIOA
#define AP_APIB_DATA2_PIN		GPIO_PIN_8

#define AP_APIB_DATA3_PORT		GPIOB
#define AP_APIB_DATA3_PIN		GPIO_PIN_15

#define AP_APIB_DATA4_PORT		GPIOB
#define AP_APIB_DATA4_PIN		GPIO_PIN_14

#define AP_APIB_DATA5_PORT		GPIOB
#define AP_APIB_DATA5_PIN		GPIO_PIN_13

#define AP_APIB_DATA6_PORT		GPIOB
#define AP_APIB_DATA6_PIN		GPIO_PIN_12

#define AP_APIB_DATA7_PORT		GPIOB
#define AP_APIB_DATA7_PIN		GPIO_PIN_11

#define AP_APIB_BUFF_OE_PORT	GPIOB
#define AP_APIB_BUFF_OE_PIN		GPIO_PIN_10

#define AP_APIB_BUFF_DIR_PORT	GPIOB
#define AP_APIB_BUFF_DIR_PIN	GPIO_PIN_1

#define AP_APIB_STAT_ATT_PORT	GPIOB
#define AP_APIB_STAT_ATT_PIN	GPIO_PIN_0

#define AP_APIB_STAT_AUX_PORT	GPIOA
#define AP_APIB_STAT_AUX_PIN	GPIO_PIN_7

#define AP_APIB_STAT_RES_PORT	GPIOA
#define AP_APIB_STAT_RES_PIN	GPIO_PIN_6

#define AP_APIB_STAT_PWR_PORT	GPIOA
#define AP_APIB_STAT_PWR_PIN	GPIO_PIN_5

#define AP_APIB_STAT_R_PORT		GPIOA
#define AP_APIB_STAT_R_PIN		GPIO_PIN_4

#define AP_APIB_STAT_W_PORT		GPIOA
#define AP_APIB_STAT_W_PIN		GPIO_PIN_3

#define AP_APIB_STAT_A_PORT		GPIOA
#define AP_APIB_STAT_A_PIN		GPIO_PIN_2

#define AP_APIB_STAT_IRQ_PORT	GPIOA
#define AP_APIB_STAT_IRQ_PIN	GPIO_PIN_1

#define AP_APIB_STAT_OE_PORT	GPIOA
#define AP_APIB_STAT_OE_PIN		GPIO_PIN_0

#define AP_APIB_STAT_DIR_PORT	GPIOC
#define AP_APIB_STAT_DIR_PIN	GPIO_PIN_15

#define AP_APIB_STAT_APIB_ACTIVE_PORT	GPIOC
#define AP_APIB_STAT_APIB_ACTIVE_PIN	GPIO_PIN_14

#define if_delay				100 //delay for hw pins


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//uint8_t		ap_com_buff[512];
//uint32_t	ap_com_buff_len;
//uint32_t	ap_com_buff_pos;

char rx_buff[512];
char tx_buff[512];
_Bool rx_ready=0;
uint32_t rx_data_len=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

_Bool Check_Signature(char* buff,uint32_t len);
void AP_APIB_DATA_PORT_INPUT();
void AP_APIB_DATA_PORT_OUTPUT();
void AP_APIB_SetDataToHWPin(char data);
char AP_APIB_GetDataFromHWPin();
void AP_APIB_DELAY(int us);
void AP_APIB_WriteByte(char address,char data);
void AP_APIB_WriteWord(char address,char data_hi, char data_lo);
char AP_APIB_ReadByte(char address);
void AP_APIB_WriteArray(char *pack);
void AP_APIB_WriteTriple(char *);
void AP_APIB_ReadTriple(char* buff);
_Bool AP_APIB_CheckATT(char);
void AP_APIB_Check_Fail();

// low level
char AP_APIB_R_Pulse();
void AP_APIB_W_Pulse();
void AP_APIB_A_Pulse();

void AP_APIB_DataWrite(char data);
char AP_APIB_DataRead();

void AP_APIB_Reset_AP();
_Bool AP_APIB_Read_AUX();
_Bool AP_APIB_Read_ATT();
_Bool AP_APIB_Read_PWR();
_Bool AP_APIB_Read_IRQ();

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /*
   * uint8_t		ap_com_buff[4096];
	uint32_t	ap_com_buff_len;
	uint32_t	ap_com_buff_pos;
   */

  /* Package struct:
   *
   * 0 - symbol *
   * 1 - command 0-255
   * 2 - symbol *
   * 3 - address 0-255
   * 4 - symbol *
   * 5,6 - package size 2 byte 0-65536
   * 7 - symbol *
   * 8-... - data package
   * n+9 - symbol *
   *
   */

  while (1)
  {
    /* USER CODE END WHILE */
	  if(rx_ready)
	  {
		  rx_ready=0;
		  if(__debug_111_)
		  {
			  //printf("RX Actions\n");
		  };
		  if(rx_data_len>0 && rx_data_len<512)
		  {
			  //CDC_Transmit_FS(rx_buff,rx_data_len);
			  // command receive

			  // check signature
			  if(rx_data_len>=4)
			  {
				  if(rx_buff[0]=='a')
				  {
					  if(rx_buff[1]=='p')
					  {
						  if(rx_buff[2]=='i')
						  {
							  if(rx_buff[3]=='b')
							  {
								  // send id message
								  tx_buff[0]='u';
								  tx_buff[1]='s';
								  tx_buff[2]='b';
								  CDC_Transmit_FS(tx_buff,3);
							  }
						  }
					  }
				  }

				  HAL_GPIO_WritePin(AP_APIB_STAT_APIB_ACTIVE_PORT, AP_APIB_STAT_APIB_ACTIVE_PIN,1);

				  //check signature
				  if(rx_buff[0]=='<')
				  {
					  if(rx_buff[1]=='w')
					  {
						  if(rx_buff[2]=='>')
						  {
							  switch(rx_buff[3])
							  {
								  case '0':
								  {
									  // check ATT
									  tx_buff[0]='<';
									  tx_buff[1]='r';
									  tx_buff[2]='>';
									  tx_buff[3]='0';
									  if(AP_APIB_CheckATT)
									  {
										  tx_buff[4]=1;
									  }
									  else
									  {
										  tx_buff[4]=0;
									  }
									  CDC_Transmit_FS(tx_buff,8);
								  }break;

								  case '1':
								  {
									  // write byte
									  AP_APIB_WriteByte(rx_buff[4],rx_buff[5]);
									  tx_buff[0]='<';
									  tx_buff[1]='r';
									  tx_buff[2]='>';
									  tx_buff[3]='1';
									  tx_buff[4]=rx_buff[5];
									  CDC_Transmit_FS(tx_buff,8);

								  }break;

								  case '2':
								  {
									  //write word
									  AP_APIB_WriteByte(rx_buff[4],rx_buff[5]);
									  AP_APIB_WriteByte(rx_buff[4],rx_buff[6]);
									  tx_buff[0]='<';
									  tx_buff[1]='r';
									  tx_buff[2]='>';
									  tx_buff[3]='2';
									  tx_buff[4]=rx_buff[6];
									  CDC_Transmit_FS(tx_buff,8);
								  }break;

								  case '3':
								  {
									  // write tripple
									  AP_APIB_WriteByte(rx_buff[4],rx_buff[5]);
									  AP_APIB_WriteByte(rx_buff[4],rx_buff[6]);
									  AP_APIB_WriteByte(rx_buff[4],rx_buff[7]);
									  tx_buff[0]='<';
									  tx_buff[1]='r';
									  tx_buff[2]='>';
									  tx_buff[3]='3';
									  tx_buff[4]=rx_buff[7];
									  CDC_Transmit_FS(tx_buff,8);
								  }break;

								  case '4':
								  {
									  //read byte
									  tx_buff[0]='<';
									  tx_buff[1]='r';
									  tx_buff[2]='>';
									  tx_buff[3]='4';
									  tx_buff[4]=AP_APIB_ReadByte(rx_buff[4]);
									  CDC_Transmit_FS(tx_buff,8);
								  }break;
								  case '5':
								  {
									  //read word
									  tx_buff[0]='<';
									  tx_buff[1]='r';
									  tx_buff[2]='>';
									  tx_buff[3]='5';
									  tx_buff[4]=AP_APIB_ReadByte(rx_buff[4]);
									  tx_buff[5]=AP_APIB_ReadByte(rx_buff[4]+1);
									  CDC_Transmit_FS(tx_buff,8);

								  }break;
								  case '6':
								  {
									  //read tripple

									  tx_buff[0]='<';
									  tx_buff[1]='r';
									  tx_buff[2]='>';
									  tx_buff[3]='6';
									  tx_buff[4]=AP_APIB_ReadByte(rx_buff[4]);
									  tx_buff[5]=AP_APIB_ReadByte(rx_buff[4]+1);
									  tx_buff[6]=AP_APIB_ReadByte(rx_buff[4]+2);
									  CDC_Transmit_FS(tx_buff,8);
								  }break;
								  case '7':
								  {
									  //Ap reset
									  AP_APIB_Reset_AP();
									  tx_buff[0]='<';
									  tx_buff[1]='r';
									  tx_buff[2]='>';
									  tx_buff[3]='7';
									  CDC_Transmit_FS(tx_buff,8);
								  }break;
								  case '8':
								  {
									  // check ATT
									 tx_buff[0]='<';
									 tx_buff[1]='r';
									 tx_buff[2]='>';
									 tx_buff[3]='8';
									 if(AP_APIB_Read_IRQ())
									 {
									 	  tx_buff[4]=1;
									 }
									 else
									 {
										 tx_buff[4]=0;
									 }
									 CDC_Transmit_FS(tx_buff,8);
								  }break;
								  case '9':
								  {

								  }break;
								  case 'a':
								  {

								  }break;

						  	  }
						  }
					  }
				  }
				  HAL_GPIO_WritePin(AP_APIB_STAT_APIB_ACTIVE_PORT, AP_APIB_STAT_APIB_ACTIVE_PIN,0);
			  }
		  }
	  }
  }
}

char AP_APIB_GetDataFromHWPin()
{
	char result=0;
	//AP_APIB_DATA_PORT_INPUT();
	result=result+HAL_GPIO_ReadPin(AP_APIB_DATA0_PORT, AP_APIB_DATA0_PIN);
	result=result+2*HAL_GPIO_ReadPin(AP_APIB_DATA1_PORT, AP_APIB_DATA1_PIN);
	result=result+4*HAL_GPIO_ReadPin(AP_APIB_DATA2_PORT, AP_APIB_DATA2_PIN);
	result=result+8*HAL_GPIO_ReadPin(AP_APIB_DATA3_PORT, AP_APIB_DATA3_PIN);
	result=result+16*HAL_GPIO_ReadPin(AP_APIB_DATA4_PORT, AP_APIB_DATA4_PIN);
	result=result+32*HAL_GPIO_ReadPin(AP_APIB_DATA5_PORT, AP_APIB_DATA5_PIN);
	result=result+64*HAL_GPIO_ReadPin(AP_APIB_DATA6_PORT, AP_APIB_DATA6_PIN);
	result=result+128*HAL_GPIO_ReadPin(AP_APIB_DATA7_PORT, AP_APIB_DATA7_PIN);
	return result;
}

void AP_APIB_SetDataToHWPin(char data)
{
	HAL_GPIO_WritePin(AP_APIB_BUFF_DIR_PORT,AP_APIB_BUFF_DIR_PIN,0); // B->A
	AP_APIB_DATA_PORT_OUTPUT();
	HAL_GPIO_WritePin(AP_APIB_DATA0_PORT, AP_APIB_DATA0_PIN,(1<<0) & data);
	HAL_GPIO_WritePin(AP_APIB_DATA1_PORT, AP_APIB_DATA1_PIN,(1<<1) & data);
	HAL_GPIO_WritePin(AP_APIB_DATA2_PORT, AP_APIB_DATA2_PIN,(1<<2) & data);
	HAL_GPIO_WritePin(AP_APIB_DATA3_PORT, AP_APIB_DATA3_PIN,(1<<3) & data);
	HAL_GPIO_WritePin(AP_APIB_DATA4_PORT, AP_APIB_DATA4_PIN,(1<<4) & data);
	HAL_GPIO_WritePin(AP_APIB_DATA5_PORT, AP_APIB_DATA5_PIN,(1<<5) & data);
	HAL_GPIO_WritePin(AP_APIB_DATA6_PORT, AP_APIB_DATA6_PIN,(1<<6) & data);
	HAL_GPIO_WritePin(AP_APIB_DATA7_PORT, AP_APIB_DATA7_PIN,(1<<7) & data);

}

void AP_APIB_Check_Fail()
{
	if(Check_Signature(rx_buff,rx_data_len))
		{
			char data=HAL_GPIO_ReadPin(AP_APIB_STAT_IRQ_PORT,AP_APIB_STAT_IRQ_PIN);

			tx_buff[0]='*';
			tx_buff[1]='a';
			tx_buff[2]='*';
			tx_buff[3]=0;
			tx_buff[4]='*';
			tx_buff[5]=0;
			tx_buff[6]=1;
			tx_buff[7]='*';
			tx_buff[8]=data;
			//tx_buff[8]=1;
			tx_buff[9]='*';
			CDC_Transmit_FS(tx_buff,10);
		}
}

_Bool AP_APIB_CheckATT(char address)
{
	AP_APIB_SetDataToHWPin(address);
	AP_APIB_A_Pulse();
	AP_APIB_A_Pulse();
	AP_APIB_A_Pulse();
	if(HAL_GPIO_ReadPin(AP_APIB_STAT_ATT_PORT,AP_APIB_STAT_ATT_PIN)==GPIO_PIN_SET)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void AP_APIB_ReadTriple(char* buff)
{
	char data_hi,data_lo;

	for(int i=0;i<3;i++)
	{
		AP_APIB_DataWrite(buff[3]+i);
		AP_APIB_A_Pulse();
		tx_buff[8+i]=AP_APIB_R_Pulse();
	}

	tx_buff[0]='*';
	tx_buff[1]='1';
	tx_buff[2]='*';
	tx_buff[3]=rx_buff[3];
	tx_buff[4]='*';
	tx_buff[5]=0;
	tx_buff[6]=2;
	tx_buff[7]='*';
	tx_buff[11]='*';

	CDC_Transmit_FS(tx_buff,12);
}

void AP_APIB_WriteTriple(char *pack)
{
	for(int i=0;i<3;i++)
	{
		AP_APIB_DataWrite(pack[3+i]);
		AP_APIB_A_Pulse();
		AP_APIB_DataWrite(pack[8+i]);
		AP_APIB_W_Pulse();
	}
}

void AP_APIB_WriteArray(char *pack)
{
	// ?
	for(int i=0;i<pack[5]*255+pack[6];i++)
	{
		AP_APIB_DataWrite(pack[8+i]);
		AP_APIB_W_Pulse();
	}

}

void AP_APIB_WriteByte(char address,char data)
{
	AP_APIB_DataWrite(address);
	AP_APIB_A_Pulse();
	AP_APIB_DataWrite(data);
	AP_APIB_W_Pulse();
}

void AP_APIB_WriteWord(char address,char data_hi, char data_lo)
{
	AP_APIB_WriteByte(address,data_lo);
	AP_APIB_WriteByte(address+1,data_hi);
}

void AP_APIB_WriteAddress(char address)
{
	AP_APIB_DataWrite(address);
	AP_APIB_A_Pulse();
}

void AP_APIB_DELAY(int us)
{
	for(int i=0;i<us;i++)
	{
		__NOP();
	}
}


char AP_APIB_ReadByte(char address)
{
	char data=0;
	AP_APIB_DataWrite(address);
	AP_APIB_A_Pulse();
	data=AP_APIB_R_Pulse();
	return data;
}

_Bool Check_Signature(char* buff,uint32_t len)
{
	if(len>5)
		{
			if(buff[0]=='*')
			{
				if(buff[2]=='*')
				{
					if(buff[4]=='*')
					{
						return 1;
					}
				}
			}
		}
		return 0;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void AP_APIB_DATA_PORT_INPUT()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = AP_APIB_DATA0_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA0_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA3_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA4_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA5_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA5_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA6_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA6_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA7_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA7_PORT, &GPIO_InitStruct);

}

void AP_APIB_DATA_PORT_OUTPUT()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = AP_APIB_DATA0_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA0_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA3_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA4_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA5_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA5_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA6_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA6_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AP_APIB_DATA7_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AP_APIB_DATA7_PORT, &GPIO_InitStruct);

}


static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();




  //inputs

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = AP_APIB_STAT_ATT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_STAT_ATT_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AP_APIB_STAT_AUX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_STAT_AUX_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AP_APIB_STAT_PWR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_STAT_PWR_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AP_APIB_STAT_IRQ_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_STAT_IRQ_PORT, &GPIO_InitStruct);

  //outputs



  GPIO_InitStruct.Pin = AP_APIB_STAT_APIB_ACTIVE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_STAT_APIB_ACTIVE_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AP_APIB_BUFF_OE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_BUFF_OE_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AP_APIB_BUFF_DIR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_BUFF_DIR_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AP_APIB_STAT_OE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_STAT_OE_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AP_APIB_STAT_DIR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_STAT_DIR_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AP_APIB_STAT_R_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_STAT_R_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AP_APIB_STAT_A_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_STAT_A_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AP_APIB_STAT_W_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_STAT_W_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AP_APIB_STAT_RES_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(AP_APIB_STAT_RES_PORT, &GPIO_InitStruct);

}

void AP_APIB_DataWrite(char data)
{
	HAL_GPIO_WritePin(AP_APIB_BUFF_DIR_PORT,AP_APIB_BUFF_DIR_PIN,0); // B->A
	AP_APIB_DELAY(if_delay);
	AP_APIB_DATA_PORT_OUTPUT();
	AP_APIB_DELAY(if_delay);
	AP_APIB_SetDataToHWPin(data);
	AP_APIB_DELAY(if_delay);
}

char AP_APIB_DataRead()
{
	HAL_GPIO_WritePin(AP_APIB_BUFF_DIR_PORT,AP_APIB_BUFF_DIR_PIN,1); // A->B
	AP_APIB_DELAY(if_delay);
	AP_APIB_DATA_PORT_INPUT();
	AP_APIB_DELAY(if_delay);
	return AP_APIB_GetDataFromHWPin();
}

char AP_APIB_R_Pulse()
{
	char result=0;
	HAL_GPIO_WritePin(AP_APIB_BUFF_DIR_PORT,AP_APIB_BUFF_DIR_PIN,1); // A->B
	AP_APIB_DELAY(if_delay);
	HAL_GPIO_WritePin(AP_APIB_STAT_R_PORT,AP_APIB_STAT_R_PIN,1);
	AP_APIB_DELAY(if_delay);
	result=AP_APIB_GetDataFromHWPin();
	AP_APIB_DELAY(if_delay);
	HAL_GPIO_WritePin(AP_APIB_STAT_R_PORT,AP_APIB_STAT_R_PIN,0);
	AP_APIB_DELAY(if_delay);
	return result;
}

void AP_APIB_A_Pulse()
{
	AP_APIB_DELAY(if_delay);
	HAL_GPIO_WritePin(AP_APIB_STAT_A_PORT,AP_APIB_STAT_A_PIN,1);
	AP_APIB_DELAY(if_delay);
	HAL_GPIO_WritePin(AP_APIB_STAT_A_PORT,AP_APIB_STAT_A_PIN,0);
	AP_APIB_DELAY(if_delay);
}

void AP_APIB_W_Pulse()
{
	AP_APIB_DELAY(if_delay);
	HAL_GPIO_WritePin(AP_APIB_STAT_W_PORT,AP_APIB_STAT_W_PIN,1);
	AP_APIB_DELAY(if_delay);
	HAL_GPIO_WritePin(AP_APIB_STAT_W_PORT,AP_APIB_STAT_W_PIN,0);
	AP_APIB_DELAY(if_delay);
}

_Bool AP_APIB_Read_IRQ()
{

	if(HAL_GPIO_ReadPin(AP_APIB_STAT_IRQ_PORT,AP_APIB_STAT_IRQ_PIN))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

_Bool AP_APIB_Read_PWR()
{
	return HAL_GPIO_ReadPin(AP_APIB_STAT_PWR_PORT,AP_APIB_STAT_PWR_PIN);
}

_Bool AP_APIB_Read_ATT()
{
	return HAL_GPIO_ReadPin(AP_APIB_STAT_ATT_PORT,AP_APIB_STAT_ATT_PIN);
}

_Bool AP_APIB_Read_AUX()
{
	return HAL_GPIO_ReadPin(AP_APIB_STAT_AUX_PORT,AP_APIB_STAT_AUX_PIN);
}

void AP_APIB_Reset_AP()
{
	HAL_GPIO_WritePin(AP_APIB_STAT_RES_PORT,AP_APIB_STAT_RES_PIN,1);
	AP_APIB_DELAY(if_delay*10);
	HAL_GPIO_WritePin(AP_APIB_STAT_RES_PORT,AP_APIB_STAT_RES_PIN,0);
}
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
