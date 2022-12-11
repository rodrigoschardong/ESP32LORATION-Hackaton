#ifndef BUZZER_BITTER_H
#define BUZZER_BITTER_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "esp_system.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "driver/gpio.h"
#include "driver/ledc.h"

void configBuzzerGeneral (void);
void changeFreq (uint32_t t_NewFreq);
void soundOn (uint32_t t_Freq,uint32_t t_Duration);
void playBeep (bool *t_flag);
void soundHandler (bool *t_play);

#endif