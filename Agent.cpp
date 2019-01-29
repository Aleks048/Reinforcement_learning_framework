#include "stdafx.h"
#include "Agent.h"
//#include "ClassificationCount.h"


//Agent 

//public
Agent::Agent(state currentState,vector<std::string>classes, int numOfImagesPerClass) :currentState(currentState),classes(classes), numOfImagesPerClass(numOfImagesPerClass)//need the starting state initialization
{}

Agent::~Agent()
{
}

state Agent::step() {

	return state{};
}

//calculate score in a particular state
int Agent::calculateScore(state* st) {
	int score = 0;
	for (int j = 0; j < classes.size(); ++j) {
		for (int i = 0; i < numOfImagesPerClass; ++i) {
			std::string temp = classifyTheImage("C:\\Users\\ytr16\\source\\repos\\generating_images\\generating_images\\images\\"+classes[j]+"\\supportTextFiles\\", "Vsimpl_"+classes[j]+"_"+to_string(i)+".txt_1",
												st);
			
			if (temp == classes[j]) {
				score++;
			}
			//std::cout << score;
		}
	}
	return score;
}

int Agent::getReward(state* st, action* ac) {
	
	state* next{};
	switch (ac->t)
	{
	case(action::type::parameters): next = &ac->changeParameter(st);
									break;
	case(action::type::function):	next = &ac->changeFunction(st);
									break;
	default: next = st;
			break;
	}
	return calculateScore(next);
	
}

//private

state Agent::selectNextState() {
	return state{};
}