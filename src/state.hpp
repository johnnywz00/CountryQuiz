/*
 TO DO:
 
 -convert click coordinates to fit other screen sizes
 -menu/splash?
 -make ctyNameTxt appear by the actual country (in Learn)?
 -redo some voice recording
 -sound/anim/stats when quiz done
 -add miniscule countries in more challenging mode?
 */

#ifndef COUNTRYQUIZ_H
#define COUNTRYQUIZ_H

#include "country.hpp"


class FullscreenOnlyApp;
class TimedEventManager;

class State
{
public:
	void onCreate ();
	
	bool handleTextEvent (Event&);
	
	void onMouseDown (int x, int y);
	
	void onMouseUp (int x, int y) { }
	
	void onKeyPress (Keyboard::Key);
    
	void onKeyRelease (Keyboard::Key) { }
	
	void update (const Time& time);
	
	void draw ();

	
	RenderWindow*  		 	rwin;
	FullscreenOnlyApp* 		app;
	TimedEventManager*      timedMgr;
	vecI					mouseVec
							, oldMouse
	;

private:
	const vector<vector<string>> continentCfgLists {
		{ "asia", "asiaCoords.txt", "asiaMap", "asiaSounds" }
		, { "europe", "europeCoords.txt", "europeMap", "europeSounds" }
		, { "africa", "africaCoords.txt", "africaMap", "africaSounds" }
		, { "samerica", "samericaCoords.txt", "samericaMap", "samericaSounds" }
		, { "usa", "usaCoords.txt", "usaMap", "usaSounds" }
	};
	
	const string instrucsStr =
		"Esc = Quit    John Ziegler, 2025   johnnywz00@yahoo.com\n"
		"Q = Take quiz!   1 = Asia   2 = Europe   3 = Africa   "
		"4 = South America   5 = United States"
	;
	
	const Color	correctColor {145, 201, 77};
	const Color	incorrectColor {173, 72, 50};
	const Color countryNameColor {35, 30, 25};
	const float secsBeforeNext = 1.8;
	
	
	void reset ();
	
	void loadContinent (Continent&);
	
	void launchQuiz ();

	string fmtForDisplay (string&);
	
	/* Only for creating new map sets */
	void registerCoords(string fname);

	/* For prepping new map image files: set all
	 * non-country-border pixels to white
	 */
	void makeBlackAndWhite (string fname);
	
	
	vector<SoundBuffer> 		buffers;
	map<string, Sound> 			soundMap;

	Sprite						mapSprite;
	vector<Continent>			continents;
	vector<Country>				countries;
	Mode						curMode;
	ZImage						zimg;
	Texture						curMapTx;
	Continent*					curContinent;
	vector<string>				curQuizList;
	int							curQuizListIdx;
	Text    			 		mouseTxt
								, ctyNameTxt
								, instrucsTxt
	;
	Textbox						newCountryTbox;
	Textbox*					activeTbox = nullptr;
}; //end class State

#endif
