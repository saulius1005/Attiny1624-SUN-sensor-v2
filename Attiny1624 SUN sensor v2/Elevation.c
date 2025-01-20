/*
 * Elevation.c
 *
 * Created: 2024-09-17 13:36:11
 *  Author: Saulius
 */ 
#include "Settings.h"

/*uint16_t elevation(){ //elevation calculation using square function
	float angles[3] = {0.0, 45.0, 90.0}, //angles (horizont-zenith) of each sensors row
	step = 90.0 / 89.0,  // interpolation step
	a = 0.0,
	b = 0.0,
	c = 0.0,
	max_value = 0.0,
	max_angle = 0.0,
	current_angle = 0.0,
	current_value = 0.0;

	a = (read.Elev[0] - 2 * read.Elev[1] + read.Elev[2]) / (angles[0] * angles[0] - 2 * angles[1] * angles[1] + angles[2] * angles[2]); //Square function coeficients calculation
	b = (read.Elev[1] - read.Elev[0] - a * (angles[1] * angles[1] - angles[0] * angles[0])) / (angles[1] - angles[0]);
	c = read.Elev[0] - a * (angles[0] * angles[0]) - b * angles[0];

	for (uint8_t i = 0; i < 90; i++) {// calcukating result of square function a*x^2 + b*x + c
		current_value = a * current_angle * current_angle + b * current_angle + c;
		if (current_value > max_value) { //checking angle
			max_value = current_value;
			max_angle = current_angle;
		}
		current_angle += step;
	}
	return (uint16_t)round(max_angle);
}*/


/*void elevation2() {
	float max_value = 0.0; // Naudojame maksimalià leistinà reikðmæ
	float angle = 0.0;
	// Apskaièiuokite interpoliacijos vertes ir raskite maksimalià tarp jø
	for (uint8_t x_interp = 0; x_interp <= TOP_ELEVATION_ANGLE; x_interp++) {
		float result = 0;
			
		for (uint8_t i = 0; i < TOTAL_ROWS; i++) {// Lagrange interpolation
			float term = sensor_top_of_rows[i];
			for (uint8_t j = 0; j < TOTAL_ROWS; j++) {
				if (j != i) {
					term *= (x_interp - angles_el[j]) / (angles_el[i] - angles_el[j]);
				}
			}
			result += term;
		}
		if (result > max_value) {
			max_value = result;
			angle = x_interp;
		}
	}
	ELEVATION.total_average_angle = angle;
}*/



/*void elevation2() {
    float max_value = -1;  // Pradinis maksimalios vertës rodiklis
    float angle = 0.0;     // Kampo, kur pasiekta maksimali vertë, inicializacija

    // Randame maksimalø taðkà nuo 0 iki 90 laipsniø
    for (float x_interp = 0; x_interp <= TOP_ELEVATION_ANGLE; x_interp += 0.1) {
        float angle_rad = x_interp * M_PI / 180.0;  // Kampas radianais
        float result = 0.0; // Inicializuojame interpoliacijos rezultatà

        // Apskaièiuojame interpoliacijà
        for (uint8_t i = 0; i < TOTAL_ROWS; i++) {
            float angle_el_rad = angles_el[i] * M_PI / 180.0;  // Jutiklio kampas radianais
            
            // Skaièiuojame jutiklio svorá
            float weight = sensor_top_of_rows[i];

            // Èia mes naudosime sinusoidiná modelá
            // Naudojame cos(kampas - jutiklio kampas)
            result += weight * cos(angle_rad - angle_el_rad);
        }

        // Tikriname, ar ði reikðmë yra didþiausia
        if (result > max_value) {
            max_value = result;  // Atnaujiname didþiausià vertæ
            angle = x_interp;     // Atkuriame kampà
        }
    }

    // Iðsaugome maksimalø kampà
    ELEVATION.total_average_angle = angle;

    // Debug: spausdinti kampà ir maksimalià vertæ
    printf("Maksimalus kampas: %.2f, Maksimali vertë: %.2f\n", angle, max_value);
}*/



