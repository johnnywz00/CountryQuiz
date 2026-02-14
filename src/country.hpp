//
//  country.hpp
//  CountryQuiz
//
//  Created by John Ziegler on 5/24/25.
//  Copyright © 2025 John Ziegler. All rights reserved.
//

#ifndef country_hpp
#define country_hpp

#include "jwzsfml.hpp"
#include "resourcemanager.hpp"
#include "timedeventmanager.hpp"


enum Mode { learn, quiz, quizEnded, configNewMap };


struct Country
{
	bool checkContainsClick(int x, int y, ZImage);
	
	string 			name;
	vector<vecI>	coords;
};



struct Continent
{
	Continent (string nam, string coords, string map, string sounds)
		: name(nam)
		, coordsFile(coords)
		, mapKey(map)
		, soundsFolder(sounds)
	{ }
	
	string 			name;
	string			coordsFile;
	string			mapKey;
	string			soundsFolder;
	vecF			countryDisplayPos;
	bool			soundsLoaded = false;
};



//struct Mode
//{
//	Mode() {}
//	Mode(string str)
//		: name(str)
//	{}
//	string			name;
//};


#endif /* country_hpp */
