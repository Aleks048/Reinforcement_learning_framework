#pragma once

#include <vector>
#include "ClassificationCount.h"


class Environment
{
public:
	Environment();
	~Environment();

	void addState(bool(*function)(std::string pathName, std::map<std::string, double>, std::vector<birthDeath> birthsDeaths),
		std::map<std::string, double> parameters);
	/*
	struct Image{
		struct Distance
		{
		public:
			int start, end;
		//public:
			Distance(int start,int end);
		};
		
		Image(std::string name);

		std::string name;

		std::vector<Distance> distances;	
	};

	
	struct State {
		int lengthNum;
		int	quantityOfLengths;

		int calculatePosReward();
		std::vector<State> neighbours;
	};

	void formStatesGrid() {
	}

	
	std::vector<State> states;

	std::vector<std::vector<std::vector<int>>> originalClassification;
	*/
private:
	std::vector<state> states;
};

