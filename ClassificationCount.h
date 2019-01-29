#pragma once
#include <string>
#include <map>
#include <vector>

//BIRTH DEATH
struct birthDeath {
	int birth;
	int death;
	birthDeath(int birth, int death);

};
//STATE ACTION
struct state {
	

	//constructors
	state();
	state(bool(*function)(std::string pathName, std::map<std::string, double>, std::vector<birthDeath> birthsDeaths),
		std::map<std::string, double> parameters);

	//functions
	bool(*function)(std::string pathName,std::map<std::string, double>,std::vector<birthDeath> birthsDeaths);
	std::map<std::string, double> parameters;
};
struct action {
	enum type {parameters,function};
	//constructor
	action(enum type t);
	type t;
	//rest
	std::string type;
	state (*changeParameter)(state* state);
	state(*changeFunction)(state* state);
};


//CLASSIFICATION IMAGE 

std::vector<birthDeath> getDataFromTextFile(std::string path, std::string filename);

std::string classifyTheImage(std::string path,std::string filename,state* st);

bool simpleTresholdPrediction(std::string pathName, std::map<std::string, double> param, std::vector<birthDeath> birthsDeaths);

