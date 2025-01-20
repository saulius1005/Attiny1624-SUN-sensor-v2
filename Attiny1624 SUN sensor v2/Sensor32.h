/*
 * Sensor32.h
 *
 * Created: 2024-10-11 21:16:00
 *  Author: Saulius
 */ 


#ifndef SENSOR32_H_
#define SENSOR32_H_

#define TOTAL_SENSORS 32
#define TOTAL_ROWS 4 //elevation accurity 22.5 Degree (90 / 4) 
#define TOP_ELEVATION_ANGLE 90

#define LINE_0_TOTAL 15 //24 Degree/ Sensor accurity (360 / 15) (Bottom line- straight to horizont)
#define LINE_1_TOTAL 12 //30 Degree/ Sensor accurity (360 / 12) (30 degree from horizont to zenith)
#define LINE_2_TOTAL 4 //90 Degree/ Sensor accurity	 (360 / 4) (90 degree from horizont to zenith)
#define LINE_3_TOTAL 1 //360 Degree/ Sensor accurity (Top line/ single senor straight to zenith)
#define ADC_MUX0 ADC_MUXPOS_AIN2_gc			//multiplexer channel 0 is AIN 2 (PA2)
#define ADC_MUX1 ADC_MUXPOS_AIN1_gc		//multiplexer channel 1 for AIN 1 (PA1)

extern int16_t sensor_data[TOTAL_SENSORS]; //All sensor voltages (inverted)
extern int16_t sensor_top_of_rows[TOTAL_ROWS]; //Calculated top voltages from each row
extern float angles_el[TOTAL_ROWS]; //angles of each row for elevation calculation

typedef struct { //creating structure for data storage for each row
	uint8_t start; //start from sensor
	uint8_t total; //total sensors in row
	float* angles; //pointer to angles array according to row
	int16_t* data; //pointer to array for data storage
	float top_angle;
	float weight; //senor data weight according to sensor count in to each row (accuracy)
} ROW;

extern ROW first; //objects for each row
extern ROW second;
extern ROW third;
extern ROW fourth;
extern ROW fifth;

#endif /* SENSOR32_H_ */