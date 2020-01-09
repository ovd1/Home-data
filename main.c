#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#define DHT11_ERROR 255

#define lcd_D7_port     PORTD                   
#define lcd_D7_bit      PORTD7
#define lcd_D7_ddr      DDRD

#define lcd_D6_port     PORTD                   
#define lcd_D6_bit      PORTD6
#define lcd_D6_ddr      DDRD

#define lcd_D5_port     PORTD                   
#define lcd_D5_bit      PORTD5
#define lcd_D5_ddr      DDRD

#define lcd_D4_port     PORTD                   
#define lcd_D4_bit      PORTD4
#define lcd_D4_ddr      DDRD

#define lcd_E_port      PORTB                   
#define lcd_E_bit       PORTB1
#define lcd_E_ddr       DDRB

#define lcd_RS_port     PORTB                   
#define lcd_RS_bit      PORTB0
#define lcd_RS_ddr      DDRB

#define lcd_LineOne     0x00                    
#define lcd_LineTwo     0x40  


//setup parameters
#define DHT11_DDR DDRB
#define DHT11_PORT PORTB
#define DHT11_PIN PINB
#define DHT11_INPUTPIN PB5

 

#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0b00110000          // reset the LCD
#define lcd_FunctionSet4bit 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0b10000000          // set cursor position



//uint8_t program_temp[]   = "Temp: ";
//uint8_t program_hum[]  = "Hum: ";

void lcd_write_4(uint8_t);
void lcd_write_instruction_4d(uint8_t);
void lcd_write_character_4d(uint8_t);
void lcd_write_string_4d(uint8_t *);
void lcd_init_4d(void);

uint8_t dht11_getdata(uint8_t);
uint8_t dht11_gettemperature(); 
uint8_t dht11_gethumidity();


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

    //lcd_write_instruction_4d(lcd_DisplayOn);
    
    //lcd_write_instruction_4d(lcd_FunctionReset);
    
    while(1){
        humidity = dht11_gethumidity();
        _delay_ms(1000); 
        temperature = dht11_gettemperature();
        
        
        sprintf(buffer, "T=%02d", temperature);
        lcd_write_string_4d(buffer);

        lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
                        

        sprintf(buffer, "H=%02d", humidity);
        lcd_write_string_4d(buffer);
        //while(1);
        lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
        _delay_ms(1000);    
        
    }
    
    return 0;
}


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


/*
 * get data from dht11
 */
uint8_t dht11_getdata(uint8_t select) {
	uint8_t bits[5];
	uint8_t i,j = 0;

	memset(bits, 0, sizeof(bits));

	//reset port
	DHT11_DDR |= (1<<DHT11_INPUTPIN); //output
	DHT11_PORT |= (1<<DHT11_INPUTPIN); //high
	_delay_ms(100);

	//send request
	DHT11_PORT &= ~(1<<DHT11_INPUTPIN); //low
	_delay_ms(18);
	DHT11_PORT |= (1<<DHT11_INPUTPIN); //high
	_delay_us(1);
	DHT11_DDR &= ~(1<<DHT11_INPUTPIN); //input
	_delay_us(39);

	//check start condition 1
	if((DHT11_PIN & (1<<DHT11_INPUTPIN))) {
		return DHT11_ERROR;
	}
	_delay_us(80);
	//check start condition 2
	if(!(DHT11_PIN & (1<<DHT11_INPUTPIN))) {
		return DHT11_ERROR;
	}
	_delay_us(80);

	//read the data
	for (j=0; j<5; j++) { //read 5 byte
		uint8_t result=0;
		for(i=0; i<8; i++) {//read every bit
			while(!(DHT11_PIN & (1<<DHT11_INPUTPIN))); //wait for an high input
			_delay_us(30);
			if(DHT11_PIN & (1<<DHT11_INPUTPIN)) //if input is high after 30 us, get result
				result |= (1<<(7-i));
			while(DHT11_PIN & (1<<DHT11_INPUTPIN)); //wait until input get low
		}
		bits[j] = result;
	}

	//reset port
	DHT11_DDR |= (1<<DHT11_INPUTPIN); //output
	DHT11_PORT |= (1<<DHT11_INPUTPIN); //low
	_delay_ms(100);

	//check checksum
	if (bits[0] + bits[1] + bits[2] + bits[3] == bits[4]) {
		if (select == 0) { //return temperature
			return(bits[2]);
		} else if(select == 1){ //return humidity
			return(bits[0]);
		}
	}

	return DHT11_ERROR;
}

/*
 * get temperature (0..50C)
 */
uint8_t dht11_gettemperature() {
	uint8_t ret = dht11_getdata(0);
	if(ret == DHT11_ERROR)
		return -1;
	else
		return ret;
}

/*
 * get humidity (20..90%)
 */
uint8_t dht11_gethumidity() {
	uint8_t ret = dht11_getdata(1);
	if(ret == DHT11_ERROR)
		return -1;
	else
		return ret;
}



