#include "uart.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    for (;;) {
        char buf[9];
        get_temp_humid(buf, sizeof(buf) - 1);
        buf[8] = '\0';
        
        printf("%s %lu\n", buf, rand() % 500000);
        
    }
        
    return 0;
}
