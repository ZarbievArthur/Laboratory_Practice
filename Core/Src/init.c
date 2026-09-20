#include "init.h"

void GPIO_Ini(void){
    RCC_GPIO_EN |= RCC_GPIOB_EN + RCC_GPIOC_EN; //Power on B and C ports

    GPIOB_MODER     |= GPIOB_MODE_PIN7_OUT; // Output Mode 
    GPIOB_OTYPER    |=  GPIOB_OTYPE_PIN7_PP; // Push-pull register
    GPIOB_OSPEEDR   |= GPIOB_OSPEED_PIN7_MID;  // Message's speed
    GPIOB_PUPDR     |= GPIOB_PUPDR_PIN7_NOPUPD; // No pull-up, pull-down
 
    GPIOC_MODER     |= GPIOC_MODE_PIN7_IN;   // Need setting Input/Output mode
    GPIOC_OTYPER    |= GPIOC_OTYPE_PIN7_PP;  // push-pull register
    GPIOC_OSPEEDR   |= GPIOC_OSPEED_PIN7_MID;    // Message's speed
    GPIOC_PUPDR     |= GPIOC_PUPDR_PIN7_NOPUPD;    // Pull-up or pull-down
}