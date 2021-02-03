#include "Debug.h"

#include <iostream>

Debug::Debug(string name)
:name(name)
{
}

void Debug::log(string text) const
{
	cout << "[" << name << "]: " << text;
}

void Debug::logLine(string line) const
{
	log(line + '\n');
}
