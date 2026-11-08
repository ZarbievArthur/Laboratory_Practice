#include "init.h"
#include "it_handlers.h"

#define FLICKER_PERIOD 2000
#define FLICKER_SEMIPERIOD 1000

uint16_t GlobalTickCount;
uint8_t LedState = 0;

int main(void) {
    GPIO_Init();
    RCC_Init();
    ITR_Init();
    SysTick_Init();

    while (1){
        if(LedState) SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);
        else SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7);
        /*
        if(GlobalTickCount >= FLICKER_PERIOD / 2){ //Если прошло 1000 прерываний системного таймера = 1 секунда
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7); //Включаем светодиод
            if(GlobalTickCount >= FLICKER_PERIOD){ //Если прошло 2000 прерываний системного таймера = 2 секунды
                GlobalTickCount = 0; //Обнуляем переменную счётчика системного таймера
            }
        }
        else{
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7); // Иначе выключаем светодиод
        }
        */
    
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7); //Включаем светодиод
        User_Delay(FLICKER_SEMIPERIOD); //Ожидаем 1 секунду
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7); //Выключаем свет   
        User_Delay(FLICKER_SEMIPERIOD);
    }
}

   