/*
    Rodrigo Schardong
    Esp32 - Blinkled
*/

#include "driver/gpio.h"    //Get pin defines
#include "driver/timer.h"   //Get timer defines

#define RED_LED_PIN GPIO_NUM_32
#define GREEN_LED_PIN GPIO_NUM_33

#define TIMER_INTR_SEL TIMER_INTR_LEVEL                                  // Timer level interrupt
#define TIMER_GROUP    TIMER_GROUP_0                                     // Test on timer group 0 
#define TIMER_DIVIDER   80                                               // Hardware timer clock divider, 80 to get 1MHz clock to timer
#define TIMER_SCALE    (TIMER_BASE_CLK / TIMER_DIVIDER)                  // Used to calculate counter value 
#define TIMER_FINE_ADJ   (0*(TIMER_BASE_CLK / TIMER_DIVIDER)/1000000)    // Used to compensate alarm value

static uint8_t * pTimerCounter;

void IRAM_ATTR timer_group0_isr(void *para){    // Timer group 0, ISR
    int timer_idx = (int) para;

    *pTimerCounter = *pTimerCounter + 1;

    TIMERG0.hw_timer[timer_idx].update = 1;
    TIMERG0.int_clr_timers.t0 = 1;
    TIMERG0.hw_timer[timer_idx].config.alarm_en = 1;
}

void tg0_timer0_init(uint32_t timeInSecs, uint8_t *timerCounter)
{
    pTimerCounter = timerCounter;
    int timer_group = TIMER_GROUP_0;
    int timer_idx = TIMER_0;
    timer_config_t config;
    config.alarm_en = 1;
    config.auto_reload = 1;
    config.counter_dir = TIMER_COUNT_UP;
    config.divider = TIMER_DIVIDER;
    config.intr_type = TIMER_INTR_SEL;
    config.counter_en = TIMER_PAUSE;
    
    // Configure timer
    timer_init(timer_group, timer_idx, &config);
    // Stop timer counter
    timer_pause(timer_group, timer_idx);
    // Load counter value
    timer_set_counter_value(timer_group, timer_idx, 0x00000000ULL);
    // Set alarm value
    timer_set_alarm_value(timer_group, timer_idx, (timeInSecs * TIMER_SCALE) - TIMER_FINE_ADJ);
    // Enable timer interrupt
    timer_enable_intr(timer_group, timer_idx);
    // Set ISR handler
    timer_isr_register(timer_group, timer_idx, timer_group0_isr, (void*) timer_idx, ESP_INTR_FLAG_IRAM, NULL);
    // Start timer counter
    timer_start(timer_group, timer_idx);
}
