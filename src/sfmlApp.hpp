//
//  sfmlApp.hpp
//  Hopscotch
//
//  Created by John Ziegler on 10/22/24.
//  Copyright © 2024 John Ziegler. All rights reserved.
//

#ifndef sfmlApp_hpp
#define sfmlApp_hpp


#include "state.hpp"


class FullscreenOnlyApp
{
public:
    FullscreenOnlyApp ();
       
	void run ();
	
	void update ();
	
	void close ()
	{
		isDone = true;
		window.close();
	}
	
	void setRedrawColor (const Color& c) { redrawColor = c; }

	RenderWindow            window;
	TimedEventManager		timedMgr;
	State            		state;
	Clock            		clock;
	Time             		elapsed;
	Image                   icon;
	Color                   redrawColor { Color::White };
	bool					isDone = false;
};

#endif /* sfmlApp_hpp */
