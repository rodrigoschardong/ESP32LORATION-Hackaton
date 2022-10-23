#include <string.h>
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

#define PIN1 27
#define PIN2 26
#define PIN3 25
#define PIN4 33

#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "wifi station";

void app_main(void)
{   
    wifi_start();
    // configStepperMotor: setup the pins as output and save them for future use
    configStepperMotor(PIN1, PIN2, PIN3, PIN4);

    // stepCounterclockwise: steps motor for the given number of steps in counterclockwise direction
    stepCounterclockwise(500);

    // delay to stop for a second.
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // stepClockWise: steps motor for the given number of steps in clockwise direction
    stepClockwise(500);
}

