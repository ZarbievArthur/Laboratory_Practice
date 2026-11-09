// Здесь будут описаны все прерывания, которые мы определим
#include "it_handlers.h"

// Исключение при нажатии кнопка
extern uint8_t LedState;            // Хранит состояние светодиода во втором режиме. Нужна для построения графиков
extern uint32_t GlobalTickCount;    // Счетчик, который отвечает за включение и выключение светодиодов
extern long FlickerFrequency;       // Частота мигания светодиода - задается в main.c
extern uint8_t Btn1_State;          // Состояние первой кнопки
extern uint8_t Btn2_State;          // Состояние второй кнопки
extern uint8_t Btn3_State;          // Состояние третьей кнопки


uint32_t FlickerCount;              // Счетчик для включения и выключения светодиода
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
    if (Btn1_State & (GlobalTickCount >= (Btn3_State * POWERON_PERIOD / 6))) FlickerCount++;
    else {
        FlickerCount = 0;
        LedState = 0;
    }
    if ((FlickerCount > (Btn3_State * POWERON_PERIOD / 6)) & (FlickerCount  <= (FlickerFrequency + Btn3_State * POWERON_PERIOD / 6))){
        if (Btn3_State == 0) {
            SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BS6);
            if (GlobalTickCount > ((5 * POWERON_PERIOD / 6))) SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BS4);
        }
        else if (Btn3_State == 1) {
            SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BS7);
            SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BS6);   //Дублирование включения светодиода нужно, чтобы не возникало проблемы выключенноого светодиода
        }
        else if (Btn3_State == 2) {
            SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BS5); 
            SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BS7);
        }
        else if (Btn3_State == 3) {
            SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BS3);
            SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BS5);
        }
        else if (Btn3_State == 4) {
            SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BS4);
            SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BS3);
        }
        else {
            SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BS4);
            SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BS4);
        } 
        LedState = 1;
    }
    else if ((FlickerCount > (Btn3_State * POWERON_PERIOD / 6)) & (FlickerCount <= (2 * FlickerFrequency + Btn3_State * POWERON_PERIOD / 6))){
        if (Btn3_State == 0) SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR6);
        else if (Btn3_State == 1) SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR7);
        else if (Btn3_State == 2) SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR5); 
        else if (Btn3_State == 3) SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR3);
        else if (Btn3_State == 4) SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BR4);
        else SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR4); 
        LedState = 0;
    }
    else {
        FlickerCount = Btn3_State * POWERON_PERIOD / 6;
    }
    ExternInterruptTickCount++;

}

// Прерывание-задержка
uint16_t DelayTickCount;

void User_Delay(uint32_t delay){
    while(DelayTickCount < delay){} //Цикл, благодаря которому происходит задержка программы
    if(DelayTickCount >= delay) DelayTickCount = 0; //Обнуление переменной
}
