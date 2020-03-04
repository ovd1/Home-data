#include <stdio.h>
#include <fcntl.h>  
#include <termios.h> 
#include <unistd.h>  
#include <errno.h>   

#include "uart.h"

int get_temp_humid(char *read_buffer, int size)
{
    int fd;
    
    fd = open("/dev/ttyACM0",O_RDWR | O_RDWR);
    
    if(fd == -1)    /* Error Checking */
        printf("\n  Error! in Opening ttyACM0  ");
    else
        printf("\n  ttyACM0 Opened Successfully ");

    struct termios SerialPortSettings;

    tcgetattr(fd, &SerialPortSettings);

    cfsetispeed(&SerialPortSettings,B9600);
    cfsetospeed(&SerialPortSettings,B9600);

    SerialPortSettings.c_cflag &= ~PARENB;
    SerialPortSettings.c_cflag &= ~CSTOPB;
    SerialPortSettings.c_cflag |=  CS8;  
   // SerialPortSettings.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
    SerialPortSettings.c_cflag &= ~CRTSCTS;
    SerialPortSettings.c_cflag |= CREAD | CLOCAL; 
    
    SerialPortSettings.c_lflag &= ~ICANON;
    SerialPortSettings.c_lflag &= ~ECHO; 
    SerialPortSettings.c_lflag &= ~ECHOE;
    SerialPortSettings.c_lflag &= ~ECHONL;
    SerialPortSettings.c_lflag &= ~ISIG; 
    
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
    //SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */
    SerialPortSettings.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); 
    
    SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/
    SerialPortSettings.c_oflag &= ~ONLCR; 
    /* Setting Time outs */
    SerialPortSettings.c_cc[VMIN] = 10; /* Read at least 10 characters */
    SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */


    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* Set the attributes to the termios structure*/
        printf("\n  ERROR ! in Setting attributes");
    else
        printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");
        
    /* Read data from serial port */

    tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */

    
    int  bytes_read = 0;    /* Number of bytes read by the read() system call */
    

    bytes_read = read(fd, read_buffer, size); /* Read the data                   */
        

    close(fd); 
    return bytes_read;

}
 
