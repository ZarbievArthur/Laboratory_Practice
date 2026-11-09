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

 /**
  * @brief Настройка тактирования периферии GPIO
  */
void GPIO_Init(void){
    // Включаем тактирование портов A, B, C
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);
    
    // Настраиваем светодиоды на выход
    MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODER6_Msk, GPIO_MODER_MODER6_0); // PC6
    MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODER7_Msk, GPIO_MODER_MODER7_0); // PC7
    MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER5_Msk, GPIO_MODER_MODER5_0); // PB5
    MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER3_Msk, GPIO_MODER_MODER3_0); // PB3
    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER4_Msk, GPIO_MODER_MODER4_0); // PA4
    MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER4_Msk, GPIO_MODER_MODER4_0); // PB4
    // Настраиваем кнопки на вход
    CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODER15_Msk);                      // PB15
    CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODER13_Msk);                      // PB13
    CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODER12_Msk);                      // PB12
 
    // Регистр OTYPER для светодиодов в Push-Pull
    CLEAR_BIT(GPIOC->OTYPER, GPIO_OTYPER_OT6);
    CLEAR_BIT(GPIOC->OTYPER, GPIO_OTYPER_OT7);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT5);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT3);
    CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT4);
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT4);

    // Настраиваем скорость работы каждого светодиода. Средняя скорость - 01
    MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED6_Msk, GPIO_OSPEEDR_OSPEED6_0);
    MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED7_Msk, GPIO_OSPEEDR_OSPEED7_0);
    MODIFY_REG(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED5_Msk, GPIO_OSPEEDR_OSPEED5_0);
    MODIFY_REG(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED3_Msk, GPIO_OSPEEDR_OSPEED3_0);
    MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED4_Msk, GPIO_OSPEEDR_OSPEED4_0);
    MODIFY_REG(GPIOB->OSPEEDR, GPIO_OSPEEDR_OSPEED4_Msk, GPIO_OSPEEDR_OSPEED4_0);


    // Убираем подтягивающие резисторы как у светодиодов
    // У кнопок ставим стягивающие  резисторы (Pull Down)
    CLEAR_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPD6_Msk);
    CLEAR_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPD7_Msk);
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPD5_Msk);
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPD3_Msk);
    CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD4_Msk);
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPD4_Msk);
    //
    MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPD15_Msk, GPIO_PUPDR_PUPD15_1);
    MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPD13_Msk, GPIO_PUPDR_PUPD13_1);
    MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPD12_Msk, GPIO_PUPDR_PUPD12_1);
 }

/**
*   @brief Настройка регистров прерывания
*/
void ITR_Init(void){
    // Необходимо включить тактирование периферии SYSCFG,
    // данная периферия получает тактирование от шины APB2.
    // Включаем периферию SYSCFG!
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN); 
    // Так как именно на этой периферии находится регистр
    // SYSCFG_EXTICR, который отвечает за нужный мультиплексор прерываний
    // Больше про этот регистр можно прочитать на странице 294 в мануале
    // а если кратенько, тот внутри каждого такого регистра можно настроить пин
    // одного номера для каждого порта. Короче, проще в мануале посмотреть
    MODIFY_REG(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI12_Msk | SYSCFG_EXTICR4_EXTI13_Msk | SYSCFG_EXTICR4_EXTI15_Msk, SYSCFG_EXTICR4_EXTI12_PB | SYSCFG_EXTICR4_EXTI13_PB | SYSCFG_EXTICR4_EXTI15_PB);
 

    // Теперь можно настраивать регистры EXTI
    // Сначала настраивается маскирование в регистре EXTI_IMR для пинов 12, 13, 15
    SET_BIT(EXTI->IMR, EXTI_IMR_MR12);
    SET_BIT(EXTI->IMR, EXTI_IMR_MR13);
    SET_BIT(EXTI->IMR, EXTI_IMR_MR15);
    // Затем настраиваем детектор пока что только для фронта, потому что прерывание будет
    // возникать только при нажатии на кнопку для нивелирования дребезга
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR12);
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR13);
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR15);
    //SET_BIT(EXTI->FTSR, EXTI_FTSR_TR13);


    // После настройки внешнего контроллера прерываний
    // переходим к настройке регистров внутреннего контроллера
    // прерываний NVIC
    NVIC_SetPriority(EXTI15_10_IRQn,
         NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    // А теперь по порядку разбираемм, что вверху происходит
    // Первое: функция  NVIC_SetPriority устанавливает приоритет выполнения соответствующему прерыванию
    // В нее необходимо передать номер прерывания и приоритет, который мы присвоим прерыванию
    // Мы выбрали номер EXTI15_10_IRQn для прерывания, так как именно это прерывания отвечает за пин 13 для каждого порта
    // Далее мы вызываем функцию NVIC_EncodePriorit, которая выбирает приоритет без конфликтов
    // в нее мы передаем все возможные приоритеты (функция NVIC_GetPriorityGrouping()) (от 1 до 15)
    // Далее мы передаем номер приоритета, который можно остановить выполненение обработчика нашего прерывания
    // и последний аргумент - это значение субприоритета. Я так и не понял, что это
    // Применив такую конфигурацию, мы точно можем сказать, что наше прерывание может прервать только прерывания самой
    // системы, которое вызывается самим процессором (?)

    // Ну и теперь прерывание нужно сделать активным
    NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/**
 * @brief Настройка системного таймера
 */
void SysTick_Init(void){
    // На всякий случай отключаем таймер
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
    //Разрешаем генерерировать прерывания 
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
    // Устанавливаем источник тактирования
    // AHB без деления
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);
    // Устанавливаем начальное значение таймера
    // Так как частота AHB составляет 180 МГц, а
    // мы хотим, чтобы частота срабатываниия таймемра
    // была, например, 1 кГц, мы должны
    // установить значение 180 000 - 1
    MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, 179999 << SysTick_LOAD_RELOAD_Pos);
    // Очищаем текущее значение таймера, а вместе
    // с этим очищает и флаг COUNTFLAG в
    // регистре STK_CTRL
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
}
