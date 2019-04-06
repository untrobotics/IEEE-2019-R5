#include "include/json/json.h"
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
struct objectlocation
{
	int xblock;
	int yblock;
};
struct lidarscans
{
	float degrees;
	float distance;
};
int main()
{
	std::string line;
	std::string token;
	Json::Value json_obj;
	std::ifstream json_file("mars2.json",std::ifstream::binary);
	json_file >> json_obj;
	json_file.close();
	int size = json_obj["size"].asInt();
	std::cout << "Size is: " << size << std::endl;
	objectlocation objects[size];
	int i;
	for(i=0;i < size;i++)
	{
		objects[i].xblock = json_obj["x coords"][i].asInt();
		std::cout << "X coord " << i << ":" << objects[i].xblock << std::endl;
		objects[i].yblock = json_obj["y coords"][i].asInt();
		std::cout << "Y coord " << i << ":" << objects[i].yblock << std::endl;
	}
	std::ifstream lid("lidar3GOOD2.txt");
	lidarscans scans[1000];
	i = 0;
	float lidarx = 3 * 2.54;
	float lidary = 1.375 * 2.54;
	float robotx = 4.5*12*2.54;
	float roboty = 4.5*12*2.54;
	float lidarfromcenter = sqrt(pow(lidary,2)+pow(lidarx,2));
	float lidaroffset = atan(lidary / lidarx);
	float lidarcoordx;
	float lidarcoordy;
	float scannedpointx;
	float scannedpointy;
	while(getline(lid,line))
	{
		int it = line.find_first_of(",");
		token = line.substr(0,it);
		scans[i].degrees = (3.14159265359f / 180.0f) * stof(token);
		//scans[i].degrees = stof(token);
		int it2 = line.find_first_of("\n");
		token = line.substr(it+1,it2);
		scans[i].distance = stof(token) * 100.0f;
		
		lidarcoordx = robotx +( lidarfromcenter * cos(scans[i].degrees + lidaroffset));
		lidarcoordy = roboty +( lidarfromcenter * sin(scans[i].degrees + lidaroffset));
		//std::cout << "The lidar x coordinates are " << lidarcoordx << std::endl;
		//std::cout << "The lidar y coordinates are " << lidarcoordy << std::endl;
		//std::cout << "The distance is: " << scans[i].distance << std::endl;
		scannedpointx = lidarcoordx +( scans[i].distance * cos(scans[i].degrees));
		scannedpointy = lidarcoordy +( scans[i].distance * sin(scans[i].degrees));

		std::cout << "Angle is: " << scans[i].degrees << std::endl;
		std::cout << "The x coordinates are " << scannedpointx << std::endl;
		std::cout << "The y coordinates are " << scannedpointy << std::endl;
		if((scannedpointx >= (objects[0].xblock * 12 * 2.54) && scannedpointx <= ((objects[0].xblock + 1) * 12 * 2.54))&&(scannedpointy >= (objects[0].yblock * 12 * 2.54) && scannedpointy <= ((objects[0].yblock + 1) * 12 * 2.54)))
		{
			std::cout << "Coordinates are for block 1." << std::endl;
		}
		else if((scannedpointx >= (objects[1].xblock * 12 * 2.54) && scannedpointx <= ((objects[1].xblock + 1) * 12 * 2.54))&&(scannedpointy >= (objects[1].yblock * 12 * 2.54) && scannedpointy <= ((objects[1].yblock + 1) * 12 * 2.54)))
		{
			std::cout << "Coordinates are for block 2." << std::endl;
		}
		i++;
	}
	return 0;
}