/* USER CODE BEGIN Header */
/**
  **************************
  * @file           : main.c
  * @brief          : Main program body
  **************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "u8g2.h"

/* USER CODE END Includes */
/* USER CODE BEGIN Header */
/**
  **********
  * @file           : main.c
  * @brief          : Main program body
  **********
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "u8g2.h"
#include <stdbool.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
u8g2_t u8g2;

//Variáveis para o display:
int ii = 0;
int i = 0;
int item_selecionado = 0; // 0 = acumulada; 1 = gas; 2 = configuracoes. || 0 max; 1 min; 2 voltar.
int gas = 0;
unsigned long select_tempo = 0;
bool select_pressionado = false;
int currentPage = 1;
bool editMax = false;
bool editMin = false;
unsigned long blink = 0;

//Variáveis lógica
float valorMax;
float valorMin;

//BITMAPS:
static unsigned char bola[9][20] = {
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x02, 0x01, 0x84, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x02, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0xff, 0x03, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0x01, 0x02, 0x01, 0x02, 0xff, 0x03, 0xff, 0x03, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0x01, 0x02, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0x02, 0x01, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0x84, 0x00, 0xfe, 0x01, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00},
   {0x78, 0x00, 0xfc, 0x00, 0xfe, 0x01, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03, 0xfe, 0x01, 0xfc, 0x00, 0x78, 0x00}};

static unsigned char acumulada_selecao[] = {
   0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x0f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x30, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f };

static unsigned char co2_selecao[] = {
   0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x02, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x60, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x60, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xfe, 0xff,
   0xff, 0xff, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f };

static unsigned char caixa_selecao[] = {
   0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x02,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x18, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x18, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x0f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07 };

static unsigned char labsolda[] = {
   0xe0, 0x03, 0xf0, 0x07, 0x78, 0x08, 0x3f, 0x71, 0x21, 0x43, 0x61, 0x46,
   0xc1, 0x44, 0x87, 0x7c, 0x08, 0x0e, 0xf0, 0x07, 0xe0, 0x03 };

static unsigned char seta[] = {
   0x0c, 0x06, 0x7f, 0x06, 0x0c };

static unsigned char contra_seta[] = {
   0x7f, 0x7f, 0x7f, 0x7f, 0x7f };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg,U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void draw(u8g2_t *u8g2);

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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
	u8g2_Setup_st7920_s_128x64_f2(&u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8g2_gpio_and_delay_stm32);
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		/*
		u8g2_ClearBuffer(&u8g2);
		//u8g2_SetFontMode(&u8g2, 1);
		//u8g2_SetFontDirection(&u8g2, 0);
		u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
		u8g2_DrawStr(&u8g2, 0, 10, "Hello STM32!");
		
		u8g2_SendBuffer(&u8g2);
		*/
		//HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SP11_CS_Pin, GPIO_PIN_SET);

		draw(&u8g2);
		HAL_Delay(200);
		//HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SP11_CS_Pin, GPIO_PIN_RESET);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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

  /** Initializes the CPU, AHB and APB buses clocks
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
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);


  /*Configure GPIO pin : SPI1_CS_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI1_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RESET_Pin */
  GPIO_InitStruct.Pin = LCD_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */

// Configuração dos pinos SPI1: PA5 (SCK) e PA7 (MOSI)
	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t byte;
  uint8_t *data;
 
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
//      data = (uint8_t *)arg_ptr;
//      while( arg_int > 0 )
//      {
//        byte = *data;
//        data++;
//        arg_int--;
//        HAL_SPI_Transmit(&hspi1, &byte, 1, 100);
//      }
			HAL_SPI_Transmit(&hspi1, arg_ptr, arg_int, 100);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
      __nop(); // 21 ns
      break;
	case U8X8_MSG_BYTE_END_TRANSFER:
			__nop();
			HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
			break;
    default:
      return 0;
  }
  return 1;
}

uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg,U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr) {
    return 1;
}

