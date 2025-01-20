/*
 * Filter.c
 *
 * Created: 2024-09-17 13:39:15
 *  Author: Saulius
 */ 
#include "Settings.h"
#include "FilterVar.h"

void azimuth_average_angle(uint8_t scale) {	 //calculating average azimuth according to each row weight (coeficients) and later those weight is corrected according to elevation data
	ROW* rows[4] = {&first, &second, &third, &fourth};// Pointer array to rows

	const float DEG_TO_RAD = M_PI / 180.0; // Degrees to radians constant
	const float RAD_TO_DEG = 180.0 / M_PI; // Radians to degrees constant
	const float alpha = 0.5;// weight coeficient for each row number of sensors coeficients
	const float beta = 0.5; // weight coeficient for elevation data

	float total_weighted_x = 0.0;
	float total_weighted_y = 0.0;
	float total_elevation_weight = 0.0;

	for (uint8_t i = 0; i < TOTAL_ROWS; i++){
		total_elevation_weight += (float)sensor_top_of_rows[i]; // calculating overall data weight
	}


	for (uint8_t i = 0; i < TOTAL_ROWS/*3*/; /*++i*/i++){		
		float angle_rad = ((float)rows[i]->top_angle / scale) * DEG_TO_RAD;// Scale the angle and convert to radians		
		float weight_correction = (alpha * rows[i]->weight) + (beta * (total_elevation_weight / sensor_top_of_rows[i])); //weight correction according elevation data of each max values of rows
		total_weighted_x += cosf(angle_rad) * /*rows[i]->weight*/weight_correction;// Precompute cosine and sine of the angle, then multiply by weight
		total_weighted_y += sinf(angle_rad) * /*rows[i]->weight*/weight_correction;
	}
	float average_angle_rad = atan2f(total_weighted_y, total_weighted_x);// Calculate the average angle in radians
	float average_angle_deg = average_angle_rad * RAD_TO_DEG;// Convert the average angle to degrees
	if (average_angle_deg < 0) {// Normalize angle to [0, 360) range
		average_angle_deg += 360.0;
	}
	AZIMUTH.total_average_angle = average_angle_deg * scale;// Store the scaled total average angle in AZIMUTH
}

void update_angle(FINAL_ANGLE* angle) { //Best optimized version without loop 
	angle->sum -= angle->storage[angle->indexer]; //removing oldest value from sum
	angle->storage[angle->indexer] = angle->total_average_angle; //adding new value to storage
	angle->sum += angle->total_average_angle; //add new value to sum
	angle->final_angle = angle->sum * SCALE / SUN_AVERAGES; //calculating final average angle according to scale factor 
	angle->indexer = (angle->indexer + 1) % SUN_AVERAGES; //cyclic indexer increasing (if reach SUN_AVERAGES indexer reseting)
}

void moving_average2() { //moving average function
	FINAL_ANGLE* angles[2] = {&AZIMUTH, &ELEVATION}; //array to storage azimuth and elevation pointers
	for (uint8_t i = 0; i < 2; i++) { //updating data for both
		update_angle(angles[i]);
	}
}