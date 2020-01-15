#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "lcd.c"
#include "lcd.h"
#include "dht11.c"
#include "dht11.h"

char buffer[8];


int main(void)
{
    uint8_t temperature = 0;
    uint8_t humidity = 0;
    lcd_D7_ddr |= (1<<lcd_D7_bit);                  
    lcd_D6_ddr |= (1<<lcd_D6_bit);
    lcd_D5_ddr |= (1<<lcd_D5_bit);
    lcd_D4_ddr |= (1<<lcd_D4_bit);

    lcd_E_ddr |= (1<<lcd_E_bit);                    
    lcd_RS_ddr |= (1<<lcd_RS_bit);                  

    lcd_init_4d();                         

   
    while(1){
        humidity = dht11_gethumidity();
        _delay_ms(1000); 
        temperature = dht11_gettemperature();
        
        sprintf(buffer, "Tem=%02d", temperature);
        lcd_write_string_4d(buffer);

        lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
                        
        sprintf(buffer, "Hum=%02d", humidity);
        lcd_write_string_4d(buffer);
        lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
        _delay_ms(1000);    
        
    }
    
    return 0;
}
