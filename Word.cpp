#include "Word.h"

using namespace std;

Word::Word() 
{
	location = -1;
	shape = "NULL";
	length = 0;
	points = 0;
	shapetype = -1;
}

Word::Word(int loc, string shp, int len, int pts, int shptype)
{
	location = loc;
	shape = shp;
	length = len;
	points = pts;
	shapetype = shptype;
}
