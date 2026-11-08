// Здесь будут описаны все прерывания, которые мы определим
#include "it_handlers.h"

// Исключение при нажатии кнопка
extern uint8_t LedState;

uint8_t BtnCount = 0;

/**
 * @brief Исключение при нажатии кнопки на PC13
 */
void EXTI15_10_IRQHandler(void){
    BtnCount++;
    if (BtnCount >= 1){
        LedState = !LedState;
        BtnCount = 0;
    }
    // Очишаем нужный бит регистра прерываний
    SET_BIT(EXTI->PR, EXTI_PR_PR13);
}


// Исключение при срабатывании таймера
extern uint16_t GlobalTickCount;

/**
 * @brief ИСключение при срабатывании таймера
 */
void SysTick_Handler(void){
    GlobalTickCount++;
}

// Прерывание-задержка
uint16_t DelayTickCount;

void User_Delay(uint32_t delay){
    while(DelayTickCount < delay){} //Цикл, благодаря которому происходит задержка программы
    if(DelayTickCount >= delay) DelayTickCount = 0; //Обнуление переменной
}
