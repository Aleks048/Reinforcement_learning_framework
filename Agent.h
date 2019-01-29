#pragma once

#include <vector>
#include "Environment.h"
#include "ClassificationCount.h"
#include <iostream>
using namespace std;

class Agent
{	

public:

	Agent(state currentState,vector<std::string>classes, int numOfImagesPerClass);
	
	~Agent();
	
	state step();//take a step
	int getReward(state* st,action* ac);


private:
	vector<std::string>classes;
	int numOfImagesPerClass;

	int currentScore;//to keep the score obrained at the moment
	state currentState;//represents ythe current state of the agent in the grid

public://for now
	int calculateScore(state* st);
private:
	state selectNextState();//not implemented
};


