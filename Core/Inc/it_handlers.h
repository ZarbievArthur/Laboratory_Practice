#include "init.h"

// Прерывание при нажатии кнопки
void EXTI15_10_IRQHandler(void);
// Прерывание таймера, которое просто считывает секунды
void SysTick_Handler(void);
// Прерывание, которое является задержкой
void User_Delay(uint32_t delay);
