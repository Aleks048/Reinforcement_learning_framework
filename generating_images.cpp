// generating_images.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <fstream>

//#include <list>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <random>

#include <opencv2/opencv.hpp>
using namespace cv;


using namespace std;



//class point
//{
//public:
//	int x, y;
//	point() { x = y = 0; }
//	point(int a, int b) { x = a; y = b; }
//	void set(int a, int b) { x = a; y = b; }
//	
//};
//--------------------------------------------------------------------------------------------------


class rndPoints
{

public:

	rndPoints(int height, int width) {
		this->imageHeight = height;
		this->imageWidth = width;
		this->centreX = width / 2;
		this->centreY = height / 2;
	}

	void generateBlank() {
		
		blankIm={ imageHeight, imageWidth, CV_8UC3, Scalar(255,255,255) };

		cv::imwrite("blank_image.jpg", blankIm);
	};
	void generateNoisy(int numOfPoints) {

		std::mt19937 eng(rd());
		std::uniform_int_distribution<> randX(0,imageWidth - 1);
		std::uniform_int_distribution<> randY(0,imageHeight - 1);
		
		//creating blank
		noisyIm = { imageHeight, imageWidth, CV_8UC3, Scalar(255,255,255) };

		//fill with noise
		for (int i = 0; i < numOfPoints; ++i) {
			int x = randX(eng);
			int y = randY(eng);
			
			noisyPoints.push_back({x-imageWidth/2,y-imageHeight/2});

			Vec3b pixel{ 0,0,0 };
			noisyIm.at<Vec3b>(x, y) = pixel;
		}
		cv::imwrite("noisy_image.jpg", noisyIm);
	}

	//void generateNoisy(int numOfNoisePoints) {
	//	Mat noisyImTemp = { blankIm.clone() };
	//	
	//	
	//	std::random_device rd;
	//	std::mt19937 eng(rd());
	//	std::uniform_int_distribution<> distrX(0, imageWidth);
	//	std::uniform_int_distribution<> distrY(0, imageHeight);

	//	for (int i = 0; i < numOfNoisePoints; ++i) {
	//		int x = distrX(eng);
	//		int y = distrY(eng);

	//		point p{ x,y };
	//		noisyImTemp = drawPoint(p, noisyImTemp);
	//	}
	//}

	//circly image genrate points/add to image/clear points
	void drawCircly(bool blankOrNoisy, int imageNum, int numOfPoints, int radius)
	{
		for (int i = 0; i < imageNum; ++i) {
			generateCircly(i,blankOrNoisy,numOfPoints, radius);	
		}

	}

	void drawLemniscate(const int& numOfImages,const int& numOfPoints,const int& radius) {
		for (int i = 0; i < numOfImages; ++i) {
			generateLemniscate(i,numOfPoints, radius);
		}
	}

	//
	//void drawBlob(int imageNum,int numOfPoints,int numOfVertices) {

	//	for (int i = 0; i < imageNum; ++i) {
	//		vector<Point2f> temp= generatePolygon(numOfVertices);


	//		//cout << temp.size();
	//		drawPointsBlobs(imageNum,numOfPoints, numOfVertices,temp);
	//	}
	//}
	   
private:
	void vietoris_rips(const vector<Point>& points,std::string name,const int& imNum) {
		string out = "";

		out += "2\n";
		out += "0.15 0.005 300\n";

		for (auto i = points.begin(); i < points.end(); ++i) {
			out += to_string(((imageWidth/2)+(double)(i->x))/100) + " " + to_string(((imageHeight / 2)+(double)(i->y))/100)+" "+"0"+"\n";
		}

		std::string fullName{ "Vsimpl_" + name + "_" + to_string(imNum) + ".txt" };

		std::string path;
		if (name == "circly") {
			path = "./images/circly/supportTextFiles/";
		}
		else if (name == "lemniscate") {
			path = "./images/lemniscate/supportTextFiles/";
		}

		ofstream output;
		
			output =ofstream{path +fullName };
		
		output << out;
		output.close();

		std::string outStr{ "perseus brips " + path + fullName + " " + path + fullName };

		std::system(&outStr[0]);
	}




