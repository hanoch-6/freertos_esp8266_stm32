#include "stdio.h"
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef debug_huart;
uint8_t ch;
uint8_t ch_r;

//重写这个函数,重定向printf函数到串口
/*fputc*/
int fputc(int c, FILE * f)
{
    ch=c;
    HAL_UART_Transmit(&debug_huart,&ch,1,1000); //发送串口，不同的单片机函数和串口命名不同，替换对于的函数串口名字即可实现不同库和不同单片机的重定向了
    return c;
}

int fgetc(FILE * f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&debug_huart,&ch, 1, 0xffff);
  return ch;
}
