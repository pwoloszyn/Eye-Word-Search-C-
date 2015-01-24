#include <string>

#ifndef WORD_H
#define WORD_H

class Word
{
	public:
		int location;
		std::string shape;
		int shapetype;
		int length;
		int points;

		Word();
		Word(int loc, std::string shp, int len, int pts, int shptype);
};

#endif
