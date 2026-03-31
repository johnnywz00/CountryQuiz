/*
 TO DO:
 
 ! got a spinning beach ball when clicking rapidly twice in vicinity of Andorra
 - voice record capitals to pair with the countries
 -(lowprio): underlying rt uses solid color for algs, but a copy rt mottles
 and blurs the color for visual presentation
 -(low): store pixelid->countryname per continent load so mousemove can immediately know what country it's over with vector/stride or [x][y] lookup
 -convert click coordinates to fit other screen sizes
 -menu/splash?
 -make ctyNameTxt appear by the actual country (in Learn)?
	make it a hover function that displays txt relative to cursor
	(but lots of flood alg if run on every mousemove)
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
	View					vw;
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
		"Esc = Quit          John Ziegler, 2025   johnnywz00@yahoo.com\n"
		" \nQ = Take quiz!\n1 = Asia\n2 = Europe\n3 = Africa\n"
		"4 = South America\n5 = United States"
	;
	
	const Color	correctColor {145, 201, 77};
	const Color	incorrectColor {173, 72, 50};
	const Color countryNameColor {35, 30, 25};
	const float secsBeforeNext = 1.8;
	
	
	void reset ();
	
	void loadContinent (Continent&);
	
	void launchQuiz ();
	
	void refreshMapImage ();
	
	void formListForQuiz ();

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
	RoundedRectangle			quizResultsRect;
	Text    			 		mouseTxt
								, ctyNameTxt
								, instrucsTxt
								, quizResultsTxt
	;
	Textbox						newCountryTbox;
	Textbox*					activeTbox = nullptr;

	Mode						curMode;
	vector<Continent>			continents;
	vector<Country>				countries;
	Texture						curMapTx;
	ZImage						zimg;

	Continent*					curContinent;
	unique_ptr<QuizResults>		curQuizResults;
	vector<string>				curQuizList;
	int							curQuizListIdx;
	
	bool						usingPartialList;
	vector<string>				curPartialList;
}; //end class State

#endif
