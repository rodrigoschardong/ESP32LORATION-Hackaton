#include <stdio.h>
#include <stdlib.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_err.h"

#include "esp_adc_cal.h"
#include "driver/adc.h"
#include "adc.h"

static esp_adc_cal_characteristics_t adc1_chars;

void initADC() {
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
}

/**
 * read adc input in mV
*/
int readADC(int samples) {
    uint32_t voltage = 0;
    for (uint8_t i = 0; i < samples; i++)
    {
        voltage += esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_6), &adc1_chars);
    }
    
    return voltage/samples;
}

/**
 * read adc input in kg
*/
double readWeight(int samples) {
    uint32_t voltage = 0;
    double weight = 0;
    for (int i = 0; i < samples; i++)
    {
        voltage += esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_6), &adc1_chars);
    }
    
    weight = 8.56*(voltage/samples)/1000.00-13.80;;
    return weight;    
}