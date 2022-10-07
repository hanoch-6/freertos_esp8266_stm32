#ifndef CONFIG_H
#define CONFIG_H

#include "stm32f1xx_hal.h"

#define run_led 0
#define status_led 1

#define ledAction(led, status)                        \
    if (led == run_led)                               \
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, status); \
    else                                              \
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, status);
#define SET_RUN_LED() ledAction(run_led, GPIO_PIN_RESET)
#define RESET_RUN_LED() ledAction(run_led, GPIO_PIN_SET)


#endif