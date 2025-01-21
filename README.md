# Project Terminated. 
## Reason: 
**The position of the Sun in the sky doesn't change :D.**
This program was written before I started using GitHub, so it will be impossible to restore the last working version (all attempts to comment out parts of the code—mainly in the elevation.c and azimuth.c sections—were unsuccessful).

This Sun sensor was designed right after the successful trial of the "Simple Attiny1624 Sun Position Sensor." 
https://youtu.be/7Qga30PvxM4
https://github.com/saulius1005/Attiny1624_SUN.git

Naturally, I immediately planned a "more perfect" version and, without testing anything, designed the PCB and ordered it to be manufactured. Once I received it, I soldered it right away and rushed into firmware development.

And that's where the problems began:

The resistance of the photoresistors changes disproportionately to sunlight, and their resistance varies non-linearly.
The resistance of the photoresistors differs by ~20% from one to another, even though they are all supposed to be 5 kΩ.
Then I decided to replace the photoresistors with photodiodes.
After numerous experiments and modifications, the same story repeated itself: the photodiode in photovoltaic mode also did not respond linearly. Finally, hope emerged with the photoconductive mode, which seemed to change relatively proportionally. However, after extensive analysis and testing, I concluded that the calculated calibration coefficients for normalizing the values, which should have worked across the entire range, did not actually work universally—especially for values near 0. Moreover, my favorite microcontrollers are constrained by IEEE-754 limitations...

Theoretically, sunlight around the sensor should graphically resemble a sinusoid in terms of azimuth and an arc in terms of elevation. The peaks would correspond to the azimuth and elevation angles. Of course, for intermediate values, I initially used sinusoidal interpolation, then Lagrange, spline, and every other method I could think of. The result was always the same: azimuth calculations were relatively decent since they were determined based on data from three rows of sensors: 15 in the first row, 12 in the second, 4 in the third, and 1 in the fourth. Naturally, the interpolation factored in the weight of each row in the overall result.

However, with elevation, it was a different story. Since elevation was calculated based on the interpolated maxima of three rows and the data from the fourth row sensor, the calculated values were completely unrepresentative of the real ones.
Until one day, the thought crossed my mind: The position of the Sun relative to the Earth doesn’t change... It has moved, moves, and will continue to move the same way for thousands of years.
And only after that, this option was chosen:
https://youtu.be/cBCFJO9GvYM
https://github.com/saulius1005/Attiny212-clock.git 


## The difficult path of knowledge:
Last photos of the sensor (with callibration ruller):
![20250121_004620](https://github.com/user-attachments/assets/abd63a0b-1639-4b5b-bb35-5cc4cbeb81de)
![20250121_004641](https://github.com/user-attachments/assets/ff21480b-6ec3-44bd-8ba7-43cfcd1bd032)
![20250121_004718](https://github.com/user-attachments/assets/1eb3f60d-de32-42ed-ac6a-3fdf6c552470)
![20250121_004816](https://github.com/user-attachments/assets/539977ae-721e-4af8-b9be-f1b7b17c39c3)

## History of the sensors array boards:
![20250121_005941](https://github.com/user-attachments/assets/5d71d97c-f19f-457f-96e7-601d026c6ec4)

## Photodiode and photoresistor tester stand:
![20241031_204205](https://github.com/user-attachments/assets/dd5e1c7a-f2db-4b52-a67b-d3b258cdbfe7)
![Ekrano kopija 2025-01-21 011249](https://github.com/user-attachments/assets/f80c25ff-337c-423b-959d-d7cc374680d6)

## Also, the differences between the sensors under the same lighting conditions:
![Ekrano kopija 2025-01-21 011320](https://github.com/user-attachments/assets/5c03c7ba-3f1b-4151-a1b4-dd1b0e175fd0)

## And finally, the differences in interpolations with slight changes in light... Of course, normalization doesn’t work for all cases...
![Ekrano kopija 2025-01-21 011428](https://github.com/user-attachments/assets/e3451865-7eb2-4458-acb3-cbd1e647ad92)


