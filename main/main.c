#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define LED_COUNT 5
const gpio_num_t led_pins[LED_COUNT] = {GPIO_NUM_32, GPIO_NUM_33, GPIO_NUM_25, GPIO_NUM_26, GPIO_NUM_27};
#define ADC_CHANNEL ADC1_CHANNEL_6  // GPIO34
#define DEFAULT_VREF    1100        // For ADC calibration (1.1V)

void init_gpio() {
    for (int i = 0; i < LED_COUNT; i++) {
        gpio_reset_pin(led_pins[i]);
        gpio_set_direction(led_pins[i], GPIO_MODE_OUTPUT);
    }
}

void app_main(void)
{
    init_gpio();

    adc1_config_width(ADC_WIDTH_BIT_12); // Set ADC resolution to 12 bits (0–4095)
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11); // Configure attenuation for up to ~3.3V input

    while (1) {
        int adc_reading = adc1_get_raw(ADC_CHANNEL);

        // Map the ADC value to the number of LEDs to light up (0 to 5 levels)
        int level = (adc_reading * (LED_COUNT + 1)) / 4096;

        printf("ADC: %d -> Level: %d\n", adc_reading, level);

        for (int i = 0; i < LED_COUNT; i++) {
            gpio_set_level(led_pins[i], (i < level) ? 1 : 0);
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
