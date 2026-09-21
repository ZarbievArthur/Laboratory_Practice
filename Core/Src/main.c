#include "init.h"

uint8_t flag;

int main(void) {

    GPIO_Ini();
    while(1){
        if(READ_BIT(GPIOC->IDR, GPIO_IDR_IDR_13) != 0){
            flag = 1;
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7); 
        }
        else{
            flag = 0;
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7); 
        }
    }
}