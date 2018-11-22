/* NINA-W10 Power Test
 */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

void periodic_task(void *pvParameter)
{
    while (1) {
        printf("Hello world, now waiting 5 seconds...\n");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    xTaskCreate(&periodic_task, "periodic_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}