	//draw a point to the image

	Mat drawPoint(Point& p, Mat& image) {
		int centreX = imageWidth / 2;
		int centreY = imageHeight / 2;

		int i = centreY + p.y;
		int j = centreX + p.x;

		//cout << image.size();

		Vec3b &intensity = image.at<Vec3b>(j, i);
		for (int k = 0; k < image.channels(); k++) {
			intensity.val[k] = 0;
		}
		return image;
	}

	//Circly image
	void generateCircly(int imNum,bool blankOrNoisy,int numOfPoints, int radius)
	{
		std::random_device rd;
		std::mt19937 eng(rd());

		//creating the contour

		vector<vector<Point>> contours;
		vector<Point>contourUp,contourDown,out;

		std::uniform_real_distribution<> rand{0.08,0.19};
		double multiplier = rand(eng);

		int numOfRegions = 5;
		vector<double> multipliers;
		for (int i = 0; i < numOfRegions; ++i){
			multipliers.push_back(rand(eng));
		}

		for (int x = 0; x < imageWidth; ++x) {
			for (int y = 0; y < imageHeight; ++y) {
				if ((pow(x-imageWidth/2, 2) + pow(y-imageHeight/2, 2)- pow(radius,2)<=100 )&& (pow(x - imageWidth / 2, 2) + pow(y - imageHeight / 2, 2)-pow(radius, 2) >= 0)) {
					if (y - imageHeight / 2 >= 0) {

						contourUp.push_back({ x,(int)(max(1+sin(y*multipliers[0])/10,0.0)*y) });
					}
					else {
						contourDown.push_back({ x,(int)(max(1 + sin(y*multipliers[1]) / 10,0.0)*y) });
					}
				}
			}
		}
		std::sort(contourUp.begin(), contourUp.end(), [=](Point i, Point j)->bool {return i.x > j.x; });
		std::sort(contourDown.begin(), contourDown.end(), [=](Point i, Point j)->bool {return i.x < j.x; });

		out = contourUp;
		out.insert(out.end(), contourDown.begin(), contourDown.end());

		contours.push_back(out);



		//seeding points
		
		vector<Point> circlyPointList;
		Point pt;
		
		
		std::uniform_int_distribution<> distrX(-(imageWidth - 1) / 2, (imageWidth - 1) / 2);
		std::uniform_int_distribution<> distrY(-(imageHeight - 1) / 2, (imageHeight - 1) / 2);
		

		//for (int i = 0; i < numOfPoints; i++)
		//{
		//	int x, y;

		//	while (true)
		//	{
		//	x = distrX(eng);
		//	y = distrY(eng);
		//	
		//	if (abs(pointPolygonTest(contours[0], Point{ centreY + y,centreX+x}, true)) < 2) break;
		//	}

		//	//pt =Point(x, y);

		//	circlyPointList.push_back({x,y});
		//}

		circlyPointList = seedThePoints(numOfPoints, contours);

		//add noisy points to the points collection
		if (!blankOrNoisy) {
			circlyPointList.insert(circlyPointList.end(), noisyPoints.begin(), noisyPoints.end());
		}


		//creating simplicies
		vietoris_rips(circlyPointList,"circly",imNum);


		//drawing popint to the image
		
		Mat circly;
		if(blankOrNoisy){
			circly = blankIm.clone();
		}
		else {
			circly = noisyIm.clone();
		}

		int size = circlyPointList.size();
		for (int i = 0; i < size; i++) {
			circly = drawPoint(circlyPointList.front(), circly);
			circlyPointList.erase(circlyPointList.begin());
		}

		cv::imwrite("./images/circly/images/circly" + to_string(imNum) + "_noContour.jpg", circly);
		drawContours(circly, contours, 0, (0, 50, 255), 1);//if want to draw the contour of the image
		cv::imwrite("./images/circly/images/circly"+to_string(imNum)+".jpg", circly);

	}	
	

