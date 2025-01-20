/*
 * CosmosVar.h
 *
 * Created: 2024-11-21 12:39:56
 *  Author: Saulius
 */ 


#ifndef COSMOSVAR_H_
#define COSMOSVAR_H_

#define DEG_TO_RAD 0.01745329251994329576923690768489 //pi/180
#define RAD_TO_DEG 57.295779513082320876798154814105 //180/pi
#define BASE_TIMEZONE_OFFSET 2 // Bazinë laiko juosta: GMT+2 (þiemà)

SolarPositionParameters solar_params = {
	.latitude = 0,
	.longitude = 0,
	.altitude = 84.0,
	.year = 2024,
	.month = 11,
	.day = 20,
	.hour = 16,
	.minute = 0,
	.second = 0
};


#endif /* COSMOSVAR_H_ */