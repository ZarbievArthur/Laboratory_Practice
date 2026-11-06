#include "init.h"


/** 
 * @brief Инициализация системы тактирования
*/
void RCC_Init(void){
    // Так как мы будем использовать внутренний RC-генератор
    // на 16 МГц. Корректировать нужную частоту будем
    // через биты HSITRIM
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U); 
    // В какой регистр заносим, в какой область (она очищается), что заносим

    //Как только очистили, настраиваем делители частот
    CLEAR_REG(RCC->CFGR);
    while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET);
    //Очищаем бит, который отвечает за включение
    // модуля PLL
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
    // Мы ждем, пока "1" в PLLON не установится в "0" (если такое есть, конечно)
    while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET);
    // Выключаем внешний кварцевый генератор
    // и систему контроля кварцевого генератора
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
    // Ждем, пока не произойдет сброс бита, который отвечает 
    // За включение HSE
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET);
    // Также выключаем внешний генератор частоты
    // (не кварцевый резонатор)
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);


    // Теперь начинаем настраивать первый регистр RCC_CR
    // Который отвечает за настройку тактового генератора

    // Запускаем внешний кварцевый резонатор
    SET_BIT(RCC->CR, RCC_CR_HSEON); 
    // И ждем, пока он запустится
    while(READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET);
    // Еще раз очищаем бит, отвечающий за включение
    // внешнего генератора частота, несвязанного
    // с кварцевым резонатором
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP); 
    // Ну и запускаем систему контроля частоты
    // кварцевого резонатора
    SET_BIT(RCC->CR, RCC_CR_CSSON);
    // Перед тем, как настраивать модуль PLL
    // То есть устанавливать бит PLLON в 1,
    // надо настроить регистр RCC_PLLCFGR


    // Настройка регистра RCC_PLLCFGR
    // Сначала очищаем регистр
    CLEAR_REG(RCC->PLLCFGR);
    // Говорим, что источником питания PLL будет
    // HSE (кварцевый генератор) - частота 4 МГц
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);
    // Делим входную частоту на 4 (частота 1 МГц)
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM, RCC_PLLCFGR_PLLM_2);
    // Умножаем частоту на PLL на 180 (частота 180 МГц)
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, RCC_PLLCFGR_PLLN_2 | RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7);
    // Делим получившуюся частоту на 2 (частота 90 МГц)
    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk);
    // Запускаем модуль PLL!
    SET_BIT(RCC->CR, RCC_CR_PLLON);
    // Ждем запуска PLL
    while(READ_BIT(RCC->CR, RCC_CR_PLLRDY));


    
    // Настройка регистра RCC_CFGR (отвечает 
    // за тактирование периферии)
    // Выбираем PLL в качестве системной частоты (System clock)
    // Чтобы это сделать необходимо занести значение 10 в
    // бит RCC_CFGR_SW
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
    // Теперь настраиваем предделитель шины AHB1
    // В нашем случае делим на 1, так как делить частоту нет 
    // необходимости
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1); 
    // Теперь настраиваем предделитель для шины APB1 
    // В нашем случае мы захотели поделить на 4 (45 МГц), т. к. на эту шину нельзя подавать большую частоту
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV4);
    // Теперь настраиваем частоту для шины APB2.
    // В нашем случае делим на 2 (90 МГц), т. к. на эту шину нельзя подавать большую частоту
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV2); 
    // Теперь настраиваем предделитель для выхода
    // MCO2 (PC9). В нашем случае предделитель равен 5,
    // чтобы осциллограф смог считать значения
    MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO2PRE, RCC_CFGR_MCO2PRE_Msk);
    // Ну и теперь настраиваем источник тактирования MCO2 на
    // System CLock
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_MCO2);


    // Последнее, что надо настроить - количество циклов задержки при 
    // обращении к FLASH-памяти.
    // Так как напряжение на контроллере 3.3 В, а частота HCLK 
    // (которая, как я понял, численно равна системной частоте) 180 МГц,
    // то нам надо установить 5 wait states (6 CPU циклов задержки)
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_5WS);
}