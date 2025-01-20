/*
 * Cosmos.h
 *
 * Created: 2024-11-21 12:39:43
 *  Author: Saulius
 */ 


#ifndef COSMOS_H_
#define COSMOS_H_

typedef struct {
	double latitude;
	double longitude;
	double altitude;
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
} SolarPositionParameters;

extern SolarPositionParameters solar_params;

#endif /* COSMOS_H_ */