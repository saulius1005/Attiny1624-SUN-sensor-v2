/*
 * Sensor32.c
 *
 * Created: 2024-10-11 21:29:28
 *  Author: Saulius
 */ 
#include "Settings.h"
#include "Sensor32Var.h"

uint16_t level(){
	uint32_t AverageLevel = 0;
	for(uint8_t i = 0; i < TOTAL_SENSORS; i++){ //calculating average light level of 32 sensors
		AverageLevel += sensor_data[i];
	}
	return AverageLevel/TOTAL_SENSORS;
}


/*void Noramlization(){

	float a[TOTAL_SENSORS]={0.0},
		  b[TOTAL_SENSORS]={0.0},
		  N_x[TOTAL_SENSORS]={0.0};

	for (uint8_t i = 0; i < TOTAL_SENSORS; i++){
			a[i] = ( sensor_koef_MAX[i] - sensor_koef_MIN[i] ) / ( log10(sens_koef_max[i]) -  log10(sens_koef_min[i]) );
			b[i] =  sensor_koef_MAX[i] - a[i] * log10(sens_koef_max[i]);

			N_x[i] = a[i] * log10(sensor_data[i]) + b[i];
			sensor_data[i] = (4095-sensor_data[i]) *  N_x[i];
		}
}*/

void ReadSensorsValues(){ // reading all voltages from 32 sensor
	int32_t min = 4096;
	for (uint8_t i = 0; i < 16; i++) {
		int32_t reader = 0,
				sum = 0,
				sumb = 0,
				readerb = 0;
		PORTA.OUT = i*16;
		_delay_us(200);
		//_delay_ms(10);
		for (uint32_t a= 0; a< 50000; a++){
			reader = /*2048+*/(int16_t)ADC0_Read(ADC_MUX0); //Read
			sum += reader;
			//_delay_loop_2(a+1);
			readerb = /*2048+*/(int16_t)ADC0_Read(ADC_MUX1);
			sumb += readerb;
			//_delay_loop_2(a+1);
		}
		sensor_data[sensor_placement0[i]] = (int32_t)sum/50000;
		sensor_data[sensor_placement1[i]] = (int32_t)sumb/50000;	
		if(sensor_data[sensor_placement0[i]] < min )
		min = sensor_data[sensor_placement0[i]];
		if(sensor_data[sensor_placement1[i]] < min )
		min = sensor_data[sensor_placement1[i]];

	}
	for (uint8_t a= 0; a<TOTAL_SENSORS;a++){
		sensor_data[a] += abs(min);
	}
}

float GetCoeficients(uint8_t sensor){
	uint32_t sum=0;
	double koef = 0.0;
	for (uint16_t a= 0; a<50; a++){
		for (uint8_t i = 0; i < 16; i++) {
			PORTA.OUT = 16 * i;
			_delay_ms(20);
			sensor_data[sensor_placement1[i]]= ADC0_Read(ADC_MUX1); //Read
			sensor_data[sensor_placement0[i]]= ADC0_Read(ADC_MUX0); //Read
			
		}
		sum +=sensor_data[sensor];
	}
	return koef = sum/50;
	
}