	//Random blob image
	/*
	vector<Point2f> generatePolygon(int numOfVertices) {
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distrR(100, (imageWidth - 1) / 2);

		double angle = 0.0;
		double incrAngle = 0.0;

		vector<Point2f> vertices(numOfVertices);

		for (int i = 0; i < numOfVertices; ++i) {
			std::uniform_real_distribution<> distrA(incrAngle + (2 * atan(1) - incrAngle) / 2, 2 * atan(1));
			incrAngle = distrA(eng);

			int radius = distrR(eng);


			int x = imageWidth / 2 + radius * cos(angle);
			int y = imageHeight / 2 + radius * sin(angle);

			vertices[i] = Point(x, y);
			if (i < numOfVertices / 4) {
				angle = incrAngle;
			}
			else if ((i >= numOfVertices / 4) && (i < numOfVertices / 2)) {
				if (i == numOfVertices / 4) {
					incrAngle = 0.0;
				}
				angle = 2 * atan(1) + incrAngle;
			}
			else if ((i >= numOfVertices / 2) && (i < 3 * numOfVertices / 4)) {
				if (i == numOfVertices / 2) {
					incrAngle = 0.0;
				}
				angle = 4 * atan(1) + incrAngle;
			}
			else {
				if (i == 3 * numOfVertices / 4) {
					incrAngle = 0.0;
				}
				angle = (3 * 2)*atan(1) + incrAngle;
			}

		}
		return vertices;
	
	}

*/



	
	void generateLemniscate(int imNum,int nOfPoints,int radius) {
		//std::random_device rd;
		std::mt19937 eng(rd());

		std::uniform_real_distribution<> randXY(0.08,0.2);
		


		vector<vector<Point>> contours;
		vector<Point> upper,lower,out;

		double a=1, b=1, c=1, d=0, e=0.015;//parameters
		double varX, varY;

		double randY = 0.8*randXY(eng);

		for (int x= 0; x < imageWidth; ++x) {
			for (int y = 0; y < imageHeight; ++y) {
				varX = (centreX - x);
				varY = (centreY - y);

				double randX = 0;// 5 * sin(x);
				
				
				if (((a*pow(pow(varX,2), 2) - b*2*pow(radius,2)*pow(varX,2) + c*2*pow(radius, 2)*pow(varY, 2) + d*2*pow(radius, 2)*(varX)*(varY) + e*2*pow(radius,2)*pow(varX,3)) <=50000)
					&& ((a*pow(pow(varX, 2), 2) - b * 2 * pow(radius, 2)*pow(varX, 2) + c * 2 * pow(radius, 2)*pow(varY, 2) + d * 2 * pow(radius, 2)*(varX)*(varY)+e * 2 * pow(radius, 2)*pow(varX, 3)) >= 0)
					) {
					out.push_back(cv::Point(x + randX, (int)(max(1 + sin(y*randY) / 10, 0.0)*y)));
					
				}
			}
		}
		
		
		
		
		//separate upper and lowerr halfs
		std::sort(out.begin(), out.end(), [=](Point i, Point j)->bool {return i.x < j.x; });

		cv::Point leftest = *(out.begin());
		cv::Point rightest = *(out.end()-1);


		double slopeL = ((centreY - leftest.y)*1.0) / (centreX-leftest.x);
		double slopeR = ((centreY - rightest.y)*1.0) / (centreX - rightest.x);
		double bL = centreY - centreX * slopeL;
		double bR = centreY - centreX * slopeR;

		for (auto i=out.begin(); i != out.end();++i) {
			if (i->x <= centreX) {
				if (i->y <= (slopeL*i->x + bL)) { upper.push_back(*i); }
				else {
					lower.push_back(*i);}
			}
			else {
				if (i->y<=(slopeR*i->x + bR)) { upper.push_back(*i); }
				else { lower.push_back(*i); }
			}
		}
		out.clear();

		std::sort(upper.begin(), upper.end(), [=](Point i, Point j)->bool{return i.x > j.x; });
		std::sort(lower.begin(), lower.end(), [=](Point i, Point j)->bool {return i.x < j.x; });
		out.insert(out.end(), upper.begin(), upper.end());
		out.insert(out.end(), lower.begin(), lower.end());

		contours.push_back(out);



		//seed the points
		vector<Point>lOfPoints = seedThePoints(nOfPoints, contours);


		std::sort(lOfPoints.begin(), lOfPoints.end(), [=](Point i, Point j)->bool {if (i.x != j.x) { return i.x < j.x; } else { return i.y < j.y; } });




		//create simplicies
		vietoris_rips(lOfPoints,"lemniscate",imNum);



		//draw points and contour into the file
		Mat lemniscate = blankIm.clone();

		int size = lOfPoints.size();
		for (int i = 0; i < size; i++) {
			lemniscate = drawPoint(lOfPoints.front(), lemniscate);
			lOfPoints.erase(lOfPoints.begin());
		}


		cv::imwrite("./images/lemniscate/images/lemniscate" + to_string(imNum) + "_noContour.jpg", lemniscate);
		drawContours(lemniscate, contours, 0, (0,50,255), 1);//if want to draw the contour of the image
		cv::imwrite("./images/lemniscate/images/lemniscate"+to_string(imNum)+".jpg", lemniscate);
		
	}
	



