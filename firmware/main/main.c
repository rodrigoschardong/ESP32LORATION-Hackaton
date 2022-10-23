/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "stepperMotor.h"
#include "ultrasonic.h"

#define PIN1 27
#define PIN2 26
#define PIN3 25
#define PIN4 33

uint8_t flag;

void app_main(void)
{   
    // configStepperMotor: setup the pins as output and save them for future use
    configStepperMotor(PIN1, PIN2, PIN3, PIN4);

    // stepCounterclockwise: steps motor for the given number of steps in counterclockwise direction
    stepCounterclockwise(500);

    // delay to stop for a second.
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // stepClockWise: steps motor for the given number of steps in clockwise direction
    stepClockwise(500);

    ultrasonicHandler(&flag);

    printf("%d", flag);
}

