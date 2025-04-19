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

#define END_OF_WORKOUT    0x01   // If set, end of workout has been reached
#define END_OF_STROKE     0x02   // If set, end of stroke has been reached
#define WORK_DISTANCE     0x04   // If set, erg is set for a distance workout
#define WORK_TIME         0x08   // If set, erg is set for a time workout
#define LOW_BATTERY       0x40   // If set, get a new battery
#define GET_DISTANCE_DATA 0xb0   // Status (uchar), distance in meters (float).
#define GET_PACE_DATA     0xb1   // Stroke rate (uchar) in strokes/min and secs/meter (float)
#define GET_HEART_PERIOD  0xb2   // Heart period (ushort. Heart rate = 576,000/heart period.
#define GET_ELAPSED_TIME  0xb3   // Status (uchar), elapsed time (float) in secs.
#define GET_UNKNOWN_1     0xb4   // ? Four uchar's
#define GET_UNKNOWN_2     0xb5   // ? Four uchar's
#define GET_UNKNOWN_3     0xb6   // ? Just zeros
#define GET_UNKNOWN_4     0xb7   // ? Bytes, need to check again

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
	bool getDistanceData(int ergnum, unsigned char& status, float& distance);
	bool getPaceData(FILE*, unsigned char, string&, string&);
	bool getHeartPeriod(FILE*, unsigned char, string&, string&);
	bool getElapsedTime(FILE*, unsigned char, string&, string&);
	bool getUnknown1(FILE*, unsigned char, string&, string&);
	bool getUnknown2(FILE*, unsigned char, string&, string&);
	bool getUnknown3(FILE*, unsigned char, string&, string&);
	bool getUnknown4(FILE*, unsigned char, string&, string&);

	bool init(string);
	string getStatusStr(int ergnum);
	bool getSerial(unsigned char*, unsigned char*, int, int);
	string byteToBinStr(unsigned char b);

};

#endif /* ERG_H_ */