/*// Apskaièiuojame kvadratinës funkcijos koeficientus
void calculateCoefficients(int idx1, int idx2, int idx3, float* a, float* b, float* c) {
	float angle0 = angles_el[idx1];
	float angle1 = angles_el[idx2];
	float angle2 = angles_el[idx3];

	float value0 = sensor_top_of_rows[idx1];
	float value1 = sensor_top_of_rows[idx2];
	float value2 = sensor_top_of_rows[idx3];

	// Kvadratinës funkcijos koeficientø skaièiavimas
	*a = (value0 - 2 * value1 + value2) / ((angle0 - angle1) * (angle0 - angle2));
	*b = (value1 - value0 - (*a) * (angle1 - angle0) * (angle1 + angle0)) / (angle1 - angle0);
	*c = value0 - (*a) * angle0 * angle0 - (*b) * angle0;
}

// Apskaièiuojame maksimalø kampà tarp pasirinktø jutikliø
float findMaxAngle(int idx1, int idx2, int idx3) {
	float a, b, c;

	// Kvadratinës funkcijos koeficientø skaièiavimas
	calculateCoefficients(idx1, idx2, idx3, &a, &b, &c);

	float max_value = -1.0f;  // Inicijuojame maksimalø vertæ
	float max_angle = 0.0f;

	// Perþiûrime kampus 0° iki 90° su maþesniu þingsniu
	for (int i = 0; i <= 90; i += 1) {
		float current_angle = (float)i;
		float current_value = a * current_angle * current_angle + b * current_angle + c;

		if (current_value > max_value) {
			max_value = current_value;
			max_angle = current_angle;
		}
	}

	return max_angle;  // Gràþina float tipo kampà
}

// Apskaièiuojame bendrà maksimalø kampà vidurká
void elevation3() {
	float max_angles[4];
	// Apskaièiuojame maksimalø kampà tarp kiekvienos trijø jutikliø kombinacijos
	max_angles[0] = findMaxAngle(0, 1, 2);
	max_angles[1] = findMaxAngle(0, 1, 3);
	max_angles[2] = findMaxAngle(0, 2, 3);
	max_angles[3] = findMaxAngle(1, 2, 3);

	ELEVATION.total_average_angle = (max_angles[0] + max_angles[1] + max_angles[2] + max_angles[3]) / 4.0f;

}
*/


/*uint16_t elevation() {
	// elevation calculation using square function
	float step = 1.0112359550561797752808988764045, // same as 90.0 / 89.0
		  a = 0.0, b = 0.0, c = 0.0,
		  max_value = 0.0, max_angle = 0.0, current_angle = 0.0, current_value = 0.0,
		  sum_x = 0.0, sum_x2 = 0.0, sum_x3 = 0.0, sum_x4 = 0.0,
		  sum_y = 0.0, sum_xy = 0.0, sum_x2y = 0.0;
	for (uint8_t i = 0; i < 4; i++) {
		sum_x += sensor_top_of_rows[i];
		sum_x2 += sensor_top_of_rows[i] * sensor_top_of_rows[i];
		sum_x3 += sensor_top_of_rows[i] * sensor_top_of_rows[i] * sensor_top_of_rows[i];
		sum_x4 += sensor_top_of_rows[i] * sensor_top_of_rows[i] * sensor_top_of_rows[i] * sensor_top_of_rows[i];
		sum_y += sensor_top_of_rows[i];
		sum_xy += sensor_top_of_rows[i] * sensor_top_of_rows[i];
		sum_x2y += sensor_top_of_rows[i] * sensor_top_of_rows[i] * sensor_top_of_rows[i];
	}
	float denominator = (4 * sum_x2 * sum_x2 - sum_x * sum_x4 - 2 * sum_x * sum_x2 * sum_x2 + 2 * sum_x3 * sum_x3);
	a = (4 * (sum_x2 * sum_xy - sum_x * sum_y) - (sum_x3 * (sum_x2y - sum_y * sum_x)) + sum_x * (sum_x * sum_x2y - sum_y * sum_x3)) / denominator;
	b = (sum_y - a * sum_x2) / 4; // Estimate b, adjusting as necessary
	c = (sum_y - a * sum_x4 - b * sum_x2) / 4; // Estimate c, adjusting as necessary

	for (uint8_t i = 0; i < 90; i++) { // calculating result of square function a*x^2 + b*x + c
		current_value = a * current_angle * current_angle + b * current_angle + c;
		if (current_value > max_value) { // checking angle
			max_value = current_value;
			max_angle = current_angle;
		}
		current_angle += step;
	}
	ELEVATION.total_average_angle = (uint16_t)round(max_angle);
	return (uint16_t)round(max_angle);
}*/



