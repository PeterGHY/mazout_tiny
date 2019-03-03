/*
 * clear;make tiny;ll main.*
 */

#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include <util/delay.h>

#include "main.h"
#include "lcd.h"
//#include "LCD-AVR-4d.h"
#include "srf04.h"
#include "adc.h"

uint8_t flipIt = 1;
char buffer[4];

void flipLed(){
    if (flipIt == 1){
        flipIt = 0;
        LED_HIGH();
    }else{
        flipIt = 1;
        LED_LOW();
    }
}

void formatStr(int a){
    if (a<10){
        lcd_write_string("   ");
        lcd_write_string(buffer);
        return;
    }
    if (a<100){
        lcd_write_string("  ");
        lcd_write_string(buffer);
        return;
    }
    if (a<1000){
        lcd_write_string(" ");
        lcd_write_string(buffer);
        return;
    }
    if (a<5000){
        lcd_write_string(buffer);
        return;
    }
}

/******************************* Main Program Code *************************/
int main(void)
{
    // srf04
    running = 0;
    up = 0;
    
    float df;
    int vol;
    
    uint16_t adc_result;
     
    // LED
    LED_DDRB_OUTPUT_MODE();
    
    // initialize the LCD display for a 4-bit interface
    lcd_init();
    
    // initialize ultrasonic
    srf04_init();
    
    // initialize adc
    adc_init();
    
    while(1){
        // display the first line of information
        lcd_write_instruction(lcd_SetCursor | lcd_LineOne);

        if (running == 0) { // launch only when next iteration can happen
            _delay_ms(60);
            sonar(); // launch ultrasound measurement!
            flipLed();
        }
//         distance = rand() % 5000;
//         flipLed();
        
         df=(float)134-distance;
         vol = (265*((pow(60,2)*acos((60-df)/60))-(60-df)*sqrt((2*60*df)-pow(df,2))))/1000;
         itoa ((int)vol,buffer,10);
         formatStr((int)vol);lcd_write_string(" lit");
         lcd_write_instruction(lcd_SetCursor | lcd_LineTwo);
         //itoa ((int)distance,buffer,10);
         //formatStr((int)distance);lcd_write_string(" mm");
         //formatStr((int)timerCounter);lcd_write_string(" cnt");
         adc_result = adc_read(0);          // PA0
         itoa ((int)adc_result,buffer,10);
         formatStr((int)adc_result);lcd_write_string(" bar");
         
         _delay_ms(1000);                 
    }
    return 0;
}
