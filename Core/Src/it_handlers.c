// Здесь будут описаны все прерывания, которые мы определим
#include "it_handlers.h"

// Исключение при нажатии кнопка
extern uint8_t LedState;            // Хранит состояние светодиода во втором режиме. Нужна для построения графиков
extern uint32_t GlobalTickCount;    // Счетчик, который отвечает за включение и выключение светодиодов
extern long Flicker_Led1Frequency;       // Частота мигания светодиода - задается в main.c
extern long Flicker_Led2Frequency;
extern long Flicker_Led3Frequency;       // Частота мигания светодиода - задается в main.c
extern long Flicker_Led4Frequency;
extern long Flicker_Led5Frequency;       // Частота мигания светодиода - задается в main.c
extern long Flicker_Led6Frequency;

extern uint8_t Btn1_State;          // Состояние первой кнопки
extern uint8_t Btn2_State;          // Состояние второй кнопки
extern uint8_t Btn3_State;          // Состояние третьей кнопки

extern uint8_t Led1_State;
extern uint8_t Led2_State;
extern uint8_t Led3_State;
extern uint8_t Led4_State;
extern uint8_t Led5_State;
extern uint8_t Led6_State;

uint32_t Flicker_Led1Count;              // Счетчик для включения и выключения 1-ого светодиода
uint32_t Flicker_Led2Count;              // Счетчик для включения и выключения 2-ого светодиода
uint32_t Flicker_Led3Count;              // Счетчик для включения и выключения 3-ого светодиода
uint32_t Flicker_Led4Count;              // Счетчик для включения и выключения 4-ого светодиода
uint32_t Flicker_Led5Count;              // Счетчик для включения и выключения 5-ого светодиода
uint32_t Flicker_Led6Count;              // Счетчик для включения и выключения 6-ого светодиода

/**
 * @brief Исключение при нажатии кнопки на PC13
 */
#define DELAY_BUTTON_FILTER 500
uint16_t ExternInterruptTickCount;
void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR15) {
        EXTI->PR = EXTI_PR_PR15;

        if (ExternInterruptTickCount >= DELAY_BUTTON_FILTER){ //Выполнится, когда пройдёт 100 мс с момента обнуления данной переменной
            Btn1_State = !Btn1_State;       // Просто меняем режим
            Btn2_State = 0;                 // Для плавного регулирования частотой
            Btn3_State = 0;                 // Выбор светодиода будет всегда начинаться с первого светодиода
            ExternInterruptTickCount = 0; //Обнуление счётчика таймера 
            GlobalTickCount = 0; // Обнуляем счетчик времени светодиодов
            
            // Выключаем все светодиоды    
            SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6 | GPIO_BSRR_BR7);                    // Выключение диодов на порте С одной командой
            SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR4);                                    // Выключение светодиодов на порте А одной командой
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR3 | GPIO_BSRR_BR4 | GPIO_BSRR_BR5);    // Выключение светодиодов на порте B одной командой
        }
    }
    if (EXTI->PR & EXTI_PR_PR13){
        EXTI->PR = EXTI_PR_PR13;
        if (ExternInterruptTickCount >= DELAY_BUTTON_FILTER){ //Выполнится, когда пройдёт 100 мс с момента обнуления данной переменной
            Btn2_State++;
            if (Btn2_State > 2){
                Btn2_State = 0;
            }
            ExternInterruptTickCount = 0; //Обнуление счётчика таймера 
        }
    }
    if (EXTI->PR & EXTI_PR_PR12){
        EXTI->PR = EXTI_PR_PR12;
        if (ExternInterruptTickCount >= DELAY_BUTTON_FILTER){ //Выполнится, когда пройдёт 100 мс с момента обнуления данной переменной
            Btn3_State++;
            if (Btn3_State > 5){
                Btn3_State = 0;
            }
            ExternInterruptTickCount = 0; //Обнуление счётчика таймера 
        }
    }
}


/**
 * @brief ИСключение при срабатывании таймера
 */