/*// Funkcija rasti spline koeficientams
void calculate_spline_coefficients(float a[], float b[], float c[], float d[]) {
	float h[TOTAL_ROWS-1], alpha[TOTAL_ROWS-1], l[TOTAL_ROWS], mu[TOTAL_ROWS], z[TOTAL_ROWS];
	
	// Apskaièiuojame h ir alpha reikðmes
	for (int i = 0; i < TOTAL_ROWS - 1; i++) {
		h[i] = angles_el[i+1] - angles_el[i];
	}
	for (int i = 1; i < TOTAL_ROWS - 1; i++) {
		alpha[i] = (3.0f / h[i]) * ((float)sensor_top_of_rows[i+1] - (float)sensor_top_of_rows[i])
		- (3.0f / h[i-1]) * ((float)sensor_top_of_rows[i] - (float)sensor_top_of_rows[i-1]);
	}

	// Inicializuojame l, mu, z reikðmes
	l[0] = 1.0f;
	mu[0] = 0.0f;
	z[0] = 0.0f;

	// Apskaièiuojame l, mu, z reikðmes iteratyviai
	for (int i = 1; i < TOTAL_ROWS - 1; i++) {
		l[i] = 2.0f * (angles_el[i+1] - angles_el[i-1]) - h[i-1] * mu[i-1];
		mu[i] = h[i] / l[i];
		z[i] = (alpha[i] - h[i-1] * z[i-1]) / l[i];
	}

	l[TOTAL_ROWS-1] = 1.0f;
	z[TOTAL_ROWS-1] = 0.0f;
	c[TOTAL_ROWS-1] = 0.0f;

	// Atgalinës iteracijos bûdu apskaièiuojame c, b, d koeficientus
	for (int j = TOTAL_ROWS - 2; j >= 0; j--) {
		c[j] = z[j] - mu[j] * c[j+1];
		b[j] = ((float)sensor_top_of_rows[j+1] - (float)sensor_top_of_rows[j]) / h[j]
		- h[j] * (c[j+1] + 2.0f * c[j]) / 3.0f;
		d[j] = (c[j+1] - c[j]) / (3.0f * h[j]);
		a[j] = (float)sensor_top_of_rows[j];  // Konvertuojame uint16_t á float
	}
}

// Funkcija spline interpoliacijai tarp dviejø taðkø
float spline_interpolation(float a[], float b[], float c[], float d[], float x) {
	int i;
	// Surandame intervalà, kuriame yra x (kampas)
	for (i = TOTAL_ROWS - 2; i >= 0; i--) {
		if (x >= angles_el[i]) {
			break;
		}
	}
	float dx = x - angles_el[i];
	// Spline funkcijos apskaièiavimas
	return a[i] + b[i] * dx + c[i] * dx * dx + d[i] * dx * dx * dx;
}*/

