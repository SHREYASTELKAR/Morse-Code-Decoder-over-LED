#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_system.h"

#define PHOTO_GPIO 0
#define ADC_CHANNEL ADC1_CHANNEL_0

/*
gpio_config_t photo = {
    .pin_bit_mask = 1ULL << PHOTO_GPIO,
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .intr_type = GPIO_INTR_DISABLE
};
*/

#define HIGH 1300
#define LOW 700

char buf[1000] = "";
char buffer[500] = "";

const char* morseAlphabet[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", "...--",
    "....-", ".....", "-....", "--...", "---..", "----.", "-----", "--..--",
    ".-.-.-", "..--..", "-..-.", "-....-", "-.--.", "-.--.-",
    ".-..-.", ".--.-.", "-...-", ".-.-.", "-.-.-.", "-....-", "...-..-",
    "-.-.-.", "..--.-", "-...-", ".-.-.", "-.-.-.", "-....-", "...-..-",
    "-.-.-.", "..--.-", "-...-"
};

const char* englishAlphabet[] = {
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N",
    "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "1", "2",
    "3", "4", "5", "6", "7", "8", "9", "0", ",", ".", "?", "!", "/", "@",
    "(", ")", "&", ":", ";", "=", "+", "-", "_", "\"", "$", "@", " "
};

static void do_morse(void *pvParameters)
{

    int on = 0;
    int off = 0;
    bool started = false;
    char slash[] = "/";
    char comma[] = ",";
    char dot[] = ".";
    char dash[] = "-";


    while (1) {
       int val = adc1_get_raw(ADC_CHANNEL);
       
       //printf("%d\n", val);
     
       //check for start with bool
       //print each char
     // was 50 12 2 8 22
     
       if (val > HIGH) {
           //space bet chars
           if ((off > 15 && off < 26) && (started == true)) {
               strcat(buf, slash);   //space bet words   
           }
           else if ((off > 6 && off < 15) && (started == true)) {
               strcat(buf, comma); //bet chars
           }	
           on+=1;
           off = 0;
           started = true;
       }
     
       else if (val < LOW)  {
           
           if ((off > 26) && (started == true)) {
               break;
           }

           //was 2 and 6

           if (on >= 1 && on <= 2) {
               strcat(buf, dot);
           }
           else if (on >= 3 && on <= 5) {
               strcat(buf, dash);
           }
           off += 1;
           on = 0;	
       }

       vTaskDelay(pdMS_TO_TICKS(12));
    }

    printf("%s\n", buf);
    //Convert to English
  
    char* token1;
    char* token2;
    char* rest = buf;
    bool first = false;

    while ((token1 = strtok_r(rest, "/", &rest)) != NULL) {
	  if (first == true) {
	      strcat(buffer, " ");
	  }
	  first = true;

          while ((token2 = strtok_r(token1, ",", &token1)) != NULL) {
               int found = 0;
               for (int i = 0; i < 57; i++) {  // Update the limit to match the size of the arrays
                   if (strcmp(token2, morseAlphabet[i]) == 0) {
                       strcat(buffer, englishAlphabet[i]);
                       found = 1;
                       break;
                   }
               }

               if (!found) {
                   printf("?");
               }
           }
     }
  
      
     printf("%s\n", buffer);

     vTaskDelete(NULL);
}

void app_main(void)
{   
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11);
   
    xTaskCreate(&do_morse, "do_morse", 4096, NULL, 5, NULL);
    
}
