#include "init.h"
#include "it_handlers.h"
#define FREQUENCY_0_5_HZ_MS         1000 / 0.5  // 2000 мс      = 2 с
#define FREQUENCY_1_7_HZ_MS         1000 / 1.7  // 588 мс       = 0,588 с
#define FREQUENCY_2_2_HZ_MS         1000 / 2.2  // 454,55 мс    = 0,455 с
#define FLICKER_PERIOD_0_3_HZ_MS    1000 / 0.3  // 3333 мс      = 3,3 c
#define FLICKER_PERIOD_0_8_HZ_MS    1000 / 0.8  // 1250 мс      = 1,25 с
#define FLICKER_PERIOD_1_4_HZ_MS    1000 / 1.4  // 714,29 мс    = 0,714 с

uint32_t GlobalTickCount;
uint8_t LedState = 0;
uint8_t Btn1_State = 0;
uint8_t Btn2_State = 0;
uint8_t Btn3_State = 0;
uint8_t PairLedState = 0;
uint8_t Led1_State = 0;
uint8_t Led2_State = 0;
uint8_t Led3_State = 0;
uint8_t Led4_State = 0;
uint8_t Led5_State = 0;
uint8_t Led6_State = 0;

long Flicker_Led1Frequency = FLICKER_PERIOD_1_4_HZ_MS;
long Flicker_Led2Frequency = FLICKER_PERIOD_1_4_HZ_MS;
long Flicker_Led3Frequency = FLICKER_PERIOD_1_4_HZ_MS;
long Flicker_Led4Frequency = FLICKER_PERIOD_1_4_HZ_MS;
long Flicker_Led5Frequency = FLICKER_PERIOD_1_4_HZ_MS;
long Flicker_Led6Frequency = FLICKER_PERIOD_1_4_HZ_MS;