/*
void elevation4(){
	 float a[TOTAL_ROWS], b[TOTAL_ROWS], c[TOTAL_ROWS], d[TOTAL_ROWS];
 
	 // Apskaièiuojame spline koeficientus
	 calculate_spline_coefficients(a, b, c, d);

	 float max_value = 0; // Inicializuojame su maþiausia ámanoma reikðme
	 uint16_t max_angle = 0;  // Maksimalios reikðmës kampas, uint16_t tipo
 
	 // Tikriname su 0.1 laipsnio tikslumu
	 for (float angle = 0.0f; angle <= 90.0; angle += 0.1) {
		 float interpolated_value = spline_interpolation(a, b, c, d, angle);

		 // Tikriname, ar reikðmë yra maksimumas
		 if (interpolated_value > max_value) {
			 max_value = interpolated_value;
			 max_angle = (uint16_t)roundf(angle);  // Kampà suapvaliname iki artimiausio sveiko skaièiaus
		 }
	 }
	ELEVATION.total_average_angle = max_angle;
}*/

/*// Lagrange interpolacijos funkcija
double lagrange_interpolate(double x_interp) {
	double result = 0.0;
	
	for (int i = 0; i < TOTAL_ROWS; i++) {
		double term = sensor_top_of_rows[i];
		
		for (int j = 0; j < TOTAL_ROWS; j++) {
			if (j != i) {
				term *= (x_interp - angles_el[j]) / (angles_el[i] - angles_el[j]);
			}
		}
		
		result += term;
	}
	
	return result;
}

void elevation5(){
    
    double max_value = 0,  // Inicializuojame su labai maþa reikðme
	max_angle = angles_el[0];  // Inicializuojame su pirmu kampu

	// Patikriname reikðmes ávairiais kampais, pavyzdþiui, su 0.1 laipsnio þingsniu
	for (double angle = angles_el[0]; angle <= angles_el[TOTAL_ROWS - 1]; angle += 0.1) {
		double value = lagrange_interpolate(angle);
		if (value > max_value) {
			max_value = value;
			max_angle = angle;
		}
	}

    ELEVATION.total_average_angle = max_angle;
}*/

/*void azel(uint8_t Row) {
    ROW *rows[5] = {&first, &second, &third, &fourth, &fifth};
    if (Row != 3) {

        float a[rows[Row]->total], b[rows[Row]->total], c[rows[Row]->total], d[rows[Row]->total];
        float h[rows[Row]->total - 1], alpha[rows[Row]->total - 1], l[rows[Row]->total], mu[rows[Row]->total], z[rows[Row]->total];

        // Apskaièiuojame h ir alpha reikðmes
        for (int i = 0; i < rows[Row]->total - 1; i++) {
            h[i] = rows[Row]->angles[i + 1] - rows[Row]->angles[i];
        }
        for (int i = 1; i < rows[Row]->total - 1; i++) {
            alpha[i] = (3.0f / h[i]) * ((float)rows[Row]->data[i + 1 + rows[Row]->start] - (float)rows[Row]->data[i + rows[Row]->start])
                     - (3.0f / h[i - 1]) * ((float)rows[Row]->data[i + rows[Row]->start] - (float)rows[Row]->data[i - 1 + rows[Row]->start]);
        }

        // Inicializuojame l, mu, z reikðmes
        l[0] = 1.0f;
        mu[0] = 0.0f;
        z[0] = 0.0f;

        // Apskaièiuojame l, mu, z reikðmes iteratyviai
        for (int i = 1; i < rows[Row]->total - 1; i++) {
            l[i] = 2.0f * (rows[Row]->angles[i + 1] - rows[Row]->angles[i - 1]) - h[i - 1] * mu[i - 1];
            mu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
        }

        l[rows[Row]->total - 1] = 1.0f;
        z[rows[Row]->total - 1] = 0.0f;
        c[rows[Row]->total - 1] = 0.0f;

        // Atgalinës iteracijos bûdu apskaièiuojame c, b, d koeficientus
        for (int j = rows[Row]->total - 2; j >= 0; j--) {
            c[j] = z[j] - mu[j] * c[j + 1];
            b[j] = ((float)rows[Row]->data[j + 1 + rows[Row]->start] - (float)rows[Row]->data[j + rows[Row]->start]) / h[j]
                 - h[j] * (c[j + 1] + 2.0f * c[j]) / 3.0f;
            d[j] = (c[j + 1] - c[j]) / (3.0f * h[j]);
            a[j] = (float)rows[Row]->data[j + rows[Row]->start];  // Konvertuojame uint16_t á float
        }

        // Surandame maksimalø interpoliuotà reikðmæ ir kampà
        float max_value = 0;
        uint16_t max_angle = 0;

        for (float angle = 0.0f; angle <= rows[Row]->angles[rows[Row]->total - 1]; angle += 0.1f) {
            int i;
            for (i = rows[Row]->total - 2; i >= 0; i--) {
                if (angle >= rows[Row]->angles[i]) {
                    break;
                }
            }

            float dx = angle - rows[Row]->angles[i];
            float interpolated_value;

            // Jei angle == 360.0, naudojame pirmàjá `data` masyvo elementà
            if (angle == 360.0f) {
                interpolated_value = a[0] + b[0] * dx + c[0] * dx * dx + d[0] * dx * dx * dx;
            } else {
                interpolated_value = a[i] + b[i] * dx + c[i] * dx * dx + d[i] * dx * dx * dx;
            }

            if (interpolated_value > max_value) {
                max_value = interpolated_value;
                max_angle = (uint16_t)roundf(angle);
            }
        }

        if (Row != 4)
            sensor_top_of_rows[Row] = (int16_t)max_value;
        else
            ELEVATION.total_average_angle = max_angle;
        
        rows[Row]->top_angle = max_angle;
    } else {
        sensor_top_of_rows[Row] = (int16_t)sensor_data[rows[Row]->start];
    }
}*/

