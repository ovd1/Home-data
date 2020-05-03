#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "lcd.c"
#include "lcd.h"
#include "dht11.c"
#include "dht11.h"

#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


static char buf_t[8];
static char buf_h[8];

void USART0Init(void) {
    UBRR0H = (uint8_t)(UBRR_VALUE>>8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}

void USART0SendByte(uint8_t u8Data) {
    while(!(UCSR0A&(1<<UDRE0))){};
    UDR0 = u8Data;
}
uint8_t USART0ReceiveByte() {
    while(!(UCSR0A&(1<<RXC0))){};
    return UDR0;
}


int main(void)
{
    uint8_t temperature = 0;
    uint8_t humidity = 0;
    
    uint8_t u8TempData;
    
    USART0Init();
    
    lcd_D7_ddr |= (1<<lcd_D7_bit);                  
    lcd_D6_ddr |= (1<<lcd_D6_bit);
    lcd_D5_ddr |= (1<<lcd_D5_bit);
    lcd_D4_ddr |= (1<<lcd_D4_bit);

    lcd_E_ddr |= (1<<lcd_E_bit);                    
    lcd_RS_ddr |= (1<<lcd_RS_bit);                  

    lcd_init_4d();                         

   
    while(1){
        int i = 0;
        while (USART0ReceiveByte() != 'm');
        
        humidity = dht11_gethumidity();
        _delay_ms(1000); 
        temperature = dht11_gettemperature();
        
        sprintf(buf_t, "T=%02d", temperature);
        lcd_write_string_4d(buf_t);
        
        lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
                        
        sprintf(buf_h, "H=%02d", humidity);
        lcd_write_string_4d(buf_h);
        lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
           
        _delay_ms(1000);
        
                
        while(buf_h[i] != 0) 
        {
            while (!( UCSR0A & (1<<UDRE0))); 
            UDR0 = buf_h[i];
            i++;                             
        }
        i = 0;
        while(buf_t[i] != 0) 
        {
            while (!( UCSR0A & (1<<UDRE0))); 
            UDR0 = buf_t[i];
            i++;                            
        }
    }
    
    return 0;
}
