/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "UserTask.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId debug_usart_tasHandle;
osThreadId add_count;
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */
static void led_control(void);
static void idle_50ms(void const *argument);
static void feed_dog(void);
static void initUsartIT(UART_HandleTypeDef *huart);
static void init_system(void);
static void usartEsp_send_task(void const *arg);
static void usartEsp_receive_task(void const *arg);
static void restart_usart(UART_HandleTypeDef *huart);

void Task_50ms(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
	/* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
	init_system();

	// 喂狗、系统运行灯控制任务
	osThreadDef(Task50MSThread, idle_50ms, osPriorityIdle, 0, 128);
	osThreadCreate(osThread(Task50MSThread), NULL);

	osThreadDef(UART2_TX_Task, usartEsp_send_task, osPriorityNormal, 0, 512);
	osThreadCreate(osThread(UART2_TX_Task), NULL);

	osThreadDef(UART2_RT_Task, usartEsp_receive_task, osPriorityNormal, 0, 512);
	osThreadCreate(osThread(UART2_RT_Task), NULL);
}
/**
 * @brief
 *
 * @param argument
 */
static void idle_50ms(void const *argument)
{
	/* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for (;;)
	{
		osDelay(50);
		feed_dog();
		// printf("co")
	}
	/* USER CODE END StartDefaultTask */
}
/**
 * @brief
 *
 * @param arg
 */
static void usartEsp_send_task(void const *arg)
{
	uint16_t datalen = 0;
	LoopQueue *sendQueue;
	unsigned int i = 0;
	char send_buffer[150];
	TickType_t old_time = 0;
	old_time = xTaskGetTickCount();
	for (;;)
	{
		osDelay(10);

		while (huart_esp8266.gState == HAL_UART_STATE_BUSY_TX && xTaskGetTickCount() - old_time <= 3)
			;
		sendQueue = getUsartSendLoopQueue(USART2_ID);
		if (sendQueue != NULL)
		{
			datalen = writeBufferLen(USART2_ID);
			if (datalen > 0)
			{
				if (datalen >= 160)
					datalen = 160;

				for (i = 0; i < datalen; ++i)
				{
					send_buffer[i] = read_elements_loop_queue(sendQueue);
				}
				HAL_UART_Transmit_DMA(&huart_esp8266, (uint8_t *)send_buffer, (uint16_t)datalen);
			}
		}
	}
}
/**
 * @brief	串口2 esp8266接收处理
 *
 * @param arg
 */
static void usartEsp_receive_task(void const *arg)
{
	uint8_t datalen = 0;
	unsigned int i = 0;
	char data = 0;
	while (1)
	{
		// 10ms 115200 全速模式下能接收144个字节
		osDelay(8);

		restart_usart(&huart_esp8266);

		datalen = readBufferLen(USART2_ID);

		if (datalen > 0)
		{
			for (i = 0; i < datalen; ++i)
			{
				data = read_char(USART2_ID);

#if DEBUG
				write_char(USART2_ID, data);
#endif
			}
		}
	}
}

/**
 * @brief
 *
 */
static void led_control(void)
{
	static unsigned int step = 0;
	++step;
	if (step <= 20)
	{
		SET_RUN_LED();
	}
	else if (step <= 40)
	{
		RESET_RUN_LED();
	}
	else
	{
		step = 0;
	}
}
/**
 * @brief
 *
 */
static void feed_dog(void)
{
	static unsigned int step = 0;
	++step;
	if (2 == step)
	{
		step = 0;
		HAL_IWDG_Refresh(&hiwdg);
	}
}
/**
 * @brief
 *
 */
static void init_system(void)
{
	initUsartBuffer(USART2_ID);

	mutex_usart2_tx = xSemaphoreCreateMutex();
	mutex_usart2_rx = xSemaphoreCreateMutex();

	initUsartIT(&huart_esp8266);

	write_str(USART2_ID, "USART2 ESP8266 ENABLE\n", sizeof("USART2 ESP8266 ENABLE\n") / sizeof(char));
}
/**
 * @brief 初始化串口接收中断
 *
 * @param huart
 */
static void initUsartIT(UART_HandleTypeDef *huart)
{
	huart->ErrorCode = HAL_UART_ERROR_NONE;
	huart->RxState = HAL_UART_STATE_BUSY_RX;

	/* Process Unlocked */
	__HAL_UNLOCK(huart);
	/* Enable the UART Parity Error Interrupt */

	__HAL_UART_ENABLE_IT(huart, UART_IT_PE);
	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */

	__HAL_UART_ENABLE_IT(huart, UART_IT_ERR);
	/* Enable the UART Data Register not empty Interrupt */

	__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
}

static void restart_usart(UART_HandleTypeDef *huart)
{
	if (huart == 0)
		return;

	if ((huart->ErrorCode |= HAL_UART_ERROR_NONE) != RESET)
	{
		huart->ErrorCode = HAL_UART_ERROR_NONE;
		huart->RxState = HAL_UART_STATE_BUSY_RX;
	}

	__HAL_UNLOCK(huart);

	__HAL_UART_ENABLE_IT(huart, UART_IT_PE);
	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */

	__HAL_UART_ENABLE_IT(huart, UART_IT_ERR);
	/* Enable the UART Data Register not empty Interrupt */

	__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
}
