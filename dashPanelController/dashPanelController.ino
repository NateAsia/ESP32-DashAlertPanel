/* --------------------- Standard / ESP Specific Imports ------------------ */ // FYI use (command + click) on a import or variable to goto its definition for more info 
  #include <stdio.h>                // Yummy OS Stuff
  #include <stdlib.h>               // Alot of helpful yummy libs
  #include "freertos/FreeRTOS.h"    // Task Management
  #include "freertos/task.h"        // Task Management
  #include "freertos/queue.h"       // Task Management 
  #include "freertos/semphr.h"      // Task Management
  #include "esp_err.h"              // ESP32 Errors     --  used for debugging
  #include "esp_log.h"              // ESP32 Logging    --  used for debugging
  #include "driver/twai.h"          // API HERE --> [https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/twai.html]

/* --------------------- Definitions and static variables ------------------ */

  #include "constants.h"

  static const twai_filter_config_t   f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL(); // TODO: FILTER
  static const twai_timing_config_t   t_config = TWAI_TIMING_CONFIG_1MBITS();     // CAN SPEED 
  static const twai_general_config_t  g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TX_GPIO_NUM, (gpio_num_t)RX_GPIO_NUM, TWAI_MODE_NO_ACK);

  static SemaphoreHandle_t rx_sem;
  static SemaphoreHandle_t tx_sem;


/* --------------------- DYNAMIC VARIABLES (I/O Status) ------------------ */


  // LEDS
    typedef struct LED_t {
      const uint8_t PIN;
      bool          display_state;
      int16_t       flash_time;         // in ms
      unsigned long last_switch;
      bool          active;
    } LED;

    LED start_btn_led = {START_BTN_PIN, false, NO_FLASH,    0, true};
    LED led2          = {LED_2_PIN,     false, FAST_FLASH,  0, true};
    LED led3          = {LED_3_PIN,     false, FAST_FLASH,  0, true};
    LED led4          = {LED_4_PIN,     false, MED_FLASH,   0, true};
    LED led5          = {LED_5_PIN,     false, SLOW_FLASH,  0, true};

    LED *all_leds[LED_LIST_SIZE] = {&start_btn_led, &led2, &led3, &led4, &led5};

  // CAN
    twai_message_t message;         // CAN Message Struct


/* --------------------------- Tasks and Functions -------------------------- */

void setup_io(){
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup Output & Turn LED on
  for(int i =0; i < LED_LIST_SIZE; i++){
    pinMode(all_leds[i] -> PIN, OUTPUT);
    digitalWrite(all_leds[i] -> PIN, HIGH);
  }

  // Turn off LED (end the light show)
  for(int i =0; i < LED_LIST_SIZE; i++){
    digitalWrite(all_leds[i] -> PIN, LOW);
    delay(500);
  }
}

static void update_outputs_task(void *arg){

  ESP_LOGI(BASE_TAG, "OUTPUT Task Started");

    while(1){

        // Output bool logic
        for(int i =0; i < LED_LIST_SIZE; i++){

          // Check if the LED should just be off
          if(! all_leds[i] -> active){
            all_leds[i] -> display_state = false;
            continue;
          }

          else{
            // Check if this is a flashing LED
            if (all_leds[i] -> flash_time > 0){
              if(millis() - all_leds[i] -> last_switch > all_leds[i] -> flash_time){
                all_leds[i] -> display_state = !all_leds[i] -> display_state;
                all_leds[i] -> last_switch = millis();
              }
            }
            else{
              all_leds[i] -> display_state = true;
            }
          }
        }

        
        // Actually update the output pin
        for(int i =0; i < LED_LIST_SIZE; i++){
          digitalWrite(all_leds[i] -> PIN, all_leds[i] -> display_state);
        }
        
        printf("ds:%d ft:%d ls:%d \n", all_leds[0] -> display_state, all_leds[0] -> flash_time, all_leds[0] -> last_switch);
        
        vTaskDelay(pdMS_TO_TICKS(5));  // 5 ms
    }

    vTaskDelete(NULL);

}



static void twai_receive_task(void *arg){

    xSemaphoreTake(rx_sem, portMAX_DELAY);
    ESP_LOGI(BASE_TAG, "Receive Task Started");

    while(1){

        if (twai_receive(&message, pdMS_TO_TICKS(5)) == ESP_OK); // NOTE: possibly decrease this delay later
        else {
            // printf("\nFailed to receive message\n");
            vTaskDelay(1);
            continue;
        }

        // Print received message
          if (message.extd) {
              printf("\n\nExtended Format\tID: 0x%.08x\t", message.identifier);
          } else {
              printf("\n\nStandard Format\tID: 0x%.03x\t", message.identifier);
          }
          if (!(message.rtr)) {
              printf("Data (%d) (hex): ", message.data_length_code);
              for (int i = 0; i < message.data_length_code; i++) {
                  printf("0x%.02x ", message.data[i]);
              }
          }

        //Process received message
        if(message.identifier == DASH_INPUT_ID){
          start_btn_led.active = message.data[0] & START_BTN_MASK;
        }
        
        vTaskDelay(1); 
    }

    xSemaphoreGive(rx_sem);   // We should never reach this line :)
    vTaskDelete(NULL);

}

void app_main(void){

    // Create Semaphore(s)
    rx_sem = xSemaphoreCreateBinary();  
    // tx_sem = xSemaphoreCreateBinary();  

    // CREATE THREADS (TASKS)
    xTaskCreatePinnedToCore(twai_receive_task,    "TWAI_rx",        4096, NULL, RX_TASK_PRIO,     NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(update_outputs_task,   "TWAI_out",      4096, NULL, OUTPUT_TASK_PRIO,    NULL, tskNO_AFFINITY);

    // Install and start TWAI driver            -   This will force the ESP32 to restart if there is a CAN error - good
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_LOGI(BASE_TAG, "CAN Driver installed");
    ESP_ERROR_CHECK(twai_start());
    ESP_LOGI(BASE_TAG, "CAN Driver started");

    xSemaphoreGive(rx_sem);                     // Allow Start of RX task   
    // xSemaphoreGive(tx_sem);                     // Allow Start of TX task
    vTaskDelay(pdMS_TO_TICKS(100));             
    
    xSemaphoreTake(rx_sem, portMAX_DELAY);      // Wait for RX Task to complete (never ;) )
    // xSemaphoreTake(tx_sem, portMAX_DELAY);      // Wait for TX task to complete (never ;) )
    printf("\nEXITING\n");                      // (we will NEVER reach this line)

    //Stop and uninstall TWAI driver            // Just incase tho
    ESP_ERROR_CHECK(twai_stop());
    ESP_ERROR_CHECK(twai_driver_uninstall());
    ESP_LOGI(BASE_TAG, "CAN Driver Uninstalled");

    //Cleanup
    vSemaphoreDelete(rx_sem);
    vSemaphoreDelete(tx_sem);
}

/* --------------------------- Arduino IDE Compliance --------------------------- */
void setup(){
  
  ESP_LOGI(BASE_TAG, "ESP-STARTING");
  setup_io();

  printf("\n\t\tInitializing CAN\n\n");
  
  app_main();

}

void loop(){
  // do nothing in this loop, the FreeRTOS is managing task scheduling above
}