int main(void) {
    GPIO_Init();
    RCC_Init();
    ITR_Init();
    SysTick_Init();

    while (1){
        if (!Btn1_State){
            Flicker_Led1Frequency = FLICKER_PERIOD_0_3_HZ_MS;
            Flicker_Led2Frequency = FLICKER_PERIOD_0_3_HZ_MS;
            Flicker_Led3Frequency = FLICKER_PERIOD_0_3_HZ_MS;
            Flicker_Led4Frequency = FLICKER_PERIOD_0_3_HZ_MS;
            Flicker_Led5Frequency = FLICKER_PERIOD_0_3_HZ_MS;
            Flicker_Led6Frequency = FLICKER_PERIOD_0_3_HZ_MS;
            if (Btn2_State == 0){
                if (GlobalTickCount == 0){
                    SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR6);
                    SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR7);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR5);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR3);
                    SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BR4);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR4);
                    continue;
                }
                if ((GlobalTickCount > 0) & (GlobalTickCount <= FREQUENCY_0_5_HZ_MS)){
                    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR4);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS5);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS3);
                    PairLedState = 0;
                }
                else if (GlobalTickCount <= 2 * FREQUENCY_0_5_HZ_MS){
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR5);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR3);
                    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS7);
                    SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS4);
                    PairLedState = 1;

                } 
                else if (GlobalTickCount <= 3 * FREQUENCY_0_5_HZ_MS) {
                    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR7);
                    SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR4);
                    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS4);
                    PairLedState = 2;
                }
                else {
                    GlobalTickCount = 0;
                    SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR6);
                    SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR7);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR5);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR3);
                    SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BR4);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR4);
                }
            }
            if (Btn2_State == 1){
                if (GlobalTickCount == 0){
                    SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR6);
                    SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR7);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR5);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR3);
                    SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BR4);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR4);
                    continue;
                }
                if ((GlobalTickCount > 0) & (GlobalTickCount <= FREQUENCY_1_7_HZ_MS)){
                    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR4);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS5);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS3);
                    PairLedState = 0;
                }
                else if (GlobalTickCount <= 2 * FREQUENCY_1_7_HZ_MS){
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR5);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR3);
                    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS7);
                    SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS4);
                    PairLedState = 1;
                } 
                else if (GlobalTickCount <= 3 * FREQUENCY_1_7_HZ_MS) {
                    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR7);
                    SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR4);
                    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS4);
                    PairLedState = 2;
                }
                else {
                    GlobalTickCount = 0;
                    SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR6);
                    SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR7);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR5);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR3);
                    SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BR4);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR4);
                }
            }
            if (Btn2_State == 2){
                if (GlobalTickCount == 0){
                    SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR6);
                    SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR7);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR5);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR3);
                    SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BR4);
                    SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR4);
                    continue;
                }
                if ((GlobalTickCount > 0) & (GlobalTickCount <= FREQUENCY_2_2_HZ_MS)){
                    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR4);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS5);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS3);
                    PairLedState = 0;
                }
                else if (GlobalTickCount <= 2 * FREQUENCY_2_2_HZ_MS){
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR5);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR3);
                    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS7);
                    SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS4);
                    PairLedState = 1;
                } 
                else if (GlobalTickCount <= 3 * FREQUENCY_2_2_HZ_MS) {
                    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR7);
                    SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR4);
                    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6);
                    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS4);
                    PairLedState = 2;
                }
                else {
                    GlobalTickCount = 0;
                }
            }
            // В 1-ом режиме надо включить 3-ий и 4-ый светодиод
            // потом их выключить и включить 2-ой и 5-ый
            // затем 1-ый и 6-ой
        }
        else{
            if (GlobalTickCount == 0){
                SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR6);
                SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR7);
                SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR5);
                SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR3);
                SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BR4);
                SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR4);
                continue;
            }
            if (Btn3_State == 0) {
                if (Btn2_State == 0) Flicker_Led1Frequency = FLICKER_PERIOD_0_3_HZ_MS;
                else if (Btn2_State == 1) Flicker_Led1Frequency = FLICKER_PERIOD_0_8_HZ_MS;
                else Flicker_Led1Frequency = FLICKER_PERIOD_1_4_HZ_MS;
            }
            else if (Btn3_State == 1){
                if (Btn2_State == 0) Flicker_Led2Frequency = FLICKER_PERIOD_0_3_HZ_MS;
                else if (Btn2_State == 1) Flicker_Led2Frequency = FLICKER_PERIOD_0_8_HZ_MS;
                else Flicker_Led2Frequency = FLICKER_PERIOD_1_4_HZ_MS;
            }
            else if (Btn3_State == 2){
                if (Btn2_State == 0) Flicker_Led3Frequency = FLICKER_PERIOD_0_3_HZ_MS;
                else if (Btn2_State == 1) Flicker_Led3Frequency = FLICKER_PERIOD_0_8_HZ_MS;
                else Flicker_Led3Frequency = FLICKER_PERIOD_1_4_HZ_MS;
            }
            else if (Btn3_State == 3){
                if (Btn2_State == 0) Flicker_Led4Frequency = FLICKER_PERIOD_0_3_HZ_MS;
                else if (Btn2_State == 1) Flicker_Led4Frequency = FLICKER_PERIOD_0_8_HZ_MS;
                else Flicker_Led4Frequency = FLICKER_PERIOD_1_4_HZ_MS;
            }
            else if (Btn3_State == 4){
                if (Btn2_State == 0) Flicker_Led5Frequency = FLICKER_PERIOD_0_3_HZ_MS;
                else if (Btn2_State == 1) Flicker_Led5Frequency = FLICKER_PERIOD_0_8_HZ_MS;
                else Flicker_Led5Frequency = FLICKER_PERIOD_1_4_HZ_MS;
            }
            else if (Btn3_State == 5){
                if (Btn2_State == 0) Flicker_Led6Frequency = FLICKER_PERIOD_0_3_HZ_MS;
                else if (Btn2_State == 1) Flicker_Led6Frequency = FLICKER_PERIOD_0_8_HZ_MS;
                else Flicker_Led6Frequency = FLICKER_PERIOD_1_4_HZ_MS;
            }
            //if (Btn2_State == 0) FlickerFrequency = FLICKER_PERIOD_0_3_HZ_MS;
            //else if (Btn2_State == 1) FlickerFrequency = FLICKER_PERIOD_0_8_HZ_MS;
            //else FlickerFrequency = FLICKER_PERIOD_1_4_HZ_MS;
            if (GlobalTickCount == 1){
                SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BS6);
            }
            if (GlobalTickCount  == POWERON_PERIOD / 6){
                SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BS7);
            }
            if ((GlobalTickCount  == 2 * POWERON_PERIOD / 6)){
                SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BS5);  
            } 
            if (GlobalTickCount  == 3 * POWERON_PERIOD / 6){
                SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BS3);
            }  
            if (GlobalTickCount  == 4 * POWERON_PERIOD / 6){
                SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BS4);
            }   
            if (GlobalTickCount  == 5 * POWERON_PERIOD / 6){
                SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BS4);
            } 
            if (GlobalTickCount  == 6 * POWERON_PERIOD / 6) {
                GlobalTickCount = 0;
                SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR6);
                SET_BIT(GPIOC->BSRR,  GPIO_BSRR_BR7);
                SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR5);
                SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR3);
                SET_BIT(GPIOA->BSRR,  GPIO_BSRR_BR4);
                SET_BIT(GPIOB->BSRR,  GPIO_BSRR_BR4);
            }
        }
    }
}

   