/**
 * @file LoopQueue.c
 * @author HuangMiaozhi (hanoch1024@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2022-10-19
 *
 * @attention
 * Jinxin Microelec co.
 *
 */

#include "LoopQueue.h"

#include "FreeRTOS.h"
#include "task.h"
/**
 * @brief init loop queue
 *
 * @param plp pointer
 * @param p_data buffer
 * @param len length
 * @return uint8_t
 */
uint8_t init_loop_queue(LoopQueue *plp, DATA_TYPE *p_data, uint8_t len)
{
    uint8_t i = 0;
    if ((void *)0 == plp || 0 == len || (void *)0 == p_data)
        return 0;
    plp->insert_index = 0;
    plp->max_len = len;
    plp->buffer = p_data;
    plp->count = 0;
    plp->read_index = 0;

    for (i = 0; i < plp->max_len; ++i)
    {
        plp->buffer[i] = 0;
    }
}
/**
 * @brief insert elements to queue
 *
 * @param plp
 * @param elements
 * @return uint8_t
 */
uint8_t insert_elements_loop_queue(LoopQueue *plp, DATA_TYPE elements)
{
    if ((void *)0 == plp)
        return 0;

    if (0 == space_loop_queue(plp))
        return 0;

    if (plp->insert_index >= plp->max_len)
        plp->insert_index = 0;

    // take elements and insert queue
    plp->buffer[plp->insert_index] = elements;

    ++plp->insert_index;

    taskDISABLE_INTERRUPTS(); // disable interrupt
    ++plp->count;             // wait process data count
    taskENABLE_INTERRUPTS();  // enable interrupt

    return 1;
}
/**
 * @brief read from loop queue
 *
 * @param plp
 * @return uint8_t
 */
DATA_TYPE read_elements_loop_queue(LoopQueue *plp)
{
    DATA_TYPE element;
    if ((void *)0 == plp)
        return 0;
    if (plp->read_index >= plp->max_len)
        plp->read_index = 0;
    element = plp->buffer[plp->read_index];

    ++plp->read_index;

    taskDISABLE_INTERRUPTS();
    --plp->count;
    taskENABLE_INTERRUPTS();

    return element;
}
/**
 * @brief return valid data count
 *
 * @param plp
 * @return uint8_t
 */
uint8_t space_loop_queue(LoopQueue *plp)
{
    if ((void *)0 == plp)
        return 0;

    if (plp->count >= plp->max_len)
        return 0;
    return (plp->max_len - plp->count);
}
/**
 * @brief return queue space
 *
 * @param plp
 * @return uint8_t
 */
uint8_t count_loop_queue(LoopQueue *plp)
{
    if ((void *)0 == plp)
        return 0;

    return plp->count;
}
/**
 * @brief clean index in looop queue 
 * 
 * @param plp 
 * @return uint8_t 
 */
uint8_t clean_loop_queue(LoopQueue *plp)
{
    if ((void *)0 == plp)
        return 0;

    plp->insert_index = 0;
    plp->read_index = 0;
    plp->count = 0;
    return 1;
}