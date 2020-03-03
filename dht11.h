#ifndef DHT11_H_
#define DHT11_H_

#include <avr/io.h>
#include <stdio.h>


#define DHT11_ERROR 255 

#define DHT11_DDR DDRB
#define DHT11_PORT PORTB
#define DHT11_PIN PINB
#define DHT11_INPUTPIN PB5


uint8_t dht11_getdata(uint8_t);
uint8_t dht11_gettemperature(); 
uint8_t dht11_gethumidity();

#endif