	template<typename T>
	T normal_pdf(T x,T m,T s) {
		static const T arg = 0.3989422804014327;//1/(sqrt of 2 PI)
		T a = (x - m) / s;
		return arg / s * exp(-T(0.5)*a*a);
	}




	vector<Point> seedThePoints(const int& nOfPoints,const vector<vector<Point>>& contours) {

		std::mt19937 eng(rd());

		std::uniform_int_distribution<> distrX(-(imageWidth - 1) / 2, (imageWidth - 1) / 2);
		std::uniform_int_distribution<> distrY(-(imageHeight - 1) / 2, (imageHeight - 1) / 2);

		std::bernoulli_distribution bernoulli_distribution;

		vector <Point> lOfPoints;

		for (int i = 0; i < nOfPoints; i++) {
			Point p;

			int x = distrX(eng);
			int y = distrY(eng);

			double test = pointPolygonTest(contours[0], Point{ centreY + y,centreX + x }, true);
			

			//normal distribution parameters
			double xPositiveMult = 0.8;
			double xNegativeMult = 0.6;
			int mean = 0;
			double var = 2.9;

			double chance;
			if (test > 0) {
				chance = normal_pdf<double>(test*xPositiveMult, mean, var);
			}
			else {
				chance = normal_pdf<double>(test*xNegativeMult, mean, var);
			}
			bernoulli_distribution = std::bernoulli_distribution(chance);
			bool takeThePoint = bernoulli_distribution(eng);

			while (!takeThePoint) {
				x = distrX(eng);
				y = distrY(eng);
				test = pointPolygonTest(contours[0], Point{ centreY + y,centreX + x }, true);

				if (test > 0) {
					chance = normal_pdf<double>(test*xPositiveMult, mean, var);
				}
				else {
					chance = normal_pdf<double>(test*xNegativeMult, mean, var);
				}

				bernoulli_distribution = std::bernoulli_distribution(chance);
				takeThePoint = bernoulli_distribution(eng);

			}

			/*while (abs(test) >1) {
				x = distrX(eng);
				y = distrY(eng);
				test = pointPolygonTest(contours[0], Point{ centreY + y,centreX + x }, true);
			}*/

			//check if point is already there
			bool isIn{ false };
			for (auto i = lOfPoints.begin(); i != lOfPoints.end(); ++i) {
				if (i->x == x && i->y == y) {
					isIn = true;
				}
			}
			if (!isIn) {
				lOfPoints.push_back(Point{ x,y });
			}
		}
		
		return lOfPoints;
	} 



		/*
		param :
			radius is in pixels
			
		*/
	//	void createSimplicities(const int radius, const vector<point>& lOfPoints) {
	//		//create simplicities
	//		vector<vector<vector<point>>> simpl;
	//		

