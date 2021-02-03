#pragma once

#include <string>

using namespace std;

class Debug // for writing results to console
{
	string name; // name of the Debug object

public:
	Debug(string name); // constructor
	void log(string text) const; // write text to console
	void logLine(string line = "") const; // write text to console and go to new line
};