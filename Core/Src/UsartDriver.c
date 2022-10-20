/**
 * @file UsartDriver.c
 * @author HuangMiaozhi (hanoch1024@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2022-10-19
 *
 * @attention
 * Jinxin Microelec co.
 *
 */
#include "UsartDriver.h"
#define USART1_RECEIVE_MAX 526
#define USART1_SEND_MAX 526
#define USART2_RECEIVE_MAX 526
#define USART2_SEND_MAX 526
#define USART3_RECEIVE_MAX 526
#define USART3_SEND_MAX 526
#define RTOS_ENABLE 1

static char usartEsp_receive_buff[USART2_RECEIVE_MAX];
static char usartEsp_receive_buff[USART2_SEND_MAX];
static LoopQueue usartEsp_receive_queue;
static LoopQueue usartEsp_send_queue;

/**
 * @brief 初始化串口2接收发送队列，
 *
 * @param id
 * @return uint8_t
 */
uint8_t initUsartBuffer(uint8_t id)
{
    if (id > USART_MAX_ID)
        return 0;

    if (USART2_ID == id)
    {
        init_loop_queue(&usartEsp_receive_queue, (char *)usartEsp_receive_buff, USART2_RECEIVE_MAX);
        init_loop_queue(&usartEsp_send_queue, (char *)usartEsp_receive_buff, USART2_SEND_MAX);
        return 1;
    }
    return 0;
}

uint32_t readBufferLen(uint8_t id)
{
    if (id >= USART_MAX_ID)
        return 0;
    if (USART2_ID == id)
    {
        return (uint32_t)count_loop_queue(&usartEsp_receive_queue);
    }
    return 0;
}

uint32_t writeBufferLen(uint8_t id)
{
    if (id >= USART_MAX_ID)
        return 0;
    if (USART2_ID == id)
    {
        return (uint32_t)count_loop_queue(&usartEsp_send_queue);
    }
    return 0;
}

LoopQueue *getUsartSendLoopQueue(uint8_t id)
{
    if (id >= USART_MAX_ID)
        return 0;

    if (USART2_ID == id)
    {
        return &usartEsp_send_queue;
    }
    return 0;
}

LoopQueue *getUsartReceiveLoopQueue(uint8_t id)
{
    if (id >= USART_MAX_ID)
        return 0;

    if (USART2_ID == id)
    {
        return &usartEsp_receive_queue;
    }
    return 0;
}
/**
 * @brief 写入字符
 *
 * @param id
 * @param c
 * @return char
 */
char write_char(uint8_t id, char c)
{
    if (id > USART_MAX_ID)
        return 0;

    if (USART2_ID == id)
    {
        insert_elements_loop_queue(&usartEsp_send_queue, c);
    }
    return c;
}
/**
 * @brief 写入字符串
 *
 * @param id
 * @param pc
 * @param len
 * @return uint8_t
 */
uint8_t write_str(uint8_t id, char *pc, uint8_t len)
{
    if (id >= USART_MAX_ID || 0 == pc || 0 == len)
        return 0;

    if (USART2_ID == id)
    {
        uint8_t datalen = len;
        for (; datalen > 0; --datalen)
        {
            insert_elements_loop_queue(&usartEsp_send_queue, (char)(*(char *)pc));
            ++pc;
        }
        return len;
    }
    return 0;
}
/**
 * @brief
 *
 * @param id
 * @return char
 */
char read_char(uint8_t id)
{
    if (id > USART_MAX_ID)
        return 0;

    if (USART2_ID == id)
    {
        return (char)read_elements_loop_queue(&usartEsp_receive_queue);
    }

    return 0;
}
/**
 * @brief 读取字符串
 *
 * @param id
 * @param buffer
 * @param len
 * @return uint32_t
 */
uint32_t read_str(uint8_t id, char *buffer, uint32_t len)
{
    int datalen = 0;
    uint32_t rlen = 0;
    uint32_t llen = 0;

    if (id >= USART_MAX_ID || 0 == buffer || 0 == len)
        return 0;

    if (USART2_ID == id)
    {
        datalen = count_loop_queue(&usartEsp_receive_queue);
        if (0 == datalen)
            return 0;

        if (datalen > len)
            llen = len; // buffer长度
        else
            llen = datalen; // queue有效长度
        rlen = llen;
        for (; llen > 0; --llen)
        {
            *buffer = (char)read_elements_loop_queue(&usartEsp_receive_queue);
            ++buffer;
        }
        return rlen;
    }
    return 0;
}