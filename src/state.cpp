
#include "state.hpp"
#include "sfmlApp.hpp"


void State::onCreate ()
{
	/* Only for configuring new maps */
	newCountryTbox = Textbox(gFont("debug"), {20, 50});
	mouseTxt = Text("", gFont("debug"), 13);
	mouseTxt.setPosition({8, 9});
	mouseTxt.setFillColor(PURPLE);

	/* Display name of current country in quiz or learn mode */
	ctyNameTxt = Text("", gFont("countryName"), 40);
	ctyNameTxt.setPosition({30, 75});
	ctyNameTxt.setScale({1.5, 1});
	ctyNameTxt.setFillColor(countryNameColor);
	ctyNameTxt.setOutlineThickness(1.5);
	ctyNameTxt.setOutlineColor(withAlpha(countryNameColor, 100));
	
	/* Show continent selection keys */
	instrucsTxt = Text(instrucsStr, gFont("instrucs"), 20);
	instrucsTxt.setPosition({15, 5});
	instrucsTxt.setFillColor(withAlpha(countryNameColor, 100));
	
	/* These sounds sound much louder than the voice sounds
	 * unless we diminish their relative volumes
	 */
	gSound("wrongChoice").setVolume(30.f);
	gSound("rightChoice").setVolume(30.f);
	
	/* If this game were to expand, probably need to parse all of
	 * the country/state lists first and count for reserve
	 */
	buffers.reserve(300);

	for (auto& clist : continentCfgLists) {
		continents.emplace_back(clist[0], clist[1], clist[2], clist[3]);
	}
	countries.reserve(70);
	
	reset();
}

bool State::handleTextEvent (Event& event)
{
	/* The program only uses TextEntered for a special mode
	 * for creating new maps
	 */
	if (activeTbox && !isCmdPressed()
		&& (event.type == Event::TextEntered
			|| event.type == Event::KeyPressed
			|| event.type == Event::KeyReleased)) {
		if (event.type == Event::TextEntered) {
			if (event.text.unicode == 8)
				if (iKP(LShift))
					activeTbox->clear();
				else activeTbox->deleteLastChar();
			else if (event.text.unicode == 9) ; // Don't write the \t
			else activeTbox->appendText(event.text.unicode);
		}
		if (event.type == Event::KeyPressed && (event.key.code == Keyboard::Escape || event.key.code == Keyboard::Enter)) {
			/* Do nothing: Cmd - Enter logs coordinates for a new country */
		}
		return true;
	}
	return false;
}

void State::onMouseDown (int x, int y)
{
	/* Learn mode: print and speak name for
	 * the clicked country
	 */
	if (curMode == learn) {
		for (auto& cty : countries) {
			if (cty.checkContainsClick(x, y, zimg)) {
				soundMap[cty.name].play();
				ctyNameTxt.setString(fmtForDisplay(cty.name));
				break;
			}
		}
	}
	
	/* Quiz mode: color the country and play a sound both
	 * corresponding to whether correct or incorrect click
	 */
	else if (curMode == quiz) {
		for (auto& cty : countries) {
			if (cty.checkContainsClick(x, y, zimg)) {
				/* Correct */
				if (cty.name == curQuizList[curQuizListIdx]) {
					for (auto& xy : cty.coords) {
						zimg.fillInWithColor(vecU(xy), correctColor);
					}
					gSound("rightChoice").play();
				}
				/* Incorrect */
				else {
					for (auto& cty : countries) {
						if (cty.name != curQuizList[curQuizListIdx])
							continue;
						for (auto& xy : cty.coords) {
							zimg.fillInWithColor(vecU(xy), incorrectColor);
						}
					}
					gSound("wrongChoice").play();
				}
				curMapTx.update(zimg);
				
				++curQuizListIdx;
				
				/* Reached end of quiz list */
				if (curQuizListIdx >= curQuizList.size()) {
					// display results
					// play finishing sound
					curMode = quizEnded;
					ctyNameTxt.setString("");
				}
				
				/* Pose the next question after specified time interval */
				else {
					timedMgr->addEvent(secsBeforeNext,
					[&]() {
						soundMap[curQuizList[curQuizListIdx]].play();
						ctyNameTxt.setString(fmtForDisplay(curQuizList[curQuizListIdx]));
					});
				}
				
				/* Don't continue the country loop since we found the click */
				break;
			} // end country contains click
		}
	}
	else if (curMode == quizEnded) {
		curMode = learn;
		mapSprite.setTexture(gTexture(curContinent->mapKey));
	}
}

void State::onKeyPress(Keyboard::Key k)
{
	if (k == Keyboard::Escape)
		app->close();
	
	/* Utility routines only used when creating a new map */
	if (curMode == configNewMap
		&& isCmdPressed()) {
		if (k == Keyboard::Enter)
			registerCoords("NEWMAPNAMEHERE.txt");
		else if (k == Keyboard::W)
			makeBlackAndWhite("NEWMAP.png");
	}
	
	/* Gameplay keystrokes */
	switch(k) {

		/* loadContinent() sets mode to `learn` */
		case Keyboard::Num1:
			loadContinent(continents[0]);
			break;
			
		case Keyboard::Num2:
			loadContinent(continents[1]);
			break;
			
		case Keyboard::Num3:
			loadContinent(continents[2]);
			break;
			
		case Keyboard::Num4:
			loadContinent(continents[3]);
			break;
			
		case Keyboard::Num5:
			loadContinent(continents[4]);
			break;
			
		case Keyboard::Q:
			launchQuiz();
			break;
			
		/* Replay sound for current quiz item */
		case Keyboard::Space:
			if (curMode == quiz)
				soundMap[curQuizList[curQuizListIdx]].play();
			break;
			
		default:
			break;
	}
}

