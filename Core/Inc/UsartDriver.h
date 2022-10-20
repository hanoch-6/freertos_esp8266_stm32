/**
 * @file UsartDriver.h
 * @author HuangMiaozhi (hanoch1024@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2022-10-19
 *
 * @attention
 * Jinxin Microelec co.
 *
 */
#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include <stdint.h>
#include "LoopQueue.h"
#define USART1_ID 0
#define USART2_ID 1
#define USART_MAX_ID 4

uint8_t initUsartBuffer(uint8_t id);
char write_char(uint8_t id, char c);
uint8_t write_str(uint8_t id, char *pc, uint8_t len);
char read_char(uint8_t id);
uint32_t read_str(uint8_t id, char *buffer, uint32_t len);
LoopQueue *getUsartReceiveLoopQueue(uint8_t id);
LoopQueue *getUsartSendLoopQueue(uint8_t id);
uint32_t writeBufferLen(uint8_t id);
uint32_t readBufferLen(uint8_t id);

#endif