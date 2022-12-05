/***********************************************************************
 * 
 * Use Analog-to-digital conversion to read push buttons on LCD keypad
 * shield and display it on LCD screen.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2018 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/
#define ENCODER_SW PD3     // Encoder button
#define JOYSTICK_SW PD2    // Joystick button

#define ENCODER_A PB4      // Encoder data
#define ENCODER_B PB5      // Encoder clock

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions

volatile uint16_t CursorX;
volatile uint16_t CursorY;

#define PD3 3          // In Arduino world, PB5 is called "13"
#define PD2 2
#define PB4 12
#define PB5 13

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/


int main(void)
{
    // Initialize display
    
    lcd_init(LCD_DISP_ON);
   
    //Configure digital inputs
    
    GPIO_mode_input_pullup(&DDRD, JOYSTICK_SW);
    GPIO_mode_input_pullup(&DDRD, ENCODER_SW);

    GPIO_mode_input_nopull(&DDRB, ENCODER_A);
    GPIO_mode_input_nopull(&DDRB, ENCODER_B);


    // Configure Analog-to-Digital Conversion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);

    
   
    // Enable ADC module
    ADCSRA |= (1 << ADEN);
    // Enable conversion complete interrupt
    ADCSRA |= (1 << ADIE);
    // Set clock prescaler to 128
    ADCSRA |= ((1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2));

    

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt
    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
   // ADC - reading input from two channels
   static uint8_t channel = 0;
   if (channel == 0)
   {
    ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3));
    channel = 1;
   }
   else
   {
    ADMUX &= ~((1 << MUX1) | (1 << MUX2) | (1 << MUX3));
    ADMUX |= (1 << MUX0);
    channel = 0;
   }
    
   // Start ADC conversion
    ADCSRA |= (1 << ADSC);



    // Switch definitions

    static uint8_t encodeButtVal = 0;
    encodeButtVal = GPIO_read(&PIND, ENCODER_SW);

    static uint8_t buttonVal = 0;
    buttonVal = GPIO_read(&PIND, JOYSTICK_SW);
    
    //Switch test

    /*if (buttonVal == 0)
    {
       lcd_gotoxy(5, 0); 
       lcd_puts("z:");
    }
    else
    {
      lcd_gotoxy(5, 0); 
       lcd_puts("x:");
    }
    
    if (encodeButtVal == 0)
    {
       lcd_gotoxy(5, 1); 
       lcd_puts("z:");
    }
    else
    {
      lcd_gotoxy(5, 1); 
       lcd_puts("y:");
    }
   */

    
    
    // timer app
    
    static uint8_t no_of_overflows = 0;
    static uint8_t tenths = 0;  // Tenths of a second
    static uint8_t seconds = 0;  // Seconds
    static uint8_t minutes = 0;  // Minutes
    static uint8_t on = 1; // 1 - Off, 0 - On
    
    char string[2];             // String for converted numbers by itoa()



    
    
   // Timer settings 
    
   if (buttonVal == 0)
    {
       if (CursorX = 0 && CursorY = 0 )
      {
        minutes = minutes + 10;
       
      }
       
       else if (CursorX = 1 && CursorY = 0 )
      {
        minutes ++;
       
      } 
       else if (CursorX = 3 && CursorY = 0 )
      {
        seconds = seconds + 10;
       
      } 
       else if (CursorX = 4 && CursorY = 0 )
      {
        seconds ++;
       
      } 
    }

    
    // Timer start, timer user interface
    
    
    if (encodeButtVal == 0) 
    {
       on = 0;    
    }    
    
    
   
    if (on == 0) 
  {
       
    
    no_of_overflows++;
    if (no_of_overflows >= 3)
    {
        // Do this every 3 x 33 ms = 100 ms
        no_of_overflows = 0;
        tenths--;
        // Count tenth of seconds 9, 8, ..., 0, 9, 8, ...
        if (tenths = 0)
        {
          tenths = 9;
          seconds--;

          if (seconds = 0)
          {
            seconds = 59;
            minutes--;
            if (minutes = 0)
            {
              tenths = 0;
              seconds = 0;
              minutes = 0;
            }
          }
        }
        
        itoa(tenths, string, 10);  // Convert decimal value to string
        // Display "00:00.tenths"
        lcd_gotoxy(6, 0);
        lcd_puts(string);

        lcd_gotoxy(5,0);
        lcd_putc('.');

        if (seconds < 10)
        {
          itoa(seconds, string, 10);
          lcd_gotoxy(4,0);
          lcd_puts(string);
          lcd_gotoxy(3,0);
          lcd_putc('0');
        }
        else{
          itoa(seconds, string, 10);
          lcd_gotoxy(3,0);
          lcd_puts(string);
        }
        
        lcd_gotoxy(2,0);
        lcd_putc(':');

        if (minutes < 10)
        {
          itoa(minutes, string, 10);
          lcd_gotoxy(1,0);
          lcd_puts(string);
          lcd_gotoxy(0,0);
          lcd_putc('0');
        }
        else{
          itoa(minutes, string, 10);
          lcd_gotoxy(0,0);
          lcd_puts(string);
        }
        
        lcd_gotoxy(15,1);

    }
   }
    
    
    if ((minutes == 0 && seconds == 0 && tenths == 0)|| (encodeButtValue == 0 && buttonVal == 0)) 
    {
       on = 1;    
    }
    
}


/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Move with cursor on LCD screen.
 **********************************************************************/
ISR(ADC_vect)
{
    static uint8_t channel = 0;
    uint16_t value;
   
    char string[4];  // String for converted numbers by itoa()

    
   // ADC Test
    
    
   /* if (channel == 0)
    {
    value = ADC;

    itoa(value, string, 10);
    lcd_gotoxy(8,0);
    lcd_puts(string);

    channel = 1;
    }
    
    else if (channel == 1)
    
    {
    value = ADC;

    itoa(value, string, 10);
    lcd_gotoxy(8,1);
    lcd_puts(string);

    channel = 0;
    }
         // Cursors 
     int CursorX = 0;
     int CursorY = 0;*/
    
    
//Cursors
   
    if (channel == 0) //osa x
    {
     value = ADC;
     if (value > 600)
     {
     CursorX++;
     if (CursorX == 2)
     {
       CursorX = 3;
     }
     else if (CursorX == 5)
     {
       CursorX = 6;
     }
     else if (CursorX > 6)
     {
       CursorX = 0;
     }
         
     lcd_gotoxy(CursorX, CursorY);
     }
    else if (value < 400)
    {
     CursorX--;
     if (CursorX == 5)
     {
       CursorX = 4;
     }
     else if (CursorX == 2)
     {
       CursorX = 1;
     }
     else if (CursorX < 0)
     {
       CursorX = 6;
     }
     lcd_gotoxy(CursorX, CursorY);
    }
    else
    {
     lcd_gotoxy(CursorX, CursorY);
    }
     channel = 1;
   }
   else if (channel == 1) //osa y
   {
    value = ADC;
     if (value > 600)
     {
      CursorY != CursorY;
      lcd_gotoxy(CursorX, CursorY);
      }
     else if (value < 400)
     {
     CursorY != CursorY;
     lcd_gotoxy(CursorX, CursorY);
     }
     else
     {
     lcd_gotoxy(CursorX, CursorY);
     }
     channel = 0;
   }
  
    
}
