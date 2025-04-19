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
		ERROR("Usage: %s <path to device>\n", basename(argv[0]));
		return EXIT_FAILURE;
	}

	if (!erg.init(string(argv[1])))
	{
		ERROR("Failed to initialise erg module\n");
		return EXIT_FAILURE;
	}

	// Test stub...
	unsigned char status;
	float distance;
	erg.getDistanceData(ERG_NUM, status, distance);

	DEBUG("status=%x, distance=%f\n", status, distance);
	//Rationale? DEBUG("Erg status=%s\n", erg.getStatusStr(ERG_NUM).c_str());

	return EXIT_SUCCESS;
}