/*void azel(uint8_t Row) {
	ROW *rows[5] = {&first, &second, &third, &fourth, &fifth};

	// Periodinë interpoliacija objektams first, second, third ir fifth
	if (Row != 3) {
		int total_points = rows[Row]->total + 1; // Papildomas taðkas intervalui iki pirmo elemento
		float a[total_points], b[total_points], c[total_points], d[total_points];
		float h[total_points - 1], alpha[total_points - 1], l[total_points], mu[total_points], z[total_points];

		// Apskaièiuojame h reikðmes, átraukdami intervalà tarp paskutinio ir pirmo kampo
		for (int i = 0; i < rows[Row]->total - 1; i++) {
			h[i] = rows[Row]->angles[i + 1] - rows[Row]->angles[i];
		}
		// Papildomas intervalas tarp paskutinës ir pirmos reikðmës
		h[rows[Row]->total - 1] = 360.0f - rows[Row]->angles[rows[Row]->total - 1] + rows[Row]->angles[0];

		// Apskaièiuojame alpha reikðmes, átraukdami paskutinës ir pirmos reikðmës skirtumà
		for (int i = 1; i < rows[Row]->total - 1; i++) {
			alpha[i] = (3.0f / h[i]) * ((float)rows[Row]->data[i + 1 + rows[Row]->start] - (float)rows[Row]->data[i + rows[Row]->start])
			- (3.0f / h[i - 1]) * ((float)rows[Row]->data[i + rows[Row]->start] - (float)rows[Row]->data[i - 1 + rows[Row]->start]);
		}
		// Papildoma alpha reikðmë tarp paskutinës ir pirmos reikðmës
		alpha[rows[Row]->total - 1] = (3.0f / h[rows[Row]->total - 1]) *
		((float)rows[Row]->data[rows[Row]->start] - (float)rows[Row]->data[rows[Row]->total - 1 + rows[Row]->start])
		- (3.0f / h[rows[Row]->total - 2]) * ((float)rows[Row]->data[rows[Row]->total - 1 + rows[Row]->start] - (float)rows[Row]->data[rows[Row]->total - 2 + rows[Row]->start]);

		// Inicializuojame l, mu, z reikðmes
		l[0] = 1.0f;
		mu[0] = 0.0f;
		z[0] = 0.0f;

		// Apskaièiuojame l, mu, z reikðmes iteratyviai
		for (int i = 1; i < rows[Row]->total - 1; i++) {
			l[i] = 2.0f * (rows[Row]->angles[i + 1] - rows[Row]->angles[i - 1]) - h[i - 1] * mu[i - 1];
			mu[i] = h[i] / l[i];
			z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
		}
		// Uþdarome ratà su paskutinës ir pirmos reikðmiø perëjimu
		l[rows[Row]->total - 1] = 2.0f * (360.0f - rows[Row]->angles[rows[Row]->total - 2]) - h[rows[Row]->total - 2] * mu[rows[Row]->total - 2];
		z[rows[Row]->total - 1] = (alpha[rows[Row]->total - 1] - h[rows[Row]->total - 2] * z[rows[Row]->total - 2]) / l[rows[Row]->total - 1];

		l[rows[Row]->total] = 1.0f;
		z[rows[Row]->total] = 0.0f;
		c[rows[Row]->total] = 0.0f;

		// Atgalinës iteracijos bûdu apskaièiuojame c, b, d koeficientus
		for (int j = rows[Row]->total - 1; j >= 0; j--) {
			c[j] = z[j] - mu[j] * c[j + 1];
			b[j] = ((float)rows[Row]->data[j + 1 + rows[Row]->start] - (float)rows[Row]->data[j + rows[Row]->start]) / h[j]
			- h[j] * (c[j + 1] + 2.0f * c[j]) / 3.0f;
			d[j] = (c[j + 1] - c[j]) / (3.0f * h[j]);
			a[j] = (float)rows[Row]->data[j + rows[Row]->start];
		}

		// Surandame maksimalø interpoliuotà reikðmæ ir kampà
		float max_value = 0;
		float max_angle = 0;

		for (float angle = 0.0f; angle <= 360.0f; angle += 0.1f) {
			int i;
			for (i = rows[Row]->total - 1; i >= 0; i--) {
				if (angle >= rows[Row]->angles[i] || (i == rows[Row]->total - 1 && angle == 360.0f)) {
					break;
				}
			}

			float dx = angle - rows[Row]->angles[i];
			float interpolated_value = a[i] + b[i] * dx + c[i] * dx * dx + d[i] * dx * dx * dx;

			if (interpolated_value > max_value) {
				max_value = interpolated_value;
				max_angle = angle;
			}
		}

		if (Row != 4) {
			sensor_top_of_rows[Row] = (int16_t)max_value;
			} else {
			ELEVATION.total_average_angle = max_angle;
		}
		rows[Row]->top_angle = max_angle;
	}
	else {
		sensor_top_of_rows[Row] = (int16_t)sensor_data[rows[Row]->start];
	}
}*/

