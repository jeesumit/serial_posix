#ifndef GNSS_H
#define GNSS_H


void id_rmc(char bufrmc[]);
void id_filter(char buffer[]);
void id_gga(char bufgga[]);
float lat_filt(float def);
float lng_filt(float kef);

#endif
