/*
 * FilterVar.h
 *
 * Created: 2024-10-12 12:41:55
 *  Author: Saulius
 */ 


#ifndef FILTERVAR_H_
#define FILTERVAR_H_

double azimuth_storage[SUN_AVERAGES]={0};
double elevation_storage[SUN_AVERAGES]={0};

FINAL_ANGLE AZIMUTH ={0, 0, azimuth_storage, 0, 0, 0}; //0 at magnetic south (for my location)
FINAL_ANGLE ELEVATION ={0, 0, elevation_storage, 0, 0, 0}; 

#endif /* FILTERVAR_H_ */