void SysTick_Handler(void){
    GlobalTickCount++;
    
    // Реализуем счетчик для каждого светодиода
    // Каждый счетчик будет производит нужное количество отсчетов для включения или выключения соответствующего светодиода
    if (Btn1_State & (GlobalTickCount > 0)) Flicker_Led1Count++; // Иначе Flicker_Led1Count никогда не обнулится
    else {
        Flicker_Led1Count = 0;
        Led1_State = 0;
    }
    if (Btn1_State & (GlobalTickCount >= (POWERON_PERIOD / 6))) Flicker_Led2Count++;
    else {
        Flicker_Led2Count = 0;
        Led2_State = 0;
    }
    if (Btn1_State & (GlobalTickCount >= (2 * POWERON_PERIOD / 6))) Flicker_Led3Count++;
    else {
        Flicker_Led3Count = 0;
        Led3_State = 0;
    }
    if (Btn1_State & (GlobalTickCount >= (3 * POWERON_PERIOD / 6))) Flicker_Led4Count++;
    else {
        Flicker_Led4Count = 0;
        Led4_State = 0;
    }
    if (Btn1_State & (GlobalTickCount >= (4 * POWERON_PERIOD / 6))) Flicker_Led5Count++;
    else {
        Flicker_Led5Count = 0;
        Led5_State = 0;
    }
    if (Btn1_State & (GlobalTickCount >= (5 * POWERON_PERIOD / 6))) Flicker_Led6Count++;
    else {
        Flicker_Led6Count = 0;
        Led6_State = 0;
    }

    // А теперь настраиваем включение и выключение каждого светодиода
    // PC6 - 1-ый светодиод
    if ((Flicker_Led1Count > 0) & (Flicker_Led1Count < Flicker_Led1Frequency)) {
        SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BS6);
        Led1_State = 1;
    }
    else if ((Flicker_Led1Count >= Flicker_Led1Frequency) & ((Flicker_Led1Count < 2 * Flicker_Led1Frequency))) {
        SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR6);
        Led1_State = 0;
    }
    else {
        Flicker_Led1Count = 0;
        Led1_State = 0;
    }
    // PC7 - 2-ой светодиод
    if ((Flicker_Led2Count > 0) & (Flicker_Led2Count < Flicker_Led2Frequency)) {
        SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BS7);
        Led2_State = 1;
    }
    else if ((Flicker_Led2Count >= Flicker_Led2Frequency) & ((Flicker_Led2Count < 2 * Flicker_Led2Frequency))) {
        SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR7);
        Led2_State = 0;
    }
    else {
        Flicker_Led2Count = 0;
        Led2_State = 0;
    }    
    // PB5 - 3-ий светодиод
    if ((Flicker_Led3Count > 0) & (Flicker_Led3Count < Flicker_Led3Frequency)) {
        SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BS5); 
        Led3_State = 1;
    }
    else if ((Flicker_Led3Count >= Flicker_Led3Frequency) & ((Flicker_Led3Count < 2 * Flicker_Led3Frequency))) {
        SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR5); 
        Led3_State = 0;
    }
    else {
        Flicker_Led3Count = 0;
        Led3_State = 0;
    }
    // PB3
    if ((Flicker_Led4Count > 0) & (Flicker_Led4Count < Flicker_Led4Frequency)) {
        SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BS3); 
        Led4_State = 1;
    }
    else if ((Flicker_Led4Count >= Flicker_Led4Frequency) & ((Flicker_Led4Count < 2 * Flicker_Led4Frequency))) {
        SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR3); 
        Led4_State = 0;
    }
    else {
        Flicker_Led4Count = 0;
        Led4_State = 0;
    }
    //PA4
    if ((Flicker_Led5Count > 0) & (Flicker_Led5Count < Flicker_Led5Frequency)) {
        SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BS4);
        Led5_State = 1;
    }
    else if ((Flicker_Led5Count >= Flicker_Led5Frequency) & ((Flicker_Led5Count < 2 * Flicker_Led5Frequency))) {
        SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BR4);
        Led5_State = 0;
    }
    else {
        Flicker_Led5Count = 0;
        Led5_State = 0;
    }
    // PB4
    if ((Flicker_Led6Count > 0) & (Flicker_Led6Count < Flicker_Led6Frequency)) {
        SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BS4);
        Led6_State = 1;
    }
    else if ((Flicker_Led6Count >= Flicker_Led6Frequency) & ((Flicker_Led6Count < 2 * Flicker_Led6Frequency))) {
        SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR4);
        Led6_State = 0;
    }
    else {
        Flicker_Led6Count = 0;
        Led6_State = 0;
    }
    ExternInterruptTickCount++;

}

// Прерывание-задержка
uint16_t DelayTickCount;

void User_Delay(uint32_t delay){
    while(DelayTickCount < delay){} //Цикл, благодаря которому происходит задержка программы
    if(DelayTickCount >= delay) DelayTickCount = 0; //Обнуление переменной
}
