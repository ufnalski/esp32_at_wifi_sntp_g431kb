/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ssd1306.h"
#include "wifi_credentials.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ESP32_AT_UART_HANDLE &huart1
#define ESP32_AT_UART_INSTANCE USART1
#define SNTPTIME_RESPONSE_LENGTH 43

#define DEBUG_UART_HANDLE &huart2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t echo_off[] = "ATE0\r\n";
uint8_t init_wifi[] = "AT+CWINIT=1\r\n";
uint8_t wifi_mode[] = "AT+CWMODE=1\r\n";
uint8_t wifi_no_autoconnect[] = "AT+CWAUTOCONN=0\r\n";
uint8_t wifi_connect[] = WIFI_CREDENTIALS;
uint8_t ntp_pool[] = "AT+CIPSNTPCFG=1,2,\"pl.pool.ntp.org\"\r\n";
uint8_t query_time[] = "AT+CIPSNTPTIME?\r\n";

extern DMA_HandleTypeDef hdma_usart1_rx;
uint8_t rx_time[SNTPTIME_RESPONSE_LENGTH];
uint8_t esp32c6at_data_received_flag = 0;

uint8_t query_time_elapsed_flag = 0;

char lcd_line[32];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
	MX_DMA_Init();
	MX_USART2_UART_Init();
	MX_I2C1_Init();
	MX_USART1_UART_Init();
	MX_TIM17_Init();
	/* USER CODE BEGIN 2 */
	ssd1306_Init();
	ssd1306_Fill(Black);
	ssd1306_SetCursor(22, 2);
	ssd1306_WriteString("ufnalski.edu.pl", Font_6x8, White);
	ssd1306_SetCursor(10, 12);
	ssd1306_WriteString("ESP32C6-AT + STM32", Font_6x8, White);
	ssd1306_SetCursor(2, 22);
	ssd1306_WriteString("---- Inspired by ----", Font_6x8, White);
	ssd1306_SetCursor(32, 32);
	ssd1306_WriteString("msalamon.pl", Font_6x8, White);
	ssd1306_SetCursor(7, 43);
	ssd1306_WriteString("SNTP clock (Poland)", Font_6x8, White);

	ssd1306_UpdateScreen();

	HAL_UART_Transmit(ESP32_AT_UART_HANDLE, echo_off, sizeof(echo_off) - 1,
			100);
	HAL_UART_Transmit_DMA(DEBUG_UART_HANDLE, echo_off, sizeof(echo_off) - 1);
	HAL_Delay(100);

	HAL_UART_Transmit(ESP32_AT_UART_HANDLE, init_wifi, sizeof(init_wifi) - 1,
			100);
	HAL_UART_Transmit_DMA(DEBUG_UART_HANDLE, init_wifi, sizeof(init_wifi) - 1);
	HAL_Delay(500);

	HAL_UART_Transmit(ESP32_AT_UART_HANDLE, wifi_mode, sizeof(wifi_mode) - 1,
			100);
	HAL_UART_Transmit_DMA(DEBUG_UART_HANDLE, wifi_mode, sizeof(wifi_mode) - 1);
	HAL_Delay(500);

	HAL_UART_Transmit(ESP32_AT_UART_HANDLE, wifi_no_autoconnect,
			sizeof(wifi_no_autoconnect) - 1, 100);
	HAL_UART_Transmit_DMA(DEBUG_UART_HANDLE, wifi_no_autoconnect,
			sizeof(wifi_no_autoconnect) - 1);
	HAL_Delay(500);

	HAL_UART_Transmit(ESP32_AT_UART_HANDLE, wifi_connect,
			sizeof(wifi_connect) - 1, 100);
	HAL_UART_Transmit_DMA(DEBUG_UART_HANDLE, wifi_connect,
			sizeof(wifi_connect) - 1);
	HAL_Delay(3000);

	HAL_UART_Transmit(ESP32_AT_UART_HANDLE, ntp_pool, sizeof(ntp_pool) - 1,
			100);
	HAL_UART_Transmit_DMA(DEBUG_UART_HANDLE, ntp_pool, sizeof(ntp_pool) - 1);
	HAL_Delay(500);

	HAL_UART_Receive_DMA(ESP32_AT_UART_HANDLE, rx_time,
	SNTPTIME_RESPONSE_LENGTH);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	HAL_UART_Transmit_DMA(ESP32_AT_UART_HANDLE, query_time,
			sizeof(query_time) - 1);
	HAL_UART_Transmit_DMA(DEBUG_UART_HANDLE, query_time,
			sizeof(query_time) - 1);

	HAL_TIM_Base_Start_IT(&htim17);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		if (query_time_elapsed_flag == 1)
		{
			query_time_elapsed_flag = 0;

			if (esp32c6at_data_received_flag == 1)
			{
				esp32c6at_data_received_flag = 0;

				memcpy(lcd_line, &rx_time[25], 8);
				lcd_line[8] = '\0';
				ssd1306_SetCursor(40, 53);
				ssd1306_WriteString(lcd_line, Font_6x8, White);

				ssd1306_UpdateScreen();

				HAL_UART_Receive_DMA(ESP32_AT_UART_HANDLE, rx_time,
				SNTPTIME_RESPONSE_LENGTH);
				__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
				HAL_UART_Transmit_DMA(ESP32_AT_UART_HANDLE, query_time,
						sizeof(query_time) - 1);
				HAL_UART_Transmit_DMA(DEBUG_UART_HANDLE, query_time,
										sizeof(query_time) - 1);
			}

		}
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
	RCC_OscInitTypeDef RCC_OscInitStruct =
	{ 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct =
	{ 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == ESP32_AT_UART_INSTANCE)
	{
		esp32c6at_data_received_flag = 1;
		HAL_UART_Transmit_DMA(DEBUG_UART_HANDLE, rx_time,
				SNTPTIME_RESPONSE_LENGTH);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM17) // every 1 second
	{
		query_time_elapsed_flag = 1;
	}
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