void azel(uint8_t Row) {
	ROW *rows[5] = {&first, &second, &third, &fourth, &fifth};

	// Periodinë interpoliacija objektams first, second, third ir fifth
	if (Row != 3) {
		int total_points = rows[Row]->total + 1; // Papildomas taðkas intervalui iki pirmo elemento
		float a[total_points], b[total_points], c[total_points], d[total_points];
		float h[total_points - 1], alpha[total_points - 1], l[total_points], mu[total_points], z[total_points];

		// Apskaièiuojame h reikðmes, átraukdami intervalà tarp paskutinio ir pirmo kampo
		for (int i = 0; i < rows[Row]->total - 1; i++) {
			h[i] = rows[Row]->angles[i + 1] - rows[Row]->angles[i];
		}

		float range = 360.0;
		if(Row == 4) //elevacijai 0-90 laipsniø
			range = 90.0;

		h[rows[Row]->total - 1] = range - fmodf(fabs(rows[Row]->angles[rows[Row]->total - 1] - rows[Row]->angles[0]), range);

		// Apskaièiuojame alpha reikðmes, átraukdami paskutinës ir pirmos reikðmës skirtumà
		for (int i = 1; i < rows[Row]->total - 1; i++) {
			alpha[i] = (3.0f / h[i]) * ((float)rows[Row]->data[i + 1 + rows[Row]->start] - (float)rows[Row]->data[i + rows[Row]->start])
			- (3.0f / h[i - 1]) * ((float)rows[Row]->data[i + rows[Row]->start] - (float)rows[Row]->data[i - 1 + rows[Row]->start]);
		}
		
		// Papildoma alpha reikðmë tarp paskutinës ir pirmos reikðmës
		alpha[rows[Row]->total - 1] = (3.0f / h[rows[Row]->total - 1]) *
		((float)rows[Row]->data[rows[Row]->start] - (float)rows[Row]->data[rows[Row]->total - 1 + rows[Row]->start])
		- (3.0f / h[rows[Row]->total - 2]) * ((float)rows[Row]->data[rows[Row]->total - 1 + rows[Row]->start] - (float)rows[Row]->data[rows[Row]->total - 2 + rows[Row]->start]);

		// Inicializuojame l, mu, z reikðmes
		l[0] = 1.0f;
		mu[0] = 0.0f;
		z[0] = 0.0f;

		// Apskaièiuojame l, mu, z reikðmes iteratyviai
		for (int i = 1; i < rows[Row]->total - 1; i++) {
			l[i] = 2.0f * (rows[Row]->angles[i + 1] - rows[Row]->angles[i - 1]) - h[i - 1] * mu[i - 1];
			mu[i] = h[i] / l[i];
			z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
		}

		// Uþdarome ratà su paskutinës ir pirmos reikðmës perëjimu
		l[rows[Row]->total - 1] = 2.0f * (range - rows[Row]->angles[rows[Row]->total - 2]) - h[rows[Row]->total - 2] * mu[rows[Row]->total - 2];
		z[rows[Row]->total - 1] = (alpha[rows[Row]->total - 1] - h[rows[Row]->total - 2] * z[rows[Row]->total - 2]) / l[rows[Row]->total - 1];

		l[rows[Row]->total] = 1.0f;
		z[rows[Row]->total] = 0.0f;
		c[rows[Row]->total] = 0.0f;

		// Atgalinës iteracijos bûdu apskaièiuojame c, b, d koeficientus
		for (int j = rows[Row]->total - 1; j >= 0; j--) {
			c[j] = z[j] - mu[j] * c[j + 1];
			b[j] = ((float)rows[Row]->data[j + 1 + rows[Row]->start] - (float)rows[Row]->data[j + rows[Row]->start]) / h[j]
			- h[j] * (c[j + 1] + 2.0f * c[j]) / 3.0f;
			d[j] = (c[j + 1] - c[j]) / (3.0f * h[j]);
			a[j] = (float)rows[Row]->data[j + rows[Row]->start];
		}

		// Surandame maksimalø interpoliuotà reikðmæ ir kampà
		float max_value = 0;
		float max_angle = 0;

		for (float angle = 0.0f; angle <= range; angle += 0.1f) {
			int i;
			for (i = rows[Row]->total - 1; i >= 0; i--) {
				if (angle >= rows[Row]->angles[i] || (i == rows[Row]->total - 1 && angle == range)) {
					break;
				}
			}

			float dx = angle - rows[Row]->angles[i];
			float interpolated_value = a[i] + b[i] * dx + c[i] * dx * dx + d[i] * dx * dx * dx;

			if (interpolated_value > max_value) {
				max_value = interpolated_value;
				max_angle = angle;
			}
		}

		if (Row != 4) {
			sensor_top_of_rows[Row] = (int16_t)max_value;
			} else {
			ELEVATION.total_average_angle = max_angle;
		}

		rows[Row]->top_angle = max_angle;
	}
	else {
		// Jei Row = 4 (elevacija), tiesiog naudojame pirmà sensorá kaip topo reikðmæ
		sensor_top_of_rows[Row] = (int16_t)sensor_data[rows[Row]->start];
	}
}




