#include <stdio.h>
#include <unistd.h>  
 
#include "uart.h"


void main(void)
{
    int i = 0;
    while (1){
        char read_buffer[32];
        int  bytes_read = get_temp_humid(read_buffer, sizeof(read_buffer));
        
        printf("\n\n  Bytes Rxed - %d", bytes_read); /* Print the number of bytes read */
        printf("\n\n  ");

        for(i=0;i<bytes_read;i++){ /*printing only the received characters*/
            printf("%c",read_buffer[i]);
        }
        putchar('\n');
        sleep(1);
    }
}
 
