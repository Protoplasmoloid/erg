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
	fd = -1;
}

erg::~erg()
{
	if (fd != -1)
		close(fd);
}

bool erg::init(string dev)
{
	//DEBUG("dev=%s", dev.c_str());

	if (-1 == (fd = open(dev.c_str(), O_RDWR)))
	{
		ERROR("Failed to open %s: %s", dev.c_str(), strerror(errno));
		return false;
	}

	//DEBUG("fd=%d", fd);

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

	//DEBUG("Sent %lu bytes, want %lu bytes", nTxd, nExp);

	//usleep(10000); // Sanity check for read speed

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
			//DEBUG("Got %lu bytes", nRxd);
		}
		else
		{
			ERROR("Timeout waiting for data");
			break;
		}
	}

	printf("Received: ");
	for (int i = 0; i < nrx; i++)
		printf("0x%02x ", rxbuff[i]);
	printf("\n");
	fflush(stdout);

	return retval == -1 ? false : true;
}

// 0xb0, status (uchar), distance in meters (float).
// Expects to read 5 bytes
// 0xc0 0x00 0x00 0x00 0x00 status=c0, distance=0.000000
// 0xc2 0x67 0x8f 0x0d 0x41 status=c2, distance=8.847510
// 0x2d 0x2a 0x27 0x9d 0x41 status=2d, distance=19.644123
bool erg::getDistanceData(unsigned char ergnum, unsigned char &status,
		float &distance)
{
	unsigned char sendBuff[NTX_COMMAND] =
		{ ergnum, GET_DISTANCE_DATA };
	unsigned char recvBuff[NRX_DISTANCE_DATA];

	if (getSerial(sendBuff, recvBuff, NTX_COMMAND, NRX_DISTANCE_DATA))
	{
		status = recvBuff[0];
		distance = ieee_float(recvBuff[1]);
		return true;
	}
	else
	{
		ERROR("Failed to get serial data");
		return false;
	}
}

// 0xb1, stroke rate (uchar) in strokes/min and secs/meter (float)
// Expects to read 5 bytes
// 6c 5c 58 fe 00 3f
// 0x02 0xaa 0xf2 0x09 0x3f status=2, pace=0.538859
bool erg::getPaceData(unsigned char ergnum, unsigned char &status, float &pace)
{
	unsigned char sendBuff[NTX_COMMAND] =
		{ ergnum, GET_PACE_DATA };
	unsigned char recvBuff[NRX_PACE_DATA];

	if (getSerial(sendBuff, recvBuff, NTX_COMMAND, NRX_PACE_DATA))
	{
		status = recvBuff[0];
		pace = ieee_float(recvBuff[1]);
		return true;
	}
	else
	{
		ERROR("Failed to get serial data");
		return false;
	}
}

// 0xb2, heart period (ushort). Note: heart rate = 576,000/heart period
// Expects to read 2 bytes
// 00 00
bool erg::getHeartPeriod(unsigned char ergnum, float &period)
{
	unsigned char sendBuff[NTX_COMMAND] =
		{ ergnum, GET_HEART_PERIOD };
	unsigned char recvBuff[NRX_HEART_PERIOD];

	if (getSerial(sendBuff, recvBuff, NTX_COMMAND, NRX_HEART_PERIOD))
	{
		period = -1; //(float((short *)recvBuff); // FIXME
		return true;
	}
	else
	{
		ERROR("Failed to get serial data");
		return false;
	}
}

// 0xb3, status (uchar), elapsed time (float) in secs.
// Expects to read 5 bytes
// 83 d0 00 e8 00 41
// 0xc0 0x8a 0x94 0x40 0x41 status=c0, time=12.036264
bool erg::getElapsedTime(unsigned char ergnum, unsigned char &status,
		float &time)
{
	unsigned char sendBuff[NTX_COMMAND] =
		{ ergnum, GET_ELAPSED_TIME };
	unsigned char recvBuff[NRX_ELAPSED_TIME];

	if (getSerial(sendBuff, recvBuff, NTX_COMMAND, NRX_ELAPSED_TIME))
	{
		status = recvBuff[0];
		time = ieee_float(recvBuff[1]);
		return true;
	}
	else
	{
		ERROR("Failed to get serial data");
		return false;
	}
}

// 0xb4
// Expects to read 4 bytes
// ae 3b 00 8d
bool erg::getUnknownB4(unsigned char ergnum)
{
	unsigned char sendBuff[NTX_COMMAND] =
		{ ergnum, GET_UNKNOWN_B4 };
	unsigned char recvBuff[NRX_UNKNOWN_B4];

	if (getSerial(sendBuff, recvBuff, NTX_COMMAND, NRX_UNKNOWN_B4))
	{
		for (int i = 0; i < NRX_UNKNOWN_B4; ++i)
			cout << byteToBinStr(recvBuff[i]) << " ";
		flush(cout);
		return true;
	}
	else
	{
		ERROR("Failed to get serial data");
		return false;
	}
}

