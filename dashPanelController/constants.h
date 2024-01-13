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
  #define TX_GPIO_NUM     D10          
  #define RX_GPIO_NUM     D9          

  #define DASH_INPUT_ID    0x660          // From the C185 or steering wheel
  // #define DRS_OUTPUT_ID 0x000          // To ECU (or back to the C185 first)


/* --------------------- DASH-C IO ------------------ */

    #define LED_BUILTIN   1
    
    #define LED_1_PIN     D0
    #define LED_2_PIN     D1
    #define LED_3_PIN     D2
    #define LED_4_PIN     D3
    #define LED_5_PIN     D5

    #define WALKY_TALK    D4
    
    
    #define START_BTN_PIN     LED_1_PIN
    #define DRS_LED_PIN       LED_2_PIN
    #define BSPD_LED_PIN      LED_3_PIN
    #define BOTS_LED_PIN      LED_4_PIN
    #define SELECTION_LED_PIN LED_5_PIN

/* --------------------- Logic Code ------------------ */

    #define LED_LIST_SIZE     5
    #define LED_FADE_SPEED    3

    // LOGIC
      #define ENGINE_ON_SPEED   700   // RPMs

    // BIT BASKS
      #define DRS_MASK          (1 << 7)
      #define BSPD_MASK         (1 << 6)
      #define BOTS_MASK         (1 << 5)
      #define SELECTION_MASK    (1 << 4)
      // #define START_BTN_MASK    0b00000001


    // Flash Speeds
      #define NO_FLASH      0       // ms
      #define FAST_FLASH    250     // ms
      #define MED_FLASH     500     // ms
      #define SLOW_FLASH    1000    // ms
    




    