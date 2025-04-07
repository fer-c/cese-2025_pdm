/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>
#include "API_debounce.h"
#include "API_delay_time_matrix.h"
#include "API_uart.h"

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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/**
 * @brief Duración de 100 ms.
 */
const tick_t T_100MS = 100;
/**
 * @brief Duración de 200 ms.
 */
const tick_t T_200MS = 200;
/**
 * @brief Duración de 500 ms.
 */
const tick_t T_500MS = 500;
/**
 * @brief Duración de 1 segundo
 */
const tick_t T_1S = 1000;

/**
 * @brief Número de tiempos en la matriz.
 */
const index_t NUM_TIEMPOS = 2;

/**
 * @brief Tamaño max del comando.
 */
const index_t CMD_SIZE = 10;

/**
 * @brief Array de duraciones para el LED.
 */
static tick_t TIEMPOS[] = { T_100MS, T_500MS };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void initCmd(uint8_t *cmd, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    cmd[i] = 0;
  }
}

static void sendEcho(uint8_t *cmd) {
  char *echo = strcat((char *)cmd, "\r\n"); // Agrega un salto de línea al comando
  uartSendString((uint8_t *)echo); // Envía el comando recibido por UART
}

static uint8_t obtenerCommando() {
  static uint8_t buffer[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // Inicializa el comando

  uartReceiveStringSize(buffer, CMD_SIZE); // Recibe el comando por UART
  uint8_t cmd = buffer[0]; // Almacena el primer byte del comando en cmd

  if (cmd != 0) {
    sendEcho(buffer); // Envía el eco del comando recibido
  }
  initCmd(buffer, CMD_SIZE);

  return cmd; // Devuelve el comando recibido
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @return int
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  uartInit();

  delay_time_matrix_t delayTimeMatrix; /* Matriz de duraciones para el LED. */
  delay_t delay; /* Delay para el LED. */
  debounce_t debounceFSM; /* Máquina de estados para el debounce. */


  delayTimeMatrixInit(&delayTimeMatrix, NUM_TIEMPOS, TIEMPOS); // Inicializa la matriz de duraciones
  delayInit(&delay, delayTimeMatrixRead(&delayTimeMatrix)); // Inicializa el delay con una duración de 100 ms
  debounceFSM_init(&debounceFSM); // Inicializa la máquina de estados para el debounce

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  tick_t newDuration = delayTimeMatrixRead(&delayTimeMatrix); // Lee la duración inicial de la matriz

  while (1) {

    // Actualización del FSM de debounce
    debounceFSM_update(&debounceFSM);
    uint8_t cmd = obtenerCommando();
    switch (cmd) {
      case 'c':
        uartSendClearScreen(); // Borra la pantalla
        uartSendConfiguration(); // Envía la configuración del UART
        break;
      case '1':
        newDuration = T_100MS; // Cambia la duración del delay a 100 ms
        uartSendString((uint8_t *)"Delay: 100 ms\r\n");
        break;
      case '2':
        newDuration = T_200MS; // Cambia la duración del delay a 200 ms
        uartSendString((uint8_t *)"Delay: 200 ms\r\n");
        break;
      case '3':
        newDuration = T_500MS; // Cambia la duración del delay a 500 ms
        uartSendString((uint8_t *)"Delay: 500 ms\r\n");
        break;
      case '4':
        newDuration = T_1S; // Cambia la duración del delay a 1 segundo
        uartSendString((uint8_t *)"Delay: 1 s\r\n");
        break;
      default:
        break; // Comando no reconocido
    }

    if (delayRead(&delay)) {
      HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
      if (readKey(&debounceFSM)) {
        // Cambia la duración del delay al siguiente valor
        newDuration = delayTimeMatrixNext(&delayTimeMatrix);
      }
      delayWrite(&delay, newDuration); // Cambia la duración del delay
    }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @return None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @return None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  return; /**< Reemplazado por API_uart#uartInit() */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @return None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BTN_Pin */
  GPIO_InitStruct.Pin = BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @return None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @return None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