	//		
	//		//zero form
	//		vector<vector<point>> zeroForm;
	//		
	//		for (auto i = lOfPoints.begin(); i != lOfPoints.end(); ++i) {
	//			zeroForm.push_back(vector<point>{*i});
	//		}
	//		


	//		//first form;
	//		vector<vector<point>> firstForm;

	//		for (auto i = lOfPoints.begin(); i != lOfPoints.end(); ++i) {
	//			for (auto j = lOfPoints.begin(); j != lOfPoints.end(); ++j) {
	//				if ((j->x != i->x)||(j->y != i->y)) {
	//					if (pow(pow(i->x - j->x, 2) + pow(i->y - j->y,2),0.5)<radius) {
	//						bool isIn{ false };
	//						for (auto k = firstForm.begin(); k != firstForm.end(); ++k) {
	//							if (((*k)[0].x == j->x) && ((*k)[0].y == j->y) && ((*k)[1].x == i->x) && ((*k)[1].y == i->y)) { isIn = true; }
	//						}
	//						if (!isIn) {
	//							firstForm.push_back({ *i,*j });
	//						}
	//					}
	//				}
	//			}
	//		}



	//		//second from
	//		vector<vector<point>> secondForm;
	//		
	//		for (auto i = lOfPoints.begin(); i != lOfPoints.end(); ++i) {
	//			for (auto j = firstForm.begin(); j != firstForm.end(); ++j) {
	//				for (auto k = j->begin(); k != j->end(); ++k) {
	//					//cout << k->x;
	//					if ((i->x != k->x)&&(i->y!=k->y)) {
	//						if (pow(pow(i->x - k->x, 2) + pow(i->y - k->y, 2), 0.5) > radius) {
	//							break;
	//						}
	//						if (k == j->end() - 1) {
	//							bool isIn {false};
	//							for (auto m = firstForm.begin(); m != firstForm.end(); ++m) {
	//								if (pointIsInVector(*i,*m) && (pointIsInVector((*j)[0], *m))&&(pointIsInVector((*j)[1], *m)))
	//								{
	//									//cout << i->x << " " <<i->y << " " << (*j)[0].x << " " << (*j)[0].y<< " " << (*j)[1].x << " " << (*j)[1].y<< endl;
	//									isIn = true;
	//									break;
	//								}
	//							}
	//							if (isIn==true) {
	//								auto temp = *j;
	//								temp.push_back(*i);
	//								secondForm.push_back(temp);
	//							}
	//						}
	//					}
	//					
	//				}
	//			}
	//		}

	//		vector<vector<point>> thirdForm;
	//		for (auto i = lOfPoints.begin(); i != lOfPoints.end(); ++i) {
	//			for (auto j = secondForm.begin(); j != secondForm.end(); ++j) {
	//				for (auto k = j->begin(); k != j->end(); ++k) {
	//					//cout << k->x;
	//					if ((i->x != k->x) && (i->y != k->y)) {
	//						if (pow(pow(i->x - k->x, 2) + pow(i->y - k->y, 2), 0.5) > radius) {
	//							break;
	//						}
	//						if (k == j->end() - 1) {
	//							bool isIn{ false };
	//							for (auto m = firstForm.begin(); m != firstForm.end(); ++m) {
	//								if (pointIsInVector(*i, *m) && pointIsInVector((*j)[0], *m) && (pointIsInVector((*j)[1], *m))&& (pointIsInVector((*j)[2], *m)))
	//								{
	//									isIn = true;
	//								}
	//							}
	//							if (isIn==false) {
	//								auto temp = *j;
	//								temp.push_back(*i);
	//								thirdForm.push_back(temp);
	//							}
	//						}
	//					}
	//					
	//				}
	//			}
	//		}



	//		//add simplicities to vector

	//		simpl.push_back(zeroForm);

	//		simpl.push_back(firstForm);
	//		simpl.push_back(secondForm);
	//		simpl.push_back(thirdForm);


	//		writeSimplicities(simpl);
	//	}

