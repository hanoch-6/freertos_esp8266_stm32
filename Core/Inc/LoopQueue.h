#ifndef LOOPQUEUE_H
#define LOOPQUEUE_H

#include <stdint.h>
typedef char DATA_TYPE;


typedef struct _LoopQueue
{
    /* data */
    DATA_TYPE* buffer;
    uint8_t max_len;
    volatile uint8_t insert_index;
    volatile uint8_t read_index;
    volatile uint8_t count;
}LoopQueue;

uint8_t init_loop_queue(LoopQueue *plp, DATA_TYPE *p_data, uint8_t len);
uint8_t insert_elements_loop_queue(LoopQueue *plp, DATA_TYPE elements);
DATA_TYPE read_elements_loop_queue(LoopQueue *plp);
uint8_t space_loop_queue(LoopQueue *plp);
uint8_t count_loop_queue(LoopQueue *plp);
uint8_t clean_loop_queue(LoopQueue *plp);

#endif