void draw(u8g2_t *u8g2)
{
	static bool downtoup = true;
	static unsigned long update_ii;
	
//Variaveis temporarias:
	int acumulada = 99000, inst = 2;
	u8g2_ClearBuffer(u8g2);
	u8g2_SetFont(u8g2, u8g2_font_6x10_tr);
	u8g2_SetBitmapMode(u8g2, 1);
	
//Animacao bola
	if (HAL_GetTick() - update_ii >= 200) {
	ii += (downtoup ? 1 : -1);
		if (ii >= 8) {
			downtoup = false;
      } else if (ii <= 0) {
			downtoup = true;
      }
	update_ii = HAL_GetTick();
	}
		u8g2_DrawXBM(u8g2, 118, 54, 10, 10, bola[ii]);
	
//Logo Labsolda
		u8g2_DrawXBM(u8g2, 113, 0, 15, 11, labsolda);
	
//Página 1
	if (currentPage == 1){

	//Acumulada e instantanea
		u8g2_DrawStr(u8g2, 3, 8, "Vazao");
		//Acumulada
		char acumuladaStr[20];
		sprintf(acumuladaStr, "Acumulada: %dL", acumulada);
		u8g2_DrawStr(u8g2, 5, 20, acumuladaStr);
		//Instantanea
		char instantaneaStr[20];
		sprintf(instantaneaStr, "Instantanea: %dL/s", inst);
		u8g2_DrawStr(u8g2, 5, 32, instantaneaStr);
		
	//Argonio ou CO2
		u8g2_DrawStr(u8g2, 3, 46, "Gas: ");
		if (gas == 0){
			u8g2_DrawStr(u8g2, 3, 76, "CO2");
		} else {
			u8g2_DrawStr(u8g2, 3, 76, "Argonio");
		}
			
	//Configuracao
			u8g2_DrawStr(u8g2, 3, 60, "Configuracoes");

	//Chamada de bitmaps para o menu:
		//Acumulada
		if (item_selecionado == 0){u8g2_DrawXBM(u8g2, 2, 11, 126, 12, acumulada_selecao);}

		//Gas
		else if (item_selecionado == 1){
			if (gas == 0) {
				u8g2_DrawXBM(u8g2, 0, 37, 55, 12, co2_selecao);
				u8g2_DrawXBM(u8g2, 56, 40, 7, 5, seta); //Seta do menu
			}
			else {
				u8g2_DrawXBM(u8g2, 0, 37, 85, 12, caixa_selecao);
				u8g2_DrawXBM(u8g2, 86, 40, 7, 5, seta); //Seta do menu
			}
    }
		
		//Configuracao
		else if (item_selecionado == 2){
			u8g2_DrawXBM(u8g2, 0, 51, 85, 12, caixa_selecao);
			u8g2_DrawXBM(u8g2, 86, 54, 7, 5, seta); //Seta do menu
		}
	} //Fim pagina 1

	
//Pagina 2
	else if (currentPage == 2){
		
	//Voltar
		u8g2_DrawStr(u8g2, 3, 57, "Voltar");
		
	//Minimo
		char minStr[20];
		sprintf(minStr, "Maximo: %.1f", valorMin);
		u8g2_DrawStr(u8g2, 3, 35, minStr);
		
	//Maximo
		char maxStr[20];
		sprintf(maxStr, "Maximo: %.1f", valorMax);
		u8g2_DrawStr(u8g2, 3, 57, maxStr);

	//Chamada de bitmaps para o menu:
		//Voltar
		if (item_selecionado == 2){
			u8g2_DrawXBM(u8g2, 0, 48, 55, 12, co2_selecao);
			u8g2_DrawXBM(u8g2, 56, 51, 7, 5, seta);
		}
		
		//Minimo caixa de selecao
		else if (item_selecionado == 1){
			u8g2_DrawXBM(u8g2, 0, 26, 85, 12, caixa_selecao);
		}
		
		//Maximo caixa de selecao
		else if (item_selecionado == 0){
			u8g2_DrawXBM(u8g2, 0, 6, 85, 12, caixa_selecao);
		}
		
		//Minimo seta de selecao
		if (!editMax && item_selecionado == 0){
			u8g2_DrawXBM(u8g2, 86, 9, 7, 5, seta);
		} else if (editMax) {
			if (HAL_GetTick() - blink <= 1000){
				u8g2_DrawXBM(u8g2, 86, 9, 7, 5, seta);
			} 
			else if (HAL_GetTick() - blink >= 1400){
				blink = HAL_GetTick();
			}
		}
		
		//Maximo seta de selecao
		if (!editMin && item_selecionado == 1){
			u8g2_DrawXBM(u8g2, 86, 29, 7, 5, seta);
		} else if (editMin) {
			if (HAL_GetTick() - blink <= 1000){
				u8g2_DrawXBM(u8g2, 86, 29, 7, 5, seta);
			} 
			else if (HAL_GetTick() - blink >= 1400){
				blink = HAL_GetTick();
			}
		}
			
	}//Fim pagina 2
		
//Envia buffer
	u8g2_SendBuffer(u8g2);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
