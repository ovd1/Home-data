#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "lcd.h"

void lcd_init_4d(void)
{
    _delay_ms(100);                            

    lcd_RS_port &= ~(1<<lcd_RS_bit);               
    lcd_E_port &= ~(1<<lcd_E_bit);              

    lcd_write_4(lcd_FunctionReset);                 
    _delay_ms(10);                                 

    lcd_write_4(lcd_FunctionReset);                
    _delay_us(200);                                

    lcd_write_4(lcd_FunctionReset);                
    _delay_us(200);                               

    lcd_write_4(lcd_FunctionSet4bit);              
    _delay_us(80);                                  

    lcd_write_instruction_4d(lcd_FunctionSet4bit);   
    _delay_us(80);                                  

    lcd_write_instruction_4d(lcd_DisplayOff);        
    _delay_us(80);                                  

    lcd_write_instruction_4d(lcd_Clear);             
    _delay_ms(4);                                   

    lcd_write_instruction_4d(lcd_EntryMode);        
    _delay_us(80);                                  
 
    lcd_write_instruction_4d(lcd_DisplayOn);         
    _delay_us(80);                                  
}


void lcd_write_string_4d(uint8_t theString[])
{
    volatile int i = 0;                             
    while (theString[i] != 0)
    {
        lcd_write_character_4d(theString[i]);
        i++;
        _delay_us(80);                              
    }
}


void lcd_write_character_4d(uint8_t theData)
{
    lcd_RS_port |= (1<<lcd_RS_bit);                 
    lcd_E_port &= ~(1<<lcd_E_bit);                  
    lcd_write_4(theData);                           
    lcd_write_4(theData << 4);                      
}


void lcd_write_instruction_4d(uint8_t theInstruction)
{
    lcd_RS_port &= ~(1<<lcd_RS_bit);               
    lcd_E_port &= ~(1<<lcd_E_bit);                  
    lcd_write_4(theInstruction);                    
    lcd_write_4(theInstruction << 4);               
}


void lcd_write_4(uint8_t theByte)
{
    lcd_D7_port &= ~(1<<lcd_D7_bit);                        
    if (theByte & 1<<7) lcd_D7_port |= (1<<lcd_D7_bit);    

    lcd_D6_port &= ~(1<<lcd_D6_bit);                        
    if (theByte & 1<<6) lcd_D6_port |= (1<<lcd_D6_bit);

    lcd_D5_port &= ~(1<<lcd_D5_bit);
    if (theByte & 1<<5) lcd_D5_port |= (1<<lcd_D5_bit);

    lcd_D4_port &= ~(1<<lcd_D4_bit);
    if (theByte & 1<<4) lcd_D4_port |= (1<<lcd_D4_bit);

    lcd_E_port |= (1<<lcd_E_bit);                   
    _delay_us(1);                                   
    lcd_E_port &= ~(1<<lcd_E_bit);                  
    _delay_us(1);                                   
}

