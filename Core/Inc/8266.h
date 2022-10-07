/**
 * @file 8266.h
 * @author HuangMiaozhi (hanoch1024@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-04
 * 
 * @attention
 * Jinxin Microelec co.
 * 
 */
#ifndef  __BSP_ESP8266_H__
#define	 __BSP_ESP8266_H__

d#include "stm32f1xx_hal.h"
#include "stdio.h"
typedef enum
{
    STA,
    AP,
    AP_STA,
}net_mode;

typedef enum
{
    TCP,
    UDP,
}NetPro_typeDef;

#define RX_BUF_MAX_LEN 1024
typedef struct 
{
    /* data */
    char data_rx_buf[RX_BUF_MAX_LEN];
    union 
    {
        /* data */
        //! __IO == volatile
        __IO uint16_t Infall;
        struct 
        {
            /* data */
            __IO uint16_t FrameLength : 15;
            __IO uint16_t FrameFinishFlag : 1;
        }InfBit;
    };
}RTx_Frame;

extern RTx_Frame esp_frame;
extern uint8_t esp8266_rxdata;


#define AT_START_TEST "AT"
#define AT_VERSION "AT+GMR"
#define AT_RESET "AT_RST"

#define ESP8266_RST_HIGH()  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET)
#define ESP8266_RST_LOW()   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET)

#endif
