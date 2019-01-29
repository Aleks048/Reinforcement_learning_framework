#include "generating_images.cpp"
#include "classificationCount.h"
#include "Agent.h"

int main(int argc, char* argv[])
{
	/*
	//create an image with desired (width,height)
	rndPoints c{ 100,100 };
	c.generateBlank();//create a blank image
					  //c.generateNoisy(50);

					  //c.drawCircly(true,1,200,25);
					  //c.drawLemniscate(25,200,25);
	
	*/
	//testing the classification functions
	/*state st{};
	st.function = &simpleTresholdPrediction;
	st.parameters = std::map<std::string, double>{};
	st.parameters["threshold"] = 20;

	std::string temp = classifyTheImage("C:\\Users\\ytr16\\source\\repos\\generating_images\\generating_images\\images\\circly\\supportTextFiles\\","Vsimpl_circly_0.txt_1",
					&st				
					);
	
	std::cout << temp;*/
	
	state st{};
	st.function = &simpleTresholdPrediction;
	st.parameters = std::map<std::string, double>{};
	st.parameters["threshold"] = 30;


	Agent a{st, { "circly", "lemniscate" },3 };
	
	

	//a.calculateTheScore(&st);

	std::system("pause");


	return 0;
}