#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "dht11.h"


uint8_t dht11_getdata(uint8_t select) {
	uint8_t bits[5];
	uint8_t i,j = 0;

	memset(bits, 0, sizeof(bits));

	DHT11_DDR |= (1<<DHT11_INPUTPIN);
	DHT11_PORT |= (1<<DHT11_INPUTPIN);
	_delay_ms(100);

	DHT11_PORT &= ~(1<<DHT11_INPUTPIN);
	_delay_ms(18);
	DHT11_PORT |= (1<<DHT11_INPUTPIN); 
	_delay_us(1);
	DHT11_DDR &= ~(1<<DHT11_INPUTPIN);
	_delay_us(39);

	if((DHT11_PIN & (1<<DHT11_INPUTPIN))) {
		return DHT11_ERROR;
	}
	_delay_us(80);

	if(!(DHT11_PIN & (1<<DHT11_INPUTPIN))) {
		return DHT11_ERROR;
	}
	_delay_us(80);

	for (j=0; j<5; j++) { 
		uint8_t result=0;
		for(i=0; i<8; i++) {
			while(!(DHT11_PIN & (1<<DHT11_INPUTPIN))); 
			_delay_us(30);
			if(DHT11_PIN & (1<<DHT11_INPUTPIN)) 
				result |= (1<<(7-i));
			while(DHT11_PIN & (1<<DHT11_INPUTPIN)); 
		}
		bits[j] = result;
	}


	DHT11_DDR |= (1<<DHT11_INPUTPIN); 
	DHT11_PORT |= (1<<DHT11_INPUTPIN); 
	_delay_ms(100);


	if (bits[0] + bits[1] + bits[2] + bits[3] == bits[4]) {
		if (select == 0) { 
			return(bits[2]);
		} else if(select == 1){ 
			return(bits[0]);
		}
	}

	return DHT11_ERROR;
}


uint8_t dht11_gettemperature() {
	uint8_t ret = dht11_getdata(0);
	if(ret == DHT11_ERROR)
		return -1;
	else
		return ret;
}

uint8_t dht11_gethumidity() {
	uint8_t ret = dht11_getdata(1);
	if(ret == DHT11_ERROR)
		return -1;
	else
		return ret;
}

