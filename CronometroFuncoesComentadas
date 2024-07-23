#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <esp_mac.h>

#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#define btn_Pin GPIO_NUM_3

BaseType_t th_cronom_1, timerInit;
TaskHandle_t th_cronom_handle_1;
TimerHandle_t timer_handle_1;

int s = 0, min = 0, click = 0;
double ms = 0.0;

void btnClickedInterrupt(void);
void btnIntrInvert(void);
void threadCronom(void);
void timerCronomPos(TimerHandle_t xTimer);
void timerCronomNeg(TimerHandle_t xTimer);

/*void btnClickedInterrupt(void)
{
  if (xTimerIsTimerActive(timer_handle_1) != pdFALSE)
  {
    xTimerStop(timer_handle_1, pdMS_TO_TICKS(100));
  }
  else
  {
    xTimerReset(timer_handle_1, pdMS_TO_TICKS(100));
  }
}*/

/*void btnIntrInvert(void)
{
  click ^= 1;
  if (click == 1){

  }
}*/

void btnClickedInterrupt(void)
{
  if (xTimerIsTimerActive(timer_handle_1) != pdFALSE)
  {
    xTimerStop(timer_handle_1, pdMS_TO_TICKS(100));
  }
  else
  {
    xTimerReset(timer_handle_1, pdMS_TO_TICKS(100));
  }
}

/*void threadCronom(void)
{
  int n = 0;

  while (1)
  {
    printf("Thread: %d s \n", ++n);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}*/

void timerCronomPos(TimerHandle_t xTimer)
{
  ms = (double)s / 1000;
  printf("Timer: %d:%d:%f \n", min, ++s, ms);
  if (s == 59)
  {
    s = 0;
    ++min;
  }
}

void timerCronomNeg(TimerHandle_t xTimer)
{
  ms = (double)s / 1000;
  printf("Timer: %d:%d:%f \n", min, --s, ms);
  if (s == 0)
  {
    s = 59;
    --min;
    if (min <= 1)
    {
      min = 0;
    }
  }
}

int main(void)
{
  return 0;
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
  // gpio_isr_handler_add(btn_Pin, (gpio_isr_t)btnIntrInvert, NULL);

  /*th_cronom_1 = xTaskCreate((TaskFunction_t)threadCronom,
                            "thread 1",
                            2 * 1024,
                            NULL,
                            5,
                            &th_cronom_handle_1);

  if (th_cronom_1 != pdPASS)
  {
    printf("Erro ao criar thread. \n");
  }

  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }*/

  timer_handle_1 = xTimerCreate("timer 1",
                                pdMS_TO_TICKS(1000),
                                pdTRUE,
                                NULL,
                                timerCronomPos);

  if (timer_handle_1 == NULL)
  {
    printf("Erro ao criar timer");
    return;
  }

  timerInit = xTimerStart(timer_handle_1, 100);

  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
