#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "gnss.h"

extern double lat,lng,vel,head,hdop,hgt;
extern long int time ,date;
extern int nsv,gpsqa;
/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B9600
/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
volatile int STOP=FALSE;

int main()
{
int fd,c, res;
struct termios oldtio,newtio;
char buf[255];
char data[255];
FILE * file;

double update_filter(double measurement, double predict, double gain);

/*----------------------KALMAN GAIN PARAMETERS --------------------------*/
double cur_lat =0.0,cur_lng=0.0,prev_lat=0.0,prev_lng=0.0;
double pred_lat,pred_lng,sum_lat,sum_lng,out_lat=0.0,out_lng=0.0;
int n =0;
float gain=0.5;

/*-----------------------------------------------------------------------*/


fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
if (fd <0) {perror(MODEMDEVICE);}
tcgetattr(fd,&oldtio); /* save current serial port settings */
bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
/*
BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
CRTSCTS : output hardware flow control (only used if the cable has
all necessary lines. See sect. 7 of Serial−HOWTO)
CS8 : 8n1 (8bit,no parity,1 stopbit)
CLOCAL : local connection, no modem contol
CREAD : enable receiving characters
*/
newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
/*
IGNPAR : ignore bytes with parity errors
ICRNL : map CR to NL (otherwise a CR input on the other computer
will not terminate input)
otherwise make device raw (no other input processing)
*/
newtio.c_iflag = IGNPAR | ICRNL;
/*
Raw output.
*/
newtio.c_oflag = 0;
/*
ICANON : enable canonical input
disable all echo functionality, and don't send signals to calling program
*/
newtio.c_lflag = ICANON;
/*

now clean the modem line and activate the settings for the port
*/
tcflush(fd, TCIFLUSH);
tcsetattr(fd,TCSANOW,&newtio);

/*
file = fopen("filt.buf", "wb");
if (file == NULL) {
    printf("Error in creating file");
    return 1;
  }
printf("File created.");
*/

while (STOP==FALSE) { /* loop until we have a terminating condition */

res = read(fd,buf,255);
buf[res]=0; 


id_filter(buf);

if(lat !=0.0 && lng !=0.0){
   n+=1;
   cur_lat = lat;cur_lng = lng;
   if(prev_lat==0.0 && prev_lng==0.0){
   prev_lat = cur_lat;
   prev_lng = cur_lng; 
   }
   
   out_lat= update_filter(cur_lat, prev_lat, gain);
   out_lng= update_filter(cur_lng, prev_lng, gain);
   
   sum_lat+=out_lat;
   sum_lng+=out_lng;
   
   pred_lat= (sum_lat/n);
   pred_lng= (sum_lng/n);
   
   sprintf(data,"Lat:%f Lng:%f \n",out_lat,out_lng);
   printf("%s ",data);
   
   sprintf(data,"Time:%ld Date:%ld speed:%.2f Lat:%f Lng:%f head:%.2f hdop:%.2f height:%.2f \n",time,date,vel,lat,lng,head,hdop,hgt);
   printf("%s ",data);

  prev_lat=pred_lat;
  prev_lng=pred_lng;
  
  if(n>=4){
  n=0;
  sum_lat=0;
  sum_lng=0;
  }

//fputs(data, file);
}


/*for(int i=0; i<strlen(data); i+=1){
     char ch = data[i];
     fputc(ch,file);
   }*/

}
/* restore the old port settings */
tcsetattr(fd,TCSANOW,&oldtio);
}

double update_filter(double measurement, double predict, double gain){
    double update = predict + gain*(measurement-predict);
    return update;
}