	////check if the point is in the vector
	//bool pointIsInVector (const point& p,const vector<point>& v) {
	//	for (auto i = v.begin(); i != v.end(); ++i) {
	//		if ((i->x == p.x) && (i->y == p.y)) { return true; }
	//	}
	//	return false;
	//}



	//void writeSimplicities(const vector<vector<vector<point>>>& simpl ) {
	//	//create the string
	//	std::string out;
	//	out += "2\n";
	//	int cI = 0;
	//	for (auto i = simpl.begin(); i != simpl.end(); ++i) {

	//		for (auto j = i->begin(); j != i->end(); ++j) {
	//			out += to_string(cI) + " ";
	//			for (auto k = j->begin(); k != j->end(); ++k) {
	//				out+=to_string(imageWidth/2+k->x) + " " + to_string(imageHeight/2+k->y)+" ";
	//			}
	//			if ((i == simpl.end() - 1)&&(j == i->end()-1)) {
	//				out += "1";
	//			}
	//			else {
	//				out += "1\n";
	//			}
	//		}
	//		++cI;
	//	}



	//	//write to the file
	//	ofstream output("simpliciesTry.txt");
	//	output << out;
	//	output.close();
	//	std::system("perseus nmfsimtop simpliciesTry.txt try");
	//	
	//}


	//void drawPointsBlobs(int imageNum,int nOfPoints,int numOfVertices, vector<Point2f>& vertices) {
	//	Mat temp = blankIm.clone();
	//	std::random_device rd;
	//	std::mt19937 eng(rd());

	//	
	//	//drawing polygon
	//	for (int i = 0; i < numOfVertices; ++i) {
	//		line(temp, vertices[i], vertices[(i + 1) % numOfVertices], {0,0,0}, 1, 8);
	//	}
	//	
	//	bitwise_not(temp,temp);

	//	//get polygon contour
	//	Mat randBlob = blankIm.clone();
	//	
	//	cvtColor(temp,temp,CV_BGR2GRAY,1);
	//	vector<vector<Point>> contours; std::vector< cv::Vec4i > hierarchy;
	//	

	//	cv::findContours(temp, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_NONE);
	//	
	//	//generate points
	//	std::uniform_int_distribution<> distrX(-(imageWidth - 1) / 2, (imageWidth - 1) / 2);
	//	std::uniform_int_distribution<> distrY(-(imageHeight - 1) / 2, (imageHeight - 1) / 2);
	//	int centreX = (imageWidth - 1) / 2;
	//	int centreY = (imageHeight - 1) / 2;
	//	list<Point> lOfPoints;

	//	for (int i = 0; i < nOfPoints; i++) {
	//		Point p;

	//		int x = distrX(eng);
	//		int y = distrY(eng);

	//		double test = pointPolygonTest(contours[1], Point{ centreY + y,centreX + x }, false);
	//		while ( test==-1){
	//			x = distrX(eng);
	//			y = distrY(eng);
	//			test = pointPolygonTest(contours[1], Point{ centreY + y,centreX + x }, false);
	//		}

	//		lOfPoints.push_back(Point{ x,y });
	//	}
	//	
	//	//draw points and contour into the files
	//	int size = lOfPoints.size();
	//	for (int i = 0; i < size; i++) {
	//		randBlob = drawPoint(lOfPoints.front(), randBlob);
	//		lOfPoints.pop_front();
	//	}
	//	//drawContours(randBlob, contours, 1, (0,50,255), 3);//if want to draw the contour of the image
	//	
	//	cout << lOfPoints.size();
	//	cv::imwrite(".\\images\\blobs\\blob"+to_string(imageNum)+".jpg", randBlob);
	//	cv::imwrite(".\\images\\blobs\\blobContours\\contour"+to_string(imageNum)+".jpg", temp);
	//
	//}






	int imageWidth, imageHeight,centreX,centreY,numOfPoints;

	Mat blankIm;
	Mat noisyIm;

	vector<Point> noisyPoints;

	std::random_device rd;
	
};


//--------------------------------------------------------------------------------------------------
