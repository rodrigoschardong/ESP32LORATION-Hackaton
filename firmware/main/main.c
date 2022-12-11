#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_spi_flash.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "stepperMotor.h"
#include "ultrasonic.h"

#include "esp_log.h"

#include "adc.h"
#include "timer.h"

#define PIN1 27
#define PIN2 26
#define PIN3 25
#define PIN4 33
#define SCALE 34

static const char *TAG = "ADC";
static bool shouldLog = false;

#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

const uint32_t timerValueSeconds = 1800;


void app_main(void)
{   
    dogFeederData_t dogFeederData;
    wifi_start();
    //double weight;
    // configStepperMotor: setup the pins as output and save them for future use
    configStepperMotor(PIN1, PIN2, PIN3, PIN4);
    initADC();
    
    uint8_t timerCounter = 0;
    //Init Timer
    tg0_timer0_init(timerValueSeconds, &timerCounter);
    // stepCounterclockwise: steps motor for the given number of steps in counterclockwise direction
    stepCounterclockwise(500);

    // delay to stop for a second.
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // stepClockWise: steps motor for the given number of steps in clockwise direction
    stepClockwise(500);

    ultrasonicHandler(&dogFeederData);

    while(true) {
        dogFeederData.readUltrasonic = 1;
        printf("percentage: %.2f%% \n", dogFeederData.percentageFull);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    // while (1) 
    // {
    //     weight = readWeight(128);
    //     if (shouldLog) {
    //         ESP_LOGI(TAG, "ADC1_CHANNEL_6: %f kg", weight);
    //     }

    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }
}

