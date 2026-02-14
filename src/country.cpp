//
//  country.cpp
//  CountryQuiz
//
//  Created by John Ziegler on 5/24/25.
//  Copyright © 2025 John Ziegler. All rights reserved.
//

#include "country.hpp"


bool Country::checkContainsClick(int x, int y, ZImage z)
{
	for (auto& coord : coords)
		if (x == coord.x && y == coord.y)
			return true;
	vecU startPt = vecU(x, y);
	int numRows = z.getSize().y;
	int numCols = z.getSize().x;
	if (!isOrBetween(x, 0, numCols - 1) ||
		!isOrBetween(y, 0, numRows - 1))
		return false;
	Color colorToReplace = z.getPixel(startPt);
	Color c = colorToReplace;
	c.r = clamp(c.r - 1, 0, 255);

	bool result = false;
	int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	queue<pair<int, int>> unresolvedPts;
	unresolvedPts.push({startPt.x, startPt.y});
	z.setPixel(startPt.x, startPt.y, c);

	while (!unresolvedPts.empty()) {
		auto [cx, cy] = unresolvedPts.front();
		unresolvedPts.pop();
		
		for (auto [dx, dy] : dirs) {
			int nx = cx + dx, ny = cy + dy;
			if (	nx >= 0 && nx < numCols &&
					ny >= 0 && ny < numRows &&
					z.getPixel(nx, ny) == colorToReplace) {
				for (auto& coord : coords)
					if (nx == coord.x && ny == coord.y)
						result = true;
						// No `break` so that country pixels all stay the same
				z.setPixel(nx, ny, c);
				unresolvedPts.push({nx, ny});
			}
		}
	}
	return result;
}
