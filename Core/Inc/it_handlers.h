#include "init.h"

#define POWERON_PERIOD              6000 * 2  // Изначально была в main.c, но данная переменная нужна в it_handkers.c

// Прерывание при нажатии кнопки
void EXTI15_10_IRQHandler(void);
// Прерывание таймера, которое просто считывает секунды
void SysTick_Handler(void);
// Прерывание, которое является задержкой
void User_Delay(uint32_t delay);
