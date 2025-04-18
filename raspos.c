#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void id_filter(char buffer[]);
float lat_filt(float def);
float lng_filt(float kef);

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
char *token;
/*
Open modem device for reading and writing and not as controlling tty
because we don't want to get killed if linenoise sends CTRL−C.
*/
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
terminal settings done, now handle input
In this example, inputting a 'z' at the beginning of a line will
exit the program.
*/
while (STOP==FALSE) { /* loop until we have a terminating condition */
/* read blocks program execution until a line terminating character is
input, even if more than 255 chars are input. If the number
of characters read is smaller than the number of chars available,
subsequent reads will return the remaining chars. res will be set
to the actual number of characters actually read */
res = read(fd,buf,255);
buf[res]=0; /* set end of string, so we can printf */
/*token = strtok(buf,",");
while(token!=NULL){
if(strcmp("$GNRMC",token)== 0){
printf("%s --Found",token);
}

if(strcmp("$GNGGA",token)==0){
printf("%s --Found",token);
}
printf("%s \n",token);
token = strtok(NULL,",");
}*/
//printf(":%s:%d\n", buf, res);
id_filter(buf);
//id_gga(buf);
//printf(":%s:%d\n", buf, res);
}
/* restore the old port settings */
tcsetattr(fd,TCSANOW,&oldtio);
}






void id_filter(char buffer[]){
char *fil;
int i=0 ,j=0,d=0,k=0,l=0;
double lat,lng,lata,lnga;

fil=strtok(buffer,",");
while(fil!=NULL){
if(strcmp("$GNRMC",fil)==0){
i=0;
j=1;
printf("%s --FOUND %d\n",fil,i);
}

if(strcmp("$GNGGA",fil)==0){
k=0;
l=1;
printf("%s --FOUND %d\n",fil,k);
}



if(i==1 && j==1){
printf("Time:%s %f\n",fil,atof(fil));
}
if(i==2 && j==1){
if(strcmp("A",fil)==0){
printf("%s \n",fil);
j=1;
}else if(strcmp("V",fil)==0){
j=0;
}
}

if(i==3 && j==1){
lat = lat_filt(atof(fil));
printf("Lat:%f\n",lat);
}
if(i==4 && j==1){
printf("%s \n",fil);
}

if(i==5 && j==1){
lng = lng_filt(atof(fil));
printf("Lng:%f\n",lng);
}

if(i==6 && j==1){
printf("%s \n ",fil);
}

if(i==7 && j==1){
printf("spd:%s %f \n",fil,atof(fil));
}

if(i==8 && j==1){
if(atof(fil) >=0.0 && atof(fil) <=360.0){
printf("Head:%s %f -- \n",fil,atof(fil));
d=1;
}
else{
d=0;
printf("Date:%s %d \n",fil,atoi(fil));
}
}
if(i==9 && j==1 && d==1){
printf("Date:%s %d \n",fil,atoi(fil));
}
i+=1;
/*------------------------------*/
if(k==1 && l==1){
printf("Time:%s %f\n",fil,atof(fil));
}
if(k==2 && l==1){
lata = lat_filt(atof(fil));
printf("Lat:%f \n",lata);
}
if(k==3 && l==1){
printf("%s \n",fil);
}

if(k==4 && l==1){
lnga = lng_filt(atof(fil));
printf("Lng:%f\n",lnga);
}

if(k==5 && l==1){
printf("%s \n ",fil);
}

if(k==6 && l==1){
printf("GPS Quality indicator:%s %d \n",fil,atoi(fil));
}

if(k==7 && l==1){
printf("Number of SV:%s %d \n",fil,atoi(fil));
}

if(k==8 && l==1){
printf("HDOP:%s %f \n",fil,atof(fil));
}


if(k==9 && l==1){
printf("Orthometric height:%s %f \n",fil,atof(fil));
}


if(k==10 && l==1){
printf(":%s \n",fil);
}

if(k==11 && l==1){
printf("Geoid separation:%s %f \n",fil,atof(fil));
}

if(k==12 && l==1){
printf(":%s \n",fil);
}

if(k==13 && l==1){
printf(":%s %f \n",fil,atof(fil));
}

if(k==14 && l==1){
printf("GPS Age:%s %d \n",fil,atoi(fil));
}

if(k==15 && l==1){
printf("Reference station ID:%s %d \n",fil,atoi(fil));
}
k+=1;
fil=strtok(NULL,",");
}
}


void id_gga(char bufgga[]){
char *filg;
int l=0 ,k=0;
double lata,lnga;

filg=strtok(bufgga,",");
while(filg!=NULL){
if(strcmp("$GNGGA",filg)==0){
k=0;
l=1;
printf("%s --FOUND %d\n",filg,k);
}
printf("%s %d %d\n",filg,k,l);
/*
if(k==1 && l==1){
printf("Time:%s %f\n",filg,atof(filg));
}


if(k==2 && l==1){
lata = atof(filg);
printf("Lat:%s %f \n",filg,lata);
}
if(k==3 && l==1){
printf("%s \n",filg);
}

if(k==4 && l==1){
lnga = atof(filg);
printf("Lng:%s %f\n",filg,lnga);
}

if(k==5 && l==1){
printf("%s \n ",filg);
}

if(k==6 && l==1){
printf("GPS Quality indicator:%s %d \n",filg,atoi(filg));
}

if(k==7 && l==1){
printf("Number of SV:%s %d \n",filg,atoi(filg));
}

if(k==8 && l==1){
printf("HDOP:%s %f \n",filg,atof(filg));
}


if(k==9 && l==1){
printf("Orthometric height:%s %f \n",filg,atof(filg));
}


if(k==10 && l==1){
printf(":%s \n",filg);
}

if(k==11 && l==1){
printf("Geoid separation:%s %f \n",filg,atof(filg));
}

if(k==12 && l==1){
printf(":%s \n",filg);
}

if(k==13 && l==1){
printf(":%s %f \n",filg,atof(filg));
}

if(k==14 && l==1){
printf("GPS Age:%s %d \n",filg,atoi(filg));
}

if(k==15 && l==1){
printf("Reference station ID:%s %d \n",filg,atoi(filg));
}
*/
k+=1;
filg=strtok(NULL,",");
}
}

float lng_filt(float kef){
    float longitude = 0.0;
    float k_lng_deg=(kef*0.01);
    unsigned int deglng = (int)k_lng_deg;
    if(deglng > 68 && 97 > deglng){
        float seclng = (kef- (float)deglng*100)/60;
        longitude = (float)deglng + seclng;

    }

    return longitude;
}

float lat_filt(float def) {
    float latitude = 0.0;
    float k_lat_deg=(def*0.01);
    unsigned int deg = (int)k_lat_deg;
    if(deg > 8 && 37 > deg){
        float sec = (def- (float)deg*100)/60;
        latitude = (float)deg + sec;
    }

    return latitude;
}


