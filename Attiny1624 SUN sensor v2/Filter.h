/*
 * FILTER.h
 *
 * Created: 2024-10-12 12:16:30
 *  Author: Saulius
 */ 


#ifndef FILTER_H_
#define FILTER_H_

#define SCALE 1				//SCALE is using to show 1 digit after integer value. Exmp.: az 123, el 33 (SCALE 1); az 122.9, el 33.2 (SCALE 10)
#define SUN_AVERAGES 5	//Moving average samples count upto 200

typedef struct { //creating structure for data storage for each row
	uint8_t indexer; //index for moving average
	float total_average_angle; //sum of angle including each row weight if aplied
	double *storage;
	float final_angle;
	double sum;
	double cosmos_angle;
} FINAL_ANGLE;

extern FINAL_ANGLE AZIMUTH;
extern FINAL_ANGLE ELEVATION;

#endif /* FILTER_H_ */