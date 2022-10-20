#ifndef USER_TASK_H
#define USER_TASK_H

#include "HardwareConfig.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#include "UsartDriver.h"

extern IWDG_HandleTypeDef hiwdg;
extern UART_HandleTypeDef huart_esp8266;

static xSemaphoreHandle mutex_usart2_tx;
static xSemaphoreHandle mutex_usart2_rx;

#endif