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
	// FIXME Need a more type agnostic way of presenting this data
	// string status, str;
	DEBUG("Start of test stubs:");
	unsigned char status;

	// FIXME The PM2+ goes into sleep mode and resets to zero after a few seconds
	// FIXME After sleep we get timeouts for first reads, after that its all ok (err most of the time!)
	// FIXME PM2+ wakes up after first poke, then we get timeouts
	float distance;
	erg.getDistanceData(ERG_NUM, status, distance);
	DEBUG("status=%x, distance=%f", status, distance);

	float time;
	erg.getElapsedTime(ERG_NUM, status, time);
	DEBUG("status=%x, time=%f", status, time);

	float pace;
	erg.getPaceData(ERG_NUM, status, pace);
	DEBUG("status=%x, pace=%f", status, pace);


	DEBUG("End of test stubs:");

	return EXIT_SUCCESS;
}

