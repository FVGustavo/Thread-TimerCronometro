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

BaseType_t th_cronom_1, timerInit1;
TaskHandle_t th_cronom_handle_1;
TimerHandle_t timer_handle_1;

uint32_t s = 0, min = 0, ms = 0;
u_int16_t click = 0;

void btnClickedInterrupt(void);
void timerCronomMs(TimerHandle_t xTimer);

void btnClickedInterrupt(void)
{
  click ^= 1;
}

void timerCronomMs(TimerHandle_t xTimer)
{
  if (ms == 0 && s == 0 && min == 0 && click == 1)
  {
    return;
  }

  if (click == 0)
  {
    ms++;
    if (ms >= 100)
    {
      ms = 0;
      s++;
      if (s >= 60)
      {
        s = 0;
        min++;
      }
    }
  }
  else
  {
    if (ms == 0)
    {
      if (s == 0)
      {
        if (min > 0)
        {
          min--;
          s = 59;
          ms = 99;
        }
      }
      else
      {
        s--;
        ms = 99;
      }
    }
    else
    {
      ms--;
    }
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

  timerInit1 = xTimerStart(timer_handle_1, 100);

  while (1)
  {
    printf("%lu:%lu:%lu \n", min, s, ms);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
