#include "Debug.h"

Debug::Debug()
{
	output_file.open("debug.txt", std::ofstream::out | std::ofstream::app);
}

Debug::~Debug()
{
	output_file.close();
}

void Debug::addDebugMessage(const char* message)
{
	output_file << message;
	output_file << "\n\n";
}
