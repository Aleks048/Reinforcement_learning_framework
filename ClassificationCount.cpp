//my headers include
#include "classificationCount.h"
//rest include
#include <iostream>
#include <fstream>

//BIRTH DEATH
//constructor
birthDeath::birthDeath(int birth, int death) :birth{ birth }, death{ death } {}

//STATE ACTION
state::state() {}
state::state(bool(*function)(std::string pathName, std::map<std::string, double>, std::vector<birthDeath> birthsDeaths),
	std::map<std::string, double> parameters) : function{ function }, parameters{parameters} {}
state simpleThtesholdIncrement(state* st) {
	auto temp = st->parameters;
	temp["threshold"]++;
	return state{ st->function, temp};
}
//action constructor
action::action(enum action::type t) :t{ t } {}

//CLASSIFICATION IMAGE 

//used to extract birth and death numbers from the text file
std::vector<birthDeath> getDataFromTextFile(std::string path,std::string filename) {
	std::vector<birthDeath> out{};
	
	std::ifstream file(path + filename + ".txt");
	std::string line;
	//file.open(path + filename);
	int birth, death, count{ 0 };
	while (file >> birth >> death) {
		//std::cout << birth << death<<std::endl;
		out.push_back(birthDeath{ birth,death });
	}

	file.close();
	
	return out;
};


//used for the reward function
std::string classifyTheImage(std::string path,std::string filename,state* st)
{
	
	std::vector<birthDeath> birthsDeaths = getDataFromTextFile(path, filename);
	//std::cout << path + filename<<std::endl;
	if (st->function(path + filename,st->parameters,birthsDeaths)) {
		return "circly";
	}
	else {
		return "lemniscate";

	}
}

//gives a prediction on the birth death numbers
bool simpleTresholdPrediction(std::string pathName, std::map<std::string, double> param, std::vector<birthDeath> birthsDeaths){
	int count = 0;
	for (int i = 0; i < birthsDeaths.size();i++) {
		if (birthsDeaths[i].death - birthsDeaths[i].birth > param["threshold"]) {
			count++;
		}
	}
	//std::cout << count<<std::endl;
	if (count < 2) {
		return true;
	}
	else {
		return false;
	}
}


