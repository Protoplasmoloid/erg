// Erg PM2+ test

#include <string>
using namespace std;

#include "erg.h"
#include "trace.h"

#define ERG_NUM 0x00 // There is only one erg

erg erg;

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		INFO("Usage: %s <path to device>", basename(argv[0]));
		return EXIT_FAILURE;
	}

	if (!erg.init(string(argv[1])))
	{
		ERROR("Failed to initialise erg module");
		return EXIT_FAILURE;
	}

	// Test stubs...
	DEBUG("Start of test stubs:");
	unsigned char status;

	// FIXME The PM2+ goes to sleep and resets to zero after a while.
	// FIXME After sleep we get timeouts for first two reads, after that all is well, mostly!

	float distance;
	erg.getDistanceData(ERG_NUM, status, distance);
	DEBUG("getDistanceData: status=0x%02x, distance=%f", status, distance);

	float pace;
	unsigned char rate;
	erg.getPaceData(ERG_NUM, rate, pace);
	DEBUG("getPaceData:     rate=0x%02x, pace=%f", rate, pace);

	float time;
	erg.getElapsedTime(ERG_NUM, status, time);
	DEBUG("getElapsedTime:  status=0x%02x, time=%f", status, time);

//	erg.getHeartPeriod(ERG_NUM); // Always get a timeout...
//	erg.getUnknownB4(ERG_NUM); // Always get a timeoutm but we do get some data.
//	erg.getUnknownB5(ERG_NUM); // Always get a timeout, but we do get some data.
//	erg.getUnknownB6(ERG_NUM); // We get some data.
//	erg.getUnknownB7(ERG_NUM); // Always get a timeout. Peeking causes timeours.
//	erg.getUnknownB8(ERG_NUM); // We get some data.
//	erg.getUnknownB9(ERG_NUM); // We get some data.

	DEBUG("End of test stubs:");

	return EXIT_SUCCESS;
}

