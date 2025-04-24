#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gnss.h"

double lat,lng,vel,head,hdop,hgt;
long int time ,date;
int nsv,gpsqa;


void id_filter(char buffer[]){
char *fil;
int i=0 ,j=0,d=0,k=0,l=0;

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
time=atoi(fil);
//printf("Time:%s %f\n",fil,atof(fil));
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
//printf("Lat:%f\n",lat);
}
if(i==4 && j==1){
printf("%s \n",fil);
}

if(i==5 && j==1){
//lngk = fil;
lng = lng_filt(atof(fil));
//printf("Lng:%f\n",lng);
}

if(i==6 && j==1){
printf("%s \n ",fil);
}

if(i==7 && j==1){
vel=atof(fil);
//printf("spd:%s %f \n",fil,atof(fil));
}

if(i==8 && j==1){
if(atof(fil) >=0.0 && atof(fil) <=360.0){
//printf("Head:%s %f -- \n",fil,atof(fil));
head=atof(fil);
d=1;
}
else{
d=0;
date = atoi(fil);
//printf("Date:%s %d \n",fil,atoi(fil));
}
}
if(i==9 && j==1 && d==1){
date=atoi(fil);
//printf("Date:%s %d \n",fil,atoi(fil));
}
i+=1;
/*------------------------------*/
if(k==1 && l==1){
time = atoi(fil);
//printf("Time:%s %f\n",fil,atof(fil));
}
if(k==2 && l==1){
lat = lat_filt(atof(fil));
//printf("Lat:%f \n",lat);
}
if(k==3 && l==1){
printf("%s \n",fil);
}

if(k==4 && l==1){
lng = lng_filt(atof(fil));
//printf("Lng:%f\n",lng);
}

if(k==5 && l==1){
printf("%s \n ",fil);
 }

if(k==6 && l==1){
gpsqa = atoi(fil);
//printf("GPS Quality indicator:%s %d \n",fil,atoi(fil));
}

if(k==7 && l==1){
nsv = atoi(fil);
//printf("Number of SV:%s %d \n",fil,atoi(fil));
}

if(k==8 && l==1){
hdop = atof(fil);
//printf("HDOP:%s %f \n",fil,atof(fil));
}


if(k==9 && l==1){
hgt = atof(fil);
//printf("Orthometric height:%s %f \n",fil,atof(fil));
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

void id_rmc(char bufrmc[]){
char *filr;
int i=0 ,j=0,d=0;

filr=strtok(bufrmc,",");
while(filr!=NULL){
if(strcmp("$GNRMC",filr)==0){
i=0;
j=1;
printf("%s --FOUND %d\n",filr,i);
}

if(i==1 && j==1){
time=atoi(filr);
//printf("Time:%s %f\n",fil,atof(fil));
}
if(i==2 && j==1){
if(strcmp("A",filr)==0){
printf("%s \n",filr);
j=1;
}else if(strcmp("V",filr)==0){
j=0;
 }
}

if(i==3 && j==1){
lat = lat_filt(atof(filr));
//printf("Lat:%f\n",lat);
}
if(i==4 && j==1){
printf("%s \n",filr);
}

if(i==5 && j==1){
//lngk = fil;
lng = lng_filt(atof(filr));
//printf("Lng:%f\n",lng);
}

if(i==6 && j==1){
printf("%s \n ",filr);
}

if(i==7 && j==1){
vel=atof(filr);
//printf("spd:%s %f \n",fil,atof(fil));
}

if(i==8 && j==1){
if(atof(filr) >=0.0 && atof(filr) <=360.0){
//printf("Head:%s %f -- \n",fil,atof(fil));
head=atof(filr);
d=1;
}
else{
d=0;
date = atoi(filr);
//printf("Date:%s %d \n",fil,atoi(fil));
}
}
if(i==9 && j==1 && d==1){
date=atoi(filr);
//printf("Date:%s %d \n",fil,atoi(fil));
}
i+=1;
filr=strtok(NULL,",");
}
}

void id_gga(char bufgga[]){
char *filg;
int l=0 ,k=0;

filg=strtok(bufgga,",");
while(filg!=NULL){
if(strcmp("$GNGGA",filg)==0){
k=0;
l=1;
printf("%s --FOUND %d\n",filg,k);
}


/*------------------------------*/
if(k==1 && l==1){
time = atoi(filg);
//printf("Time:%s %f\n",fil,atof(fil));
}
if(k==2 && l==1){
lat = lat_filt(atof(filg));
//printf("Lat:%f \n",lat);
}
if(k==3 && l==1){
printf("%s \n",filg);
}

if(k==4 && l==1){
lng = lng_filt(atof(filg));
//printf("Lng:%f\n",lng);
}

if(k==5 && l==1){
printf("%s \n ",filg);
 }

if(k==6 && l==1){
gpsqa = atoi(filg);
//printf("GPS Quality indicator:%s %d \n",fil,atoi(fil));
}

if(k==7 && l==1){
nsv = atoi(filg);
//printf("Number of SV:%s %d \n",fil,atoi(fil));
}

if(k==8 && l==1){
hdop = atof(filg);
//printf("HDOP:%s %f \n",fil,atof(fil));
}


if(k==9 && l==1){
hgt = atof(filg);
//printf("Orthometric height:%s %f \n",fil,atof(fil));
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
