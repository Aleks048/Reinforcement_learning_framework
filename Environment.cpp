#include "stdafx.h"
#include "Environment.h"




Environment::Environment()
{
}

Environment::~Environment()
{
}

void Environment::addState(bool(*function)(std::string pathName, std::map<std::string, double>, std::vector<birthDeath>birthsDeaths), std::map<std::string, double> parameters)
{
	states.push_back(state{function,parameters});
}
