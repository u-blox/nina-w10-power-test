/* NINA-W10 Power Test
 * Copyright (c) 2006-2018 u-blox Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This program is intended to test the "hibernate" behaviour of the NINA-W10
 * module.  The SW API reference doesn't refer to hibernate but, from this
 * forum question: https://www.esp32.com/viewtopic.php?f=2&t=3083, it seems
 * that configuring for deep sleep in a certain way should result in
 * hibernate. See also this example:
 * https://github.com/espressif/esp-idf/blob/cc5673435be92c4beceb7108c738d6741ea7230f/examples/system/deep_sleep/main/deep_sleep_example_main.c
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "sys/time.h"
#include "rtc_wake_stub_data.h"

#define RTC_REGISTER_ADDRESS_BANK_1 ((int *) 0x3FF4804C)
#define RTC_REGISTER_ADDRESS_BANK_2 ((int *) 0x3FF480B0)
#define SLEEP_TIME_USECONDS (1 * 1000000)

/** Read from one of 8 32-bit RTC registers.
 *
 * @param offset offset of register.
 * @return       the byte from that register.
 */
static inline int rtc_data_read(size_t offset)
{
    int *reg = RTC_REGISTER_ADDRESS_BANK_1;

    if (offset >= 4) {
        reg = RTC_REGISTER_ADDRESS_BANK_2;
        offset -= 4;
    }
 
    printf("Reading from address 0x%08x...\n",
           (int) (reg + offset));
    return *(reg + offset);
}

/** Write to one of 8 32-bit RTC registers.
 *
 * @param data   the byte to write.
 * @param offset offset of register.
 */
static inline void rtc_data_write(int data, size_t offset)
{
    int *reg = RTC_REGISTER_ADDRESS_BANK_1;

    if (offset >= 4) {
        reg = RTC_REGISTER_ADDRESS_BANK_2;
        offset -= 4;
    }

    printf("Writing %d (0x%08x) to address 0x%08x...\n", data, data,
           (int) (reg + offset));
    *(reg + offset) = data;
}

void app_main()
{
    int wakeup_cause = esp_sleep_get_wakeup_cause();
    struct timeval now;
    struct timeval sleep_enter_time;
    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;
    int stored_variable;

    switch (wakeup_cause) {
        case ESP_SLEEP_WAKEUP_TIMER:
            printf("Wake up from timer. Time spent in deep sleep: %dms.\n", sleep_time_ms);
        break;
        default:
            printf("Wake up cause %d.\n", wakeup_cause);
        break;
    }

    for (size_t x = 0; x < 8; x++) {
        stored_variable = rtc_data_read(x);
        printf("Stored variable is %d.\n", stored_variable);
    }
    for (size_t x = 0; x < 1; x++) {
        stored_variable = rtc_data_read(x);
        printf("Incrementing stored variable...\n");
        stored_variable++;
        rtc_data_write(stored_variable, x);
    }

    printf("Entering hibernate...\n");
    gettimeofday(&sleep_enter_time, NULL);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // This should put the processor into hibernate,
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    esp_sleep_enable_timer_wakeup(SLEEP_TIME_USECONDS);
    esp_deep_sleep_start();
}
