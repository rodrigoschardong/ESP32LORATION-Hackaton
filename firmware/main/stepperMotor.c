#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "stepperMotor.h"

int stepPins[4];

void configStepperMotor(int pin1, int pin2, int pin3, int pin4) {

    stepPins[0] = pin1;
    stepPins[1] = pin2;
    stepPins[2] = pin3;
    stepPins[3] = pin4;

    gpio_set_direction(stepPins[0],GPIO_MODE_OUTPUT);
    gpio_set_direction(stepPins[1],GPIO_MODE_OUTPUT);
    gpio_set_direction(stepPins[2],GPIO_MODE_OUTPUT);
    gpio_set_direction(stepPins[3],GPIO_MODE_OUTPUT);
}

void stepCounterclockwise(int steps) {
    for(int j = 0; j < 400; j++) {
        for(int i = 0; i < 4; i++) {
            gpio_set_level(stepPins[0], i == 0);
            gpio_set_level(stepPins[1], i == 1);
            gpio_set_level(stepPins[2], i == 2);
            gpio_set_level(stepPins[3], i == 3);
            vTaskDelay(15 / portTICK_PERIOD_MS); 
        }
    }

    gpio_set_level(stepPins[0], 0);
    gpio_set_level(stepPins[1], 0);
    gpio_set_level(stepPins[2], 0);
    gpio_set_level(stepPins[3], 0);
}

void stepClockwise(int steps) {
    for(int j = 0; j < 400; j++) {
        for(int i = 0; i < 4; i++) {
            gpio_set_level(stepPins[0], i == 3);
            gpio_set_level(stepPins[1], i == 2);
            gpio_set_level(stepPins[2], i == 1);
            gpio_set_level(stepPins[3], i == 0);
            vTaskDelay(15 / portTICK_PERIOD_MS); 
        }
    }

    gpio_set_level(stepPins[0], 0);
    gpio_set_level(stepPins[1], 0);
    gpio_set_level(stepPins[2], 0);
    gpio_set_level(stepPins[3], 0);
}