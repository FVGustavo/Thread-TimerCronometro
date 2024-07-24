#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <esp_mac.h>

#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#define btn_Pin GPIO_NUM_3

BaseType_t th_cronom_1, timer_init_1, timer_change_p_1;
// TaskHandle_t th_cronom_handle_1;
TimerHandle_t timer_handle_1;

u_int16_t click, quant = 0;
uint32_t cnt = 0, s = 0, min = 0, ms = 0;

void btnClickedInterrupt(void);
void timerCronomMs(TimerHandle_t xTimer);

void btnClickedInterrupt(void)
{
  click = 0;
  click ^= (1 << quant);
  quant++;
  if (quant >= 3)
  {
    quant = 0;
  }
}

void timerCronomMs(TimerHandle_t xTimer)
{
  if (click == 1)
  {
    s = 0;
    min = 0;
    ms++;
  }
  if (click == 2)
  {
    timer_change_p_1 = xTimerChangePeriod(timer_handle_1,
                                          pdMS_TO_TICKS(1000),
                                          pdMS_TO_TICKS(100));
    ms = 0;
    min = 0;
    s++;
  }
  if (click == 4)
  {
    timer_change_p_1 = xTimerChangePeriod(timer_handle_1,
                                          pdMS_TO_TICKS(60000),
                                          pdMS_TO_TICKS(100));
    ms = 0;
    s = 0;
    min++;
  }
}

void app_main(void)
{
  gpio_config_t btn_conf;
  btn_conf.intr_type = GPIO_INTR_NEGEDGE;        // Habilita interrupção na queda
  btn_conf.mode = GPIO_MODE_INPUT;               // Configura o pino como entrada
  btn_conf.pin_bit_mask = (1ULL << btn_Pin);     // Seleciona o pino GPIO
  btn_conf.pull_up_en = GPIO_PULLUP_ENABLE;      // Habilita pull up
  btn_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // Desabilita pull down
  gpio_config(&btn_conf);

  gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
  gpio_isr_handler_add(btn_Pin, (gpio_isr_t)btnClickedInterrupt, NULL);

  timer_handle_1 = xTimerCreate("timer 1",
                                pdMS_TO_TICKS(10),
                                pdTRUE,
                                NULL,
                                timerCronomMs);

  if (timer_handle_1 == NULL)
  {
    printf("Erro ao criar timer 1");
    return;
  }

  timer_init_1 = xTimerStart(timer_handle_1, 100);

  while (1)
  {
    if (click == 1)
    {
      timer_change_p_1 = xTimerChangePeriod(timer_handle_1,
                                            pdMS_TO_TICKS(10),
                                            pdMS_TO_TICKS(100));
      s = 0;
      min = 0;
      ms++;
    }
    printf("Button: %hu - Cronometro: %lu:%lu:%lu \n", click, min, s, ms);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
