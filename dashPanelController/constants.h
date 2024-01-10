/* 
  ******************** CONSTANTS FILE ********************

  - Please use this for all variables that remain constant :)
  - Please label definitions with useful information on the right
  - Add new sections as needed

 */



/* --------------------- ESP-IDF Setup ------------------ */
  // LOG TAGS
    #define BASE_TAG    "DASH CONTROLLER"  



/* --------------------- FreeRTOS Setup ------------------ */
  #define TX_TASK_PRIO      9         // TX Task Priority 
  #define RX_TASK_PRIO      8         // RX Task Priority 
  #define INPUT_TASK_PRIO   7         // INPUT Task Priority
  #define OUTPUT_TASK_PRIO  6         // INPUT Task Priority


/* --------------------- TWAI / CAN Transceiver Setup ------------------ */
  #define TX_GPIO_NUM     8          // "TX2" on the ESP32-WROOM-32
  #define RX_GPIO_NUM     9          // "RX2" on the ESP32-WROOM-32

  #define DASH_INPUT_ID    0x000          // From the C185 or steering wheel
  // #define DRS_OUTPUT_ID   0x000          // To ECU (or back to the C185 first)


/* --------------------- DASH-C IO ------------------ */

    #define LED_BUILTIN   1
    
    #define LED_1_PIN     D0
    #define LED_2_PIN     D1
    #define LED_3_PIN     D2
    #define LED_4_PIN     D3
    #define LED_5_PIN     D5

    #define WALKY_TALK    D4
    
    
    #define START_BTN_PIN LED_1_PIN

/* --------------------- Logic Code ------------------ */

    #define LED_LIST_SIZE     5

    #define START_BTN_MASK    0b00000001


    // Flash Speeds
      #define NO_FLASH      0       // ms
      #define FAST_FLASH    250     // ms
      #define MED_FLASH     500     // ms
      #define SLOW_FLASH    1000    // ms
    




    