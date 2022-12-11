#include "buzzerBitter.h"

#define buzzer 5
#define GPIO_OUTPUT_SPEED LEDC_HIGH_SPEED_MODE

ledc_timer_config_t timer_conf;
ledc_channel_config_t ledc_conf;

/**
 * @brief function used to configure the buzzer parameter
 * 
 */
void configBuzzerGeneral (void)
{
	timer_conf.speed_mode = GPIO_OUTPUT_SPEED;
	timer_conf.bit_num    = LEDC_TIMER_10_BIT;
	timer_conf.timer_num  = LEDC_TIMER_1;
	timer_conf.freq_hz 	  = 1;
	ledc_timer_config(&timer_conf);

	ledc_conf.gpio_num   = buzzer;
	ledc_conf.speed_mode = GPIO_OUTPUT_SPEED;
	ledc_conf.channel    = LEDC_CHANNEL_0;
	ledc_conf.intr_type  = LEDC_INTR_DISABLE;
	ledc_conf.timer_sel  = LEDC_TIMER_1;
	ledc_conf.duty       = 0x01FF;
}

/**
 * @brief Changes the frequence parameter
 * 
 * @param t_NewFreq new value to frequence
 */
void changeFreq (uint32_t t_NewFreq)
{
	timer_conf.freq_hz = t_NewFreq;
	ledc_timer_config(&timer_conf);
}

/**
 * @brief Executes a sound based on their frequence and duration
 * 
 * @param t_Freq Signal frequence
 * @param t_Duration Signal duration
 */
void soundOn (uint32_t t_Freq, uint32_t t_Duration)
{	                            
	changeFreq(t_Freq);

	ledc_channel_config(&ledc_conf);

	// start
    ledc_set_duty(GPIO_OUTPUT_SPEED, LEDC_CHANNEL_0, 0x7F); 
    ledc_update_duty(GPIO_OUTPUT_SPEED, LEDC_CHANNEL_0);
	vTaskDelay(t_Duration/portTICK_PERIOD_MS);

	// stop
    ledc_set_duty(GPIO_OUTPUT_SPEED, LEDC_CHANNEL_0, 0);
    ledc_update_duty(GPIO_OUTPUT_SPEED, LEDC_CHANNEL_0);
}

/**
 * @brief Function used to play a determinated song
 * 
 */
void playBeep (bool *t_flag)
{
	configBuzzerGeneral();

	while(1)
	{
		if(*t_flag)
		{
			printf("Playing sound...\n");

			soundOn(3000, 100);
			soundOn(2560, 50);
			soundOn(800, 500);
			vTaskDelay(pdMS_TO_TICKS(100));
			soundOn(2500, 500);

			*t_flag = pdFALSE;
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

/**
 * @brief Function wich create a parallel task to the sound effect
 * 
 */
void soundHandler (bool *t_play)
{
	xTaskCreate(playBeep, "playBeep", configMINIMAL_STACK_SIZE * 3, t_play, 6, NULL);
}
