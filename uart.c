#include <stdio.h>
#include <fcntl.h>  
#include <termios.h> 
#include <unistd.h>  
#include <errno.h>   

#include "uart.h"

//#define DEBUG_LOGS

#ifdef DEBUG_LOGS
#define D_LOG printf
#else
#define D_LOG
#endif

int get_temp_humid(char *read_buffer, int size)
{
    int fd;
    
    fd = open("/dev/ttyACM0",O_RDWR);
    
    if(fd == -1)    
        printf("\n  Error! in Opening ttyACM0  ");
    else
        D_LOG("\n  ttyACM0 Opened Successfully ");

    struct termios SerialPortSettings;

    tcgetattr(fd, &SerialPortSettings);

    cfsetispeed(&SerialPortSettings,B9600);
    cfsetospeed(&SerialPortSettings,B9600);

    SerialPortSettings.c_cflag &= ~PARENB;
    SerialPortSettings.c_cflag &= ~CSTOPB;
    SerialPortSettings.c_cflag |=  CS8;  
    SerialPortSettings.c_cflag &= ~CRTSCTS;
    SerialPortSettings.c_cflag |= CREAD | CLOCAL; 
    
    SerialPortSettings.c_lflag &= ~ICANON;
    SerialPortSettings.c_lflag &= ~ECHO; 
    SerialPortSettings.c_lflag &= ~ECHOE;
    SerialPortSettings.c_lflag &= ~ECHONL;
    SerialPortSettings.c_lflag &= ~ISIG; 
    
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);       
    SerialPortSettings.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); 
    
    SerialPortSettings.c_oflag &= ~OPOST;
    SerialPortSettings.c_oflag &= ~ONLCR; 
    SerialPortSettings.c_cc[VMIN] = size; 
    SerialPortSettings.c_cc[VTIME] = 0; 


    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) 
        D_LOG("\n  ERROR ! in Setting attributes");
    else
        D_LOG("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");
        

    tcflush(fd, TCIFLUSH);   

    int  bytes_read = 0;    
    
    write(fd, "m", 1);
    bytes_read = read(fd, read_buffer, size); 
    if (bytes_read != size) {
        fprintf(stderr, "read=%d requested=%d\n", bytes_read, size);
    }
        

    close(fd); 
    return bytes_read;

}
 