// 0xb5
// Expects to read 4 bytes
// 00 63 04 02
bool erg::getUnknownB5(unsigned char ergnum)
{
	unsigned char sendBuff[NTX_COMMAND] =
		{ ergnum, GET_UNKNOWN_B5 };
	unsigned char recvBuff[NRX_UNKNOWN_B5];

	if (getSerial(sendBuff, recvBuff, NTX_COMMAND, NRX_UNKNOWN_B5))
	{
		for (int i = 0; i < NRX_UNKNOWN_B5; ++i)
			cout << byteToBinStr(recvBuff[i]) << " ";
		flush(cout);
		return true;
	}
	else
	{
		ERROR("Failed to get serial data");
		return false;
	}
}

// 0xb6
// Expects to read 6 bytes
// 00 00 00 00 00 00
bool erg::getUnknownB6(unsigned char ergnum)
{
	unsigned char sendBuff[NTX_COMMAND] =
		{ ergnum, GET_UNKNOWN_B6 };
	unsigned char recvBuff[NRX_UNKNOWN_B6];

	if (getSerial(sendBuff, recvBuff, NTX_COMMAND, NRX_UNKNOWN_B6))
	{
		for (int i = 0; i < NRX_UNKNOWN_B6; ++i)
			cout << byteToBinStr(recvBuff[i]) << " ";
		flush(cout);
		return true;
	}
	else
	{
		ERROR("Failed to get serial data");
		return false;
	}
}

// 0xb7
// Expects to read 4 bytes
// ff ff ff ff
bool erg::getUnknownB7(unsigned char ergnum)
{
	unsigned char sendBuff[NTX_COMMAND] =
		{ ergnum, GET_UNKNOWN_B7 };
	unsigned char recvBuff[NRX_UNKNOWN_B7];

	if (getSerial(sendBuff, recvBuff, NTX_COMMAND, NRX_UNKNOWN_B7))
	{
		for (int i = 0; i < NRX_UNKNOWN_B7; ++i)
			cout << byteToBinStr(recvBuff[i]) << " ";
		flush(cout);
		return true;
	}
	else
	{
		ERROR("Failed to get serial data");
		return false;
	}
}

// 0xb8
// Expects to read 4 bytes
// ff ff ff ff
bool erg::getUnknownB8(unsigned char ergnum)
{
	unsigned char sendBuff[NTX_COMMAND] =
		{ ergnum, GET_UNKNOWN_B8 };
	unsigned char recvBuff[NRX_UNKNOWN_B8];

	if (getSerial(sendBuff, recvBuff, NTX_COMMAND, NRX_UNKNOWN_B8))
	{
		for (int i = 0; i < NRX_UNKNOWN_B8; ++i)
			cout << byteToBinStr(recvBuff[i]) << " ";
		flush(cout);
		return true;
	}
	else
	{
		ERROR("Failed to get serial data");
		return false;
	}
}

// 0xb9
// Expects to read 4 bytes
// ff ff ff ff
bool erg::getUnknownB9(unsigned char ergnum)
{
	unsigned char sendBuff[NTX_COMMAND] =
		{ ergnum, GET_UNKNOWN_B9 };
	unsigned char recvBuff[NRX_UNKNOWN_B9];

	if (getSerial(sendBuff, recvBuff, NTX_COMMAND, NRX_UNKNOWN_B9))
	{
		for (int i = 0; i < NRX_UNKNOWN_B9; ++i)
			cout << byteToBinStr(recvBuff[i]) << " ";
		flush(cout);
		return true;
	}
	else
	{
		ERROR("Failed to get serial data");
		return false;
	}
}

//
bool erg::isEndOfWorkout(unsigned char b)
{
	return (b & IS_END_OF_WORKOUT) == IS_END_OF_WORKOUT ? true : false;
}

//
bool erg::isEndOfStroke(unsigned char b)
{
	return (b & IS_END_OF_STROKE) == IS_END_OF_STROKE ? true : false;
}

//
bool erg::isWorkDistance(unsigned char b)
{
	return (b & IS_WORK_DISTANCE) == IS_WORK_DISTANCE ? true : false;
}

//
bool erg::isWorkTime(unsigned char b)
{
	return (b & IS_WORK_TIME) == IS_WORK_TIME ? true : false;
}

//
bool erg::isLowBattery(unsigned char b)
{
	return (b & IS_LOW_BATTERY) == IS_LOW_BATTERY ? true : false;
}

// Get the status as a string
string erg::getStatusStr(int ergnum)
{
	string str;
	unsigned char status;
	float distance; // Not used, we just want status

	// Distance request returns status
	//getDistanceData(ergnum, status, distance);

	DEBUG("status=%04x, distance=%f", status, distance);

	if ((status & IS_END_OF_WORKOUT) == IS_END_OF_WORKOUT)
		str += "EndOfWorkout ";
	if ((status & IS_END_OF_STROKE) == IS_END_OF_STROKE)
		str += "EndOfStroke ";
	if ((status & IS_WORK_DISTANCE) == IS_WORK_DISTANCE)
		str += "WorkDistance ";
	if ((status & IS_WORK_TIME) == IS_WORK_TIME)
		str += "WorkTime ";
	if ((status & IS_LOW_BATTERY) == IS_LOW_BATTERY)
		str += "LowBattery ";

	return str;
}

//
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

