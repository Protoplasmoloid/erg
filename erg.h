/*
 * erg.h
 *
 *  Created on: Apr 16, 2025
 *      Author: Proto
 */

#ifndef ERG_H_
#define ERG_H_

#include <string>
#include <iostream>
using namespace std;

#include <string.h>
#include <unistd.h>
#include <fcntl.h>

const unsigned char IS_END_OF_WORKOUT = 0x01; // If set, end of workout has been reached
const unsigned char IS_END_OF_STROKE = 0x02; // If set, end of stroke has been reached
const unsigned char IS_WORK_DISTANCE = 0x04; // If set, erg is set for a distance workout
const unsigned char IS_WORK_TIME = 0x08; // If set, erg is set for a time workout
const unsigned char IS_LOW_BATTERY = 0x40; // If set, get a new battery
const unsigned char GET_DISTANCE_DATA = 0xb0; // Status (uchar), distance in meters (float).
const unsigned char GET_PACE_DATA = 0xb1; // Stroke rate (uchar) in strokes/min and secs/meter (float)
const unsigned char GET_HEART_PERIOD = 0xb2; // Heart period (ushort. Heart rate = 576,000/heart period.
const unsigned char GET_ELAPSED_TIME = 0xb3; // Status (uchar), elapsed time (float) in secs.
const unsigned char GET_UNKNOWN_B4 = 0xb4;   // ? Four uchar's
const unsigned char GET_UNKNOWN_B5 = 0xb5;   // ? Four uchar's
const unsigned char GET_UNKNOWN_B6 = 0xb6;   // ?
const unsigned char GET_UNKNOWN_B7 = 0xb7;   // ?
const unsigned char GET_UNKNOWN_B8 = 0xb8;   // ?
const unsigned char GET_UNKNOWN_B9 = 0xb9;   // ?
const unsigned char NTX_COMMAND = 2;
const unsigned char NRX_DISTANCE_DATA = 5;
const unsigned char NRX_PACE_DATA = 5;
const unsigned char NRX_HEART_PERIOD = 2;
const unsigned char NRX_ELAPSED_TIME = 5;
const unsigned char NRX_UNKNOWN_B4 = 4;
const unsigned char NRX_UNKNOWN_B5 = 4;
const unsigned char NRX_UNKNOWN_B6 = 6;
const unsigned char NRX_UNKNOWN_B7 = 4;
const unsigned char NRX_UNKNOWN_B8 = 4;
const unsigned char NRX_UNKNOWN_B9 = 4;

#define ieee_float(x)*(float*)&x // Return IEEE754 float (32 bit)

class erg
{
private:
	int fd; // Serial device descriptor

public:
	erg();
	virtual ~erg();

	bool isEndOfWorkout(unsigned char);
	bool isEndOfStroke(unsigned char);
	bool isWorkDistance(unsigned char);
	bool isWorkTime(unsigned char);
	bool isLowBattery(unsigned char);
	bool getDistanceData(unsigned char, unsigned char&, float&);
	bool getPaceData(unsigned char, unsigned char&, float&);
	bool getHeartPeriod(unsigned char, float&);
	bool getElapsedTime(unsigned char, unsigned char&, float&);
	bool getUnknownB4(unsigned char);
	bool getUnknownB5(unsigned char);
	bool getUnknownB6(unsigned char);
	bool getUnknownB7(unsigned char);
	bool getUnknownB8(unsigned char);
	bool getUnknownB9(unsigned char);

	bool init(string);
	string getStatusStr(int ergnum);
	bool getSerial(unsigned char*, unsigned char*, int, int);
	string byteToBinStr(unsigned char b);

};

#endif /* ERG_H_ */
