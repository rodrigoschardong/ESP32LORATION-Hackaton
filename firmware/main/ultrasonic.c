/*
 * Copyright (c) 2016 Ruslan V. Uss <unclerus@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of itscontributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file ultrasonic.c
 *
 * ESP-IDF driver for ultrasonic range meters, e.g. HC-SR04, HY-SRF05 and the like
 *
 * Ported from esp-open-rtos
 *
 * Copyright (c) 2016 Ruslan V. Uss <unclerus@gmail.com>
 *
 * BSD Licensed as described in the file LICENSE
 */
#include "esp_idf_lib_helpers.h"
#include "ultrasonic.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include <esp_err.h>

#define TRIGGER_LOW_DELAY 4
#define TRIGGER_HIGH_DELAY 10
#define PING_TIMEOUT 6000
#define ROUNDTRIP_M 5800.0f
#define ROUNDTRIP_CM 58
#define ROUNDTRIP_MM 5.8

#if HELPER_TARGET_IS_ESP32
static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
#define PORT_ENTER_CRITICAL portENTER_CRITICAL(&mux)
#define PORT_EXIT_CRITICAL portEXIT_CRITICAL(&mux)

#elif HELPER_TARGET_IS_ESP8266
#define PORT_ENTER_CRITICAL portENTER_CRITICAL()
#define PORT_EXIT_CRITICAL portEXIT_CRITICAL()

#else
#error cannot identify the target
#endif

#define timeout_expired(start, len) ((esp_timer_get_time() - (start)) >= (len))

#define CHECK_ARG(VAL) do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)
#define CHECK(x) do { esp_err_t __; if ((__ = x) != ESP_OK) return __; } while (0)
#define RETURN_CRITICAL(RES) do { PORT_EXIT_CRITICAL; return RES; } while(0)


esp_err_t ultrasonic_init(const ultrasonic_sensor_t *dev)
{
    CHECK_ARG(dev);

    CHECK(gpio_set_direction(dev->trigger_pin, GPIO_MODE_OUTPUT));
    CHECK(gpio_set_direction(dev->echo_pin, GPIO_MODE_INPUT));

    return gpio_set_level(dev->trigger_pin, 0);
}


esp_err_t ultrasonic_measure_raw(const ultrasonic_sensor_t *dev, uint32_t max_time_us, uint32_t *time_us)
{
    CHECK_ARG(dev && time_us);

    PORT_ENTER_CRITICAL;

    // Ping: Low for 2..4 us, then high 10 us
    CHECK(gpio_set_level(dev->trigger_pin, 0));
    ets_delay_us(TRIGGER_LOW_DELAY);
    CHECK(gpio_set_level(dev->trigger_pin, 1));
    ets_delay_us(TRIGGER_HIGH_DELAY);
    CHECK(gpio_set_level(dev->trigger_pin, 0));

    // Previous ping isn't ended
    if (gpio_get_level(dev->echo_pin))
        RETURN_CRITICAL(ESP_ERR_ULTRASONIC_PING);

    // Wait for echo
    int64_t start = esp_timer_get_time();
    while (!gpio_get_level(dev->echo_pin))
    {
        if (timeout_expired(start, PING_TIMEOUT))
            RETURN_CRITICAL(ESP_ERR_ULTRASONIC_PING_TIMEOUT);
    }

    // got echo, measuring
    int64_t echo_start = esp_timer_get_time();
    int64_t time = echo_start;
    while (gpio_get_level(dev->echo_pin))
    {
        time = esp_timer_get_time();
        if (timeout_expired(echo_start, max_time_us))
            RETURN_CRITICAL(ESP_ERR_ULTRASONIC_ECHO_TIMEOUT);
    }
    PORT_EXIT_CRITICAL;

    *time_us = time - echo_start;

    return ESP_OK;
}

esp_err_t ultrasonic_measure(const ultrasonic_sensor_t *dev, float max_distance, float *distance)
{
    CHECK_ARG(dev && distance);

    uint32_t time_us;
    CHECK(ultrasonic_measure_raw(dev, max_distance * ROUNDTRIP_M, &time_us));
    *distance = time_us / ROUNDTRIP_M;

    return ESP_OK;
}

esp_err_t ultrasonic_measure_cm(const ultrasonic_sensor_t *dev, uint32_t max_distance, uint32_t *distance)
{
    CHECK_ARG(dev && distance);

    uint32_t time_us;
    CHECK(ultrasonic_measure_raw(dev, max_distance * ROUNDTRIP_CM, &time_us));
    *distance = time_us / ROUNDTRIP_CM;

    return ESP_OK;
}

esp_err_t ultrasonic_measure_mm(const ultrasonic_sensor_t *dev, uint32_t max_distance, uint32_t *distance)
{
    CHECK_ARG(dev && distance);

    uint32_t time_us;
    CHECK(ultrasonic_measure_raw(dev, max_distance * ROUNDTRIP_CM, &time_us));
    *distance = time_us / ROUNDTRIP_MM;

    return ESP_OK;
}

#define MAX_DISTANCE_CM 500 // 5m max

//#if defined(CONFIG_IDF_TARGET_ESP8266)
//#define TRIGGER_GPIO 4
//#define ECHO_GPIO 5
//#else
#define TRIGGER_GPIO 18
#define ECHO_GPIO 19
//#endif

void ultrasonic_test(dogFeederData_t *pDogFeederData)
{
    float sumEmptyPercentage;
    int32_t sumDistance;
    uint32_t distance;
    float percentage;

    ultrasonic_sensor_t sensor = {
        .trigger_pin = TRIGGER_GPIO,
        .echo_pin = ECHO_GPIO
    };

    ultrasonic_init(&sensor);

    while (true) {
        if(pDogFeederData->readUltrasonic) {
            sumDistance = 0;
            sumEmptyPercentage = 0;
            for(int i = 0; i < 3; i++) {
                esp_err_t res = ultrasonic_measure_mm(&sensor, MAX_DISTANCE_CM, &distance);
                if (res != ESP_OK) {
                    printf("Error %d: ", res);
                    switch (res) {
                        case ESP_ERR_ULTRASONIC_PING:
                            printf("Cannot ping (device is in invalid state)\n");
                            break;
                        case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                            printf("Ping timeout (no device found)\n");
                            break;
                        case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                            printf("Echo timeout (i.e. distance too big)\n");
                            break;
                        default:
                            printf("%s\n", esp_err_to_name(res));
                    }
                }
                else {
                    vTaskDelay(50 / portTICK_PERIOD_MS);
                    percentage = (((int32_t)distance - 45)*100) / 194;
                    if(percentage > 100) {
                        percentage = 100;
                    } else if(percentage < 0) percentage = 0;
                    sumEmptyPercentage += percentage;
                    sumDistance += distance;
                }
            }
            pDogFeederData->distanceMM = sumDistance / 3;
            pDogFeederData->percentageFull = 100 - (sumEmptyPercentage / 3);
            pDogFeederData->readUltrasonic = 0;
        }
        
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    
    
}

void ultrasonicHandler(dogFeederData_t *dogFeederData)
{
    xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, dogFeederData , 5, NULL);
}