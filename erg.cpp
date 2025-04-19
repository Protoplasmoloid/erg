/*
 * erg.cpp
 *
 *  Created on: Apr 16, 2025
 *      Author: Proto
 */

#include "erg.h"
#include "trace.h"

erg::erg()
{
	DEBUG("In constructor\n");

	fd = -1;
}

erg::~erg()
{
	DEBUG("In destructor\n");

	if (fd != -1)
		close(fd);
}

bool erg::init(string dev)
{
	DEBUG("dev=%s\n", dev.c_str());

	if (-1 == (fd = open(dev.c_str(), O_RDWR)))
	{
		ERROR("Failed to open %s: %s\n", dev.c_str(),
				strerror(errno));
		return false;
	}

	DEBUG("fd=%d\n", fd);

	return true;
}

bool erg::getSerial(unsigned char *txbuff, unsigned char *rxbuff, int ntx,
		int nrx)
{
	// Assume sending the data will always work!
	ssize_t nTxd = write(fd, txbuff, ntx);
	ssize_t nRxd = 0;
	ssize_t nExp = nrx;
	unsigned char *pBuff = rxbuff;

	INFO("Sent %lu bytes, want %lu bytes\n", nTxd, nExp);

	int retval = -1;
	fd_set rfds;
	struct timeval tv;

	// Get the expected number of bytes
	while (0 != nExp)
	{
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);

		// Wait for up to one second
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		retval = select(fd + 1, &rfds, NULL, NULL, &tv);

		if (retval == -1)
		{
			ERROR("Select failed: %s", strerror(errno));
			break;
		}
		else if (retval)
		{
			nRxd = read(fd, pBuff, nExp); // FD_ISSET(fd, &rfds) will be true
			nExp -= nRxd;
			pBuff += nRxd;
			INFO("Got %lu bytes\n", nRxd);
		}
		else
		{
			ERROR("Timeout waiting for data\n");
			break;
		}
	}

	printf("Received: ");
	for (int i = 0; i < nrx; i++)
	{
		printf("0x%02x ", rxbuff[i]);
		fflush(stdout);
	}
	printf("\n");

	return retval == -1 ? false : true;
}

// Expects to read 5 bytes
// 0xb0, status (uchar), distance in meters (float).
bool erg::getDistanceData(int ergnum, unsigned char& status, float& distance)
{
#warning "For testing: num sent = num received = 5 with loopback"
	int ntx = 5;
	int nrx = 5;

	unsigned char sendBuff[ntx] =
		{ 0x31, 0x32, 0x33, 0x34, 0x35 };
	unsigned char recvBuff[nrx];

	if (getSerial(sendBuff, recvBuff, ntx, nrx))
	{
		status = recvBuff[0];
		distance = ieee_float(recvBuff[1]);
		return true;
	}

	return false;
}

// Expects to read 5 bytes
// 0xb1, stroke rate (uchar) in strokes/min and secs/meter (float)
bool erg::getPaceData(FILE *fp, unsigned char ergnum, string &status,
		string &distance)
{

	return false;
}

// Expects to read 2 bytes
// 0xb2, heart period (ushort. Heart rate = 576,000/heart period.
bool erg::getHeartPeriod(FILE *fp, unsigned char ergnum, string &status,
		string &distance)
{

	return false;
}

// Expects to read 5 bytes
// 0xb3, status (uchar), elapsed time (float) in secs.
bool erg::getElapsedTime(FILE *fp, unsigned char ergnum, string &status,
		string &distance)
{

	return false;
}

// 0xb4
bool erg::getUnknown1(FILE *fp, unsigned char ergnum, string &status,
		string &distance)
{

	return false;
}

// 0xb5
bool erg::getUnknown2(FILE *fp, unsigned char ergnum, string &status,
		string &distance)
{

	return false;
}

// 0xb6
bool erg::getUnknown3(FILE *fp, unsigned char ergnum, string &status,
		string &distance)
{

	return false;
}

// 0xb7
bool erg::getUnknown4(FILE *fp, unsigned char ergnum, string &status,
		string &distance)
{

	return false;
}

bool erg::isEndOfWorkout(unsigned char b)
{
	return (b & END_OF_WORKOUT) == END_OF_WORKOUT ? true : false;
}

bool erg::isEndOfStroke(unsigned char b)
{
	return (b & END_OF_STROKE) == END_OF_STROKE ? true : false;
}

bool erg::isWorkDistance(unsigned char b)
{
	return (b & WORK_DISTANCE) == WORK_DISTANCE ? true : false;
}

bool erg::isWorkTime(unsigned char b)
{
	return (b & WORK_TIME) == WORK_TIME ? true : false;
}

bool erg::isLowBattery(unsigned char b)
{
	return (b & LOW_BATTERY) == LOW_BATTERY ? true : false;
}

// Get the status as a string
string erg::getStatusStr(int ergnum)
{
	string str;
	unsigned char status;
	float distance; // Not used, we just want status

	// Distance request returns status
	getDistanceData(ergnum, status, distance);

	DEBUG("status=%04x, distance=%f\n", status, distance);

	if ((status & END_OF_WORKOUT) == END_OF_WORKOUT)
		str += "EndOfWorkout ";
	if ((status & END_OF_STROKE) == END_OF_STROKE)
		str += "EndOfStroke ";
	if ((status & WORK_DISTANCE) == WORK_DISTANCE)
		str += "WorkDistance ";
	if ((status & WORK_TIME) == WORK_TIME)
		str += "WorkTime ";
	if ((status & LOW_BATTERY) == LOW_BATTERY)
		str += "LowBattery ";

	return str;
}

string erg::byteToBinStr(unsigned char byte)
{
	string str;

	str += ((byte) & 0x80 ? '1' : '0');
	str += ((byte) & 0x40 ? '1' : '0');
	str += ((byte) & 0x20 ? '1' : '0');
	str += ((byte) & 0x10 ? '1' : '0');
	str += ((byte) & 0x08 ? '1' : '0');
	str += ((byte) & 0x04 ? '1' : '0');
	str += ((byte) & 0x02 ? '1' : '0');
	str += ((byte) & 0x01 ? '1' : '0');

	return str;
}