void State::update (const Time& time)
{
	timedMgr->fireReadyEvents(time);
	
	if (curMode == configNewMap)
		mouseTxt.setString(tS(mouseVec.x) + ", " + tS(mouseVec.y));
}

void State::draw ()
{
	rwin->draw(mapSprite);
	
	if (curMode == configNewMap) {
		rwin->draw(mouseTxt);
		rwin->draw(newCountryTbox);
	}
	
	else {
		rwin->draw(ctyNameTxt);
		rwin->draw(instrucsTxt);
	}
}

void State::reset ()
{
	loadContinent(continents[0]);
}

void State::loadContinent (Continent& cont)
{
	curMode = learn;
	ctyNameTxt.setString("");
	curContinent = &cont;
	curMapTx = gTexture(cont.mapKey);
	zimg = curMapTx.copyToImage();
	mapSprite.setTexture(curMapTx);
	// Don't center the sprite because click coordinates
	//  are hardcoded from top-left corner (0, 0) of map image
		
	countries.clear();
	std::ifstream countryFile {
		Resources::executingDir() / "resources" / cont.coordsFile
	};
	if (!countryFile.is_open()) {
		cerr << "Error loading country coordinates file (" << cont.coordsFile << ").\n";
		return;
	}
	string line;
	Country c_;
	Country* c;
	while (getline(countryFile, line)) {
		stringstream ss(line);
		string token;
		vecI coords;
		ss >> token;
		if (!token.size() || isspace(token[0]) || token == ":")
			continue;
		token.erase(0, token.find_first_not_of("\n\b"));
		auto itr = find_if(countries.begin(), countries.end(),
						[&](auto x) { return x.name == token; });
		if (itr == countries.end()) {
			c_ = Country();
			c_.name = token;
			c = &c_;
			if (!cont.soundsLoaded) {
				SoundBuffer sb;
				string filePath = (Resources::executingDir() / "resources" / "sounds" / cont.soundsFolder / (token + ".wav")).string();
				if (!sb.loadFromFile(filePath))
					cerr << "Couldn't load sound file " << filePath << endl;
				else {
					buffers.push_back(sb);
					Sound s { buffers.back() };
					soundMap.insert({token, s});
				}
			}
		}
		else {
			c = &(*itr);
		}
		ss >> coords.x >> coords.y;
		if (cont.name == "africa")
			coords.x += 191;	// Changed map after logging Africa coord file
		c->coords.push_back(coords);
		if (itr == countries.end())
			countries.push_back(c_);
	}
	countryFile.close();
	cont.soundsLoaded = true;
}

void State::launchQuiz ()
{
	curMode = quiz;
	
	/* Set all country colors pale: they'll turn
	 * green or red depending on whether answered
	 * correctly.
	 */
	for (auto& cty : countries) {
		for (auto& xy : cty.coords) {
			Color alphaColor = zimg.getPixel(vecU(xy.x, xy.y));
			alphaColor.a = 60;
			zimg.fillInWithColor(vecU(xy), alphaColor);
		}
	}
	curMapTx.update(zimg);
	
	/* Load the names of the current continent's countries */
	curQuizList.clear();
	for (auto& cty : countries)
		curQuizList.push_back(cty.name);
	
	/* Shuffle the countries for the quiz */
	for (size_t i = countries.size() - 1; i > 0; --i) {
		int j = rand() % (i + 1);
		std::swap(curQuizList[i], curQuizList[j]);
	}
	curQuizListIdx = 0;
	
	/* Start quizzing */
	soundMap[curQuizList[0]].play();
	ctyNameTxt.setString(fmtForDisplay(curQuizList[0]));
}

string State::fmtForDisplay (string& str)
{
	string ret = str;
	for (auto& ch : ret)
		if (ch == '-')
			ch = ' ';
	return toUpper(ret);
}

void State::registerCoords(string fname)
{
	std::fstream fs {Resources::executingDir() / "resources" / fname, std::ios_base::app};
	fs << string(newCountryTbox.boxTxt.getString()) << " " << tS(mouseVec.x) << " " << tS(mouseVec.y) << endl;
	fs.close();
}

void State::makeBlackAndWhite (string fname)
{
	zimg.loadFromFile((Resources::executingDir() / fname).string());
	forNum(zimg.getSize().x) {
		forNumJ(zimg.getSize().y) {
			Color pix = zimg.getPixel(vecU(i, j));
			if (pix.r + pix.g + pix.b > 280)
				zimg.setPixel(i, j, Color::White);
		}
	}
	zimg.saveToFile("borders-and-white.png");
}
