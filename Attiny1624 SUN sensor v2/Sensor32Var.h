/*
 * Sensor32Var.h
 *
 * Created: 2024-10-11 22:49:27
 *  Author: Saulius
 */ 


#ifndef SENSOR32VAR_H_
#define SENSOR32VAR_H_

float angles_0[LINE_0_TOTAL]={0.0, 24.0, 48.0, 72.0, 96.0, 120.0, 144.0, 168.0, 192.0, 216.00, 240.0, 264.0, 288.0, 312.0, 336.0}; //0 Row angles
float angles_1[LINE_1_TOTAL]={0.0, 30.0, 60.0, 90.0, 120.0, 150.0, 180.0, 210.0, 240.0, 270.0, 300.0, 330.0}; //1 Row angles
float angles_2[LINE_2_TOTAL]={0.0, 90.0, 180.0, 270.0}; //2 Row angles
float angles_3[LINE_3_TOTAL]={360.0}; //3 Row angle
float angles_el[TOTAL_ROWS]={0.0, 30.0, 60.0, 90.0};//angles of each row for elevation

uint8_t sensor_placement0[16]={12, 25, 3, 26, 21, 2, 22, 10, 13, 27, 14, 28, 23, 11, 24, 0}; //MUX0 (CD4067) sensor array
uint8_t sensor_placement1[16]={4, 29, 15, 30, 6, 1, 18, 7, 16, 31, 5, 17, 19, 8, 20, 9}; //MUX1 (CD4067) sensor array

int16_t sensor_data[TOTAL_SENSORS]={0};
int16_t sensor_top_of_rows[TOTAL_ROWS]={0};

ROW first = {17, 15, angles_0, sensor_data, 0, 0.48387}; //creating objekts for each row to store all requaired data.
ROW second = {5, 12, angles_1, sensor_data, 0, 0.38710};
ROW third = {1, 4, angles_2, sensor_data, 0, 0.12903};
ROW fourth = {0, 1, angles_3, sensor_data, 0, 0.0}; //fourth row top angle is 90, becouse if sun is over this sensor azimuth is 0 assume like that I just simple cant predict at this point azimuth angle (impossible) 
ROW fifth = {0, 4, angles_el, sensor_top_of_rows, 0, 0.0}; //for elevation

#endif /* SENSOR32VAR_H_ */