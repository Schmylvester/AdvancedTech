#pragma once
#include <fstream>

class Debug
{
public:
	Debug();
	~Debug();

	void addDebugMessage(const char* message);
private:
	std::ofstream output_file;
};