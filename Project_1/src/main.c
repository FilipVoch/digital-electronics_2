/***********************************************************************
 * 
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2022 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

#define SERVO_X PB1 
#define SERVO_Y PB2
#define POSITION_2 10
#define POSITION_1 1250

#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>         // C library. Needed for number conversions

volatile uint16_t servo_x = POSITION_1;
volatile uint16_t servo_y = POSITION_2;

int main(void)
{
    // Configure 8-bit Timer/Counter0 to control servo motors
    // Set prescaler to 16 ms and enable overflow interrupt
    TIM0_overflow_16ms();
    TIM0_overflow_interrupt_enable();

    GPIO_mode_output(&DDRB, SERVO_X);        // servo 1
    GPIO_mode_output(&DDRB, SERVO_Y);        // servo 2

    TCCR1A |= (1 << WGM11);                  
    TCCR1B |= (1 << WGM13);

    TCCR1A |= (1 << COM0A1) | (1 << COM0B1); 

    ICR1 = 2500;
                      
    OCR1A = servo_x;
    OCR1B = servo_y;
    
    TCCR1B |= (1 << CS11) | (1 << CS10 ); 

    PCICR |= (1<<PCIE0);                   
    PCMSK0 |= (1<<PCINT0);                 
  

    // Enables interrupts by setting the global interrupt mask
    sei();
    
    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines, ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer0 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 33 ms.
 **********************************************************************/

ISR(TIMER0_OVF_vect)
{
    static uint8_t servo_x_step = 0;        
    static uint8_t servo_y_step = 0;
    static uint8_t increment = 1;   

    if (servo_x == POSITION_1)      
      {
        servo_x_step = 1;           
      }
    else if (servo_x == POSITION_2) 
      {
        servo_x_step = 0;          
      }
       
    if (servo_x_step == 0)
      {
        servo_x += increment;              
      }
        
    if (servo_x_step == 1)
      {
        servo_x -= increment;               
      }
    OCR1A = servo_x;                
       
      
    if (servo_y == POSITION_1)      
      {
        servo_y_step = 1;              
      }
    else if (servo_y == POSITION_2) 
      {
        servo_y_step = 0;               
      }
       
    if (servo_y_step == 0)
      {
        servo_y += increment + 1;               
      }

    if (servo_y_step == 1)
      {
        servo_y -= increment + 1;                
      }
    OCR1B = servo_y;                  
}
