/**
 * @file 8266.c
 * @author HuangMiaozhi (hanoch1024@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-04
 * 
 * @attention
 * Jinxin Microelec co.
 * 
 */
#include "8266.h"
#include "FreeRTOS.h"
#include "stdbool.h"

UART_HandleTypeDef huart;
RTx_Frame esp_frame;


void ESP8266_AT_test(void);

void ESP8266_AT_test(void)
{
    char count = 0;
    ESP8266_RST_HIGH();
    os_delay(1000);    

}
bool ESP8266_cmd(uint8_t AT_buffer)
{

}
void ESP8266_Rst(void)
{
#if 0
    ESP8266_cmd(AT_RESET);
#endif
    ESP8266_RST_LOW();
    os_delay(1000);
    ESP8266_RST_HIGH();
}