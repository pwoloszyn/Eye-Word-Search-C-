#include "WordGrid.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>

using namespace std;

// The default constructor
WordGrid::WordGrid()
{
	
}

// The constructor.
WordGrid::WordGrid(int x, int y, string category, int difficulty)
{
	wordlistfile = "WordLists/";

	// Sets the gridsize.
	gridsize = x*y;

	grid = new char[gridsize];
	for(int i = 0; i< gridsize ; i++)
		grid[i] = '#';
	count = 0;
	cat = category;	//Defines the word category.
	dif = difficulty;	//Defines the difficulty setting.

	// Specifies the number of words we need to add.
	wordnum = x/2 + (6-dif);
	maxwordsize = x;

	// We fill the wordsublist with junk;
	for(int i=0; i<50; i++)
		WordSubList[i] = "###";

	for(int i=0; i<20; i++)
		usedwords[i] = "";

	wordsinsublist = 0;

	// Sets the path for the wordlist file.
	GetWordListFile();

	GridBuilder();

	FillTheGrid();
}

// Adds the word to the wordlsit array.
void WordGrid::AddWord(Word aword) 
{
	if(count < 20) 
	{
		wlist[count] = aword;
		count++;
	}
}

// Retrieves the board coordinates based on the word and the character
// number of that word (position).
// So the word "apple" has shape "2222", and it's starting position is
// 23, if we want the position of the 'l' or 4th character we do this:
// CoordFromShape("2222", 23, 3).
int WordGrid::CoordFromShape(string shape, int startpoint, int position)
{
	int loc;
	char dir;

	// position 0 reffers to the startpoint, position 1 to the first
	// char of the shape string.
	loc = startpoint;

	for(int i=0;i<position;i++)
	{
		dir = shape.at(i);
		switch (dir)
		{
			case '0':
				loc = loc - maxwordsize;
				break;
			case '1':
				loc = loc - maxwordsize + 1;
				break;
			case '2':
				loc = loc + 1;
				break;
			case '3':
				loc = loc + maxwordsize + 1;
				break;
			case '4':
				loc = loc + maxwordsize;
				break;
			case '5':
				loc = loc + maxwordsize - 1;
				break;
			case '6':
				loc = loc - 1;
				break;
			case '7':
				loc = loc - maxwordsize - 1;
				break;
		}
	}

	return loc;
}

/*
	The following code is the core part of
	the grid generation procedure.
*/
void WordGrid::GridBuilder()
{
	int wordstartpoint;
	int wordshapetype;
	int wordlength;
	string wordshape;

	int randwrdsel;

	string cmparr; // The comparison string, used to handle character overlaps.
	
	srand(time(NULL));

	while(count < wordnum)
	{

		wordstartpoint = rand() % gridsize;
		wordlength = (rand() % (maxwordsize - 3)) + 3; // A minimal word size is 3 letters.
		switch(dif)	// We get the word shape type based on the difficulty setting.
		{
			case 0:
				wordshapetype = 0;
				break;
			case 1:
				wordshapetype = rand() % 2;
				break;
			case 2:
				wordshapetype = rand() % 3;
				break;
			case 3:
				wordshapetype = rand() % 4;
				break;
			case 4:
				wordshapetype = 1 + rand() % 4;
				if(wordlength < maxwordsize)
					wordlength += 1;
				break;
			case 5:
				wordshapetype = 2 + rand() % 3;
				if(wordlength < (maxwordsize - 1))
					wordlength += 2;
				break;
			default:
				break;
		}
		// We generate the word shape string.
		if((wordshapetype == 4) && (wordlength < 5))
			wordlength = 5;


		wordshape = GetWordShape(wordshapetype, wordlength);

		// We check if the word fits the board boundariesswitch statements inside switch statements
		// and that the shape and location isn't already used.
		if(IsLegal(wordshape, wordstartpoint, wordshapetype))
		{
			// We fill up the comparison string with chars from the grid.
			for(int i=0; i<wordlength; i++)
			{
				cmparr += grid[CoordFromShape(wordshape, wordstartpoint, i)];
			}

			// We scan the word list txt file for words that
			// match our criteria.
			string temp;
			ifstream wlfile(wordlistfile.c_str());
			while(getline(wlfile, temp)) 
			{
				WordCompare(temp, cmparr);
			}
			// Better safe than sorry.
			wlfile.close();

			// If we found at least one word, we randomly select one
			// and add it to the board word list & and we put the characters in the grid.
			if(wordsinsublist>0)
			{
				randwrdsel = rand() % Lengthofsublist();

				Word wordt(wordstartpoint, wordshape, wordlength, wordlength/2, wordshapetype);
				for(int i=0;i<wordlength;i++)
				{
					grid[CoordFromShape(wordshape, wordstartpoint, i)] = WordSubList[randwrdsel].at(i);
				}
				usedwords[count] = WordSubList[randwrdsel];
				AddWord(wordt);
			}
		}
		// Reset the variables:
		wordsinsublist = 0;
		cmparr = "";
		for(int i=0; i<50; i++)
			WordSubList[i] = "###";
		wordshape = "";
	}
}

// This method handles the word lists directories
// data, so that our grid builder method can know
// where to find the file corresponding to our
// category and difficulty specification. 

void WordGrid::GetWordListFile()
{	

	wordlistfile += cat;
	int cri = dif/2;

	switch (cri)
	{
		case 0:
			wordlistfile += "/easy.txt";
			break;
		case 1:
			wordlistfile += "/med.txt";
			break;
		case 2:
			wordlistfile += "/hard.txt";
			break;
	}
}

/*
	This method produces a word shape string, here is the key:
	
	7 0 1
	6 * 2
	5 4 3

	The star indicates the word start point. A word shape "222",
	would be a 4 letter word that is horizontal an going right, while a
	"55555" would be a six letter word that is diagonal and going down and
	left.
	
	The types are:
	0 - horizontal & vertical.
	1 - diagonal.
	2 - circular. (words that take two turns, like: "224466" or "111333555").
	3 - wave shaped. (words like: "11331133" or "44220022").
	4 - custom shped.	(Uses a set of premade shapes).

*/
string WordGrid::GetWordShape(int type, int length)
{

	string shape = "";

	char directions[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};
	int selection;

	int cmplen;
	int suboradd;
	int startdir;
	int subroutelen;
	int counterorclock;
	
	switch(type)
	{
		case 0:	// horizontal & vertical
			selection = (rand() % 4) * 2;

			for(int i=1;i<length;i++)
			{
				shape += directions[selection];
			}
			break;
		case 1: // diagonal
			selection = 1 + (rand() % 4) * 2;
			
			for(int i=1;i<length;i++)
			{
				shape += directions[selection];
			}
			break;
		case 2: // circular
			cmplen = 0;
			suboradd = rand() % 2;
			startdir = rand() % 8;
			subroutelen = (length+1)/3;
			
			if(suboradd == 0)
			{
				counterorclock = 2;
			} else {
				counterorclock = -2;
			}
			for(int i=0;i<subroutelen;i++)
			{
				shape += directions[startdir];
				cmplen++;
			}
			startdir = TrueModulo((startdir+counterorclock), 8);
			for(int i=0;i<subroutelen;i++)
			{
				shape += directions[startdir];
				cmplen++;
			}
			startdir = TrueModulo((startdir+counterorclock), 8);
			for(int i=0;i<subroutelen;i++)
			{
				if(cmplen < length - 1)
				{
					shape += directions[startdir];
					cmplen++;
				}
			}
			startdir = TrueModulo((startdir+counterorclock), 8);
			for(int i=cmplen;i<length-1;i++)
			{
				shape += directions[startdir];
			}
			break;
		case 3: // wave shaped
			startdir = rand() % 8;
			suboradd = rand() % 2;
			for(int i=1; i<length; i++)
			{
				if(suboradd == 0) // 0 means we subtract from the direction value
				{
					startdir = TrueModulo((startdir-2), 8);
					shape += directions[startdir];
					suboradd = 1;
				} else {
					startdir = TrueModulo((startdir+2), 8);
					shape += directions[startdir];
					suboradd = 0;
				}
			}
			break;
		case 4: // custom
			shape = CustomShapes(length);
			break;
		default:
			break;
	}

	return shape;
}

// Checks whether it is possible to place the word shape on the board.
bool WordGrid::IsLegal(string shape, int startpoint, int shapetyp)
{
	int pos;
	char temp;
	for(int i=0; i<(shape.length()); i++)
	{
		pos = CoordFromShape(shape, startpoint, i);
		temp = shape.at(i);

		// This switch statment checks if the word shape
		// fits in the board boundaries.
		switch(temp)
		{
			case '0':
				if((pos / maxwordsize) < 1)
					return false;
				break;
			case '1':
				if(((pos / maxwordsize) < 1) || ((pos % maxwordsize) == (maxwordsize - 1)))
					return false;
				break;
			case '2':
				if((pos % maxwordsize) == (maxwordsize - 1))
					return false;
				break;
			case '3':
				if(((pos / maxwordsize) > (maxwordsize - 2)) || ((pos % maxwordsize) == (maxwordsize - 1)))
					return false;
				break;
			case '4':
				if((pos / maxwordsize) > (maxwordsize - 2))
					return false;
				break;
			case '5':
				if(((pos / maxwordsize) > (maxwordsize - 2)) || ((pos % maxwordsize) == 0))
					return false;
				break;
			case '6':
				if((pos % maxwordsize) == 0)
					return false;
				break;
			case '7':
				if(((pos / maxwordsize) < 1) || ((pos % maxwordsize) == 0))
					return false;
				break;
			default:
				break;
		}		
	}

	// This for loop checks if the word shapes overlap with an
	// already present word on the board.
	for(int i=0; i<wordnum; i++)
	{
		if(wlist[i].location == startpoint && wlist[i].shapetype == shapetyp)
			return false;
	}
	return true;
}

// Checks if the given word string contains at specific locations
// the chars provided by the comparator string, as well as being of equal length.
// So if the word string is: "apple", and the comaprator is: "###l#" we
// have a match and the word is added to the WordSubList array.
void WordGrid::WordCompare(string word, string comparator)
{
	int lenA;
	int lenB;

	bool gotamatch = false;

	lenA = word.length() - 1;
	lenB = comparator.length();

	if(lenA == lenB)

	{
		gotamatch = true;

		for(int i=0; i<lenA; i++)
		{

			if((comparator.at(i) != '#') && (word.at(i) != comparator.at(i)))
			{
				gotamatch = false;
			}
		}
	}

	for(int i=0; i<20 ; i++)
	{
		if(word == usedwords[i])
			gotamatch = false;
	}

	if(gotamatch)
	{
		WordSubList[wordsinsublist] = word;
		wordsinsublist++;
	}
}

// Retrieves the length of the array
// containing the temporary word lsit.
int WordGrid::Lengthofsublist()
{
	int len = 0;

	for(int i=0;i<50;i++)
	{
		if(WordSubList[i]=="###")
		{
			i = 50;
		} else {
			len++;
		}
	}
	return len;
}

void WordGrid::FillTheGrid()
{
	char alph [26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q', 'R','S','T','U','V','W','X','Y','Z'};

	for(int i = 0; i < maxwordsize ; i++) {
		for(int j = 0; j < maxwordsize; j++) {
			if(grid[(i*maxwordsize)+j] == '#')
				grid[(i*maxwordsize)+j] = alph[rand() % 26];
		}
	}
}

int WordGrid::TrueModulo(int x, int y)
{
	if(x < 0)
		return (y + x) % y;
	else
		return x % y;
}

// Creates and returnes a copy of the object

WordGrid WordGrid::operator=(const WordGrid& rhs)
{
	WordGrid clonedwordgrid;
	clonedwordgrid.grid = grid;
	for(int i=0; i<20; i++)
		clonedwordgrid.wlist[i] = wlist[i];
	clonedwordgrid.count = count;
	return clonedwordgrid;
}

string WordGrid::CustomShapes(int length)
{
	int dir;	//direction of the shape;
   int type; //type of the shape;

	char directions[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};

	dir = rand() % 4;
	type = rand() % 3;

	string shapeCS = "";

	switch(length)
	{
		case 5:
			switch(type)
			{
				case 0:
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					break;
				case 1:
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					break;
				case 2:
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					break;
			}
			break;
		case 6:
			switch(type)
			{
				case 0:
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					break;
				case 1:
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					break;
				case 2:
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					break;
			}
			break;
		case 7:
			switch(type)
			{
				case 0:
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					break;
				case 1:
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					break;
				case 2:
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					break;
			}
			break;
		case 8:
			switch(type)
			{
				case 0:
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					break;
				case 1:
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					break;
				case 2:
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					break;
			}
			break;
		case 9:
			switch(type)
			{
				case 0:
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					break;
				case 1:
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					break;
				case 2:
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					break;
			}
			break;
		case 10:
			switch(type)
			{
				case 0:
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					break;
				case 1:
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					break;
				case 2:
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					break;
			}
			break;
		case 11:
			switch(type)
			{
				case 0:
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					break;
				case 1:
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					break;
				case 2:
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					break;
			}
			break;
		case 12:
			switch(type)
			{
				case 0:
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					break;
				case 1:
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					break;
				case 2:
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(7 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(7 + dir*2) % 8];
					shapeCS += directions[(7 + dir*2) % 8];
					break;
			}
			break;
		case 13:
			switch(type)
			{
				case 0:
					shapeCS += directions[(5 + dir*2) % 8];
					shapeCS += directions[(5 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(7 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					break;
				case 1:
					shapeCS += directions[(5 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(7 + dir*2) % 8];
					shapeCS += directions[(7 + dir*2) % 8];
					shapeCS += directions[(7 + dir*2) % 8];
					shapeCS += directions[(7 + dir*2) % 8];
					shapeCS += directions[(5 + dir*2) % 8];
					shapeCS += directions[(5 + dir*2) % 8];
					break;
				case 2:
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(6 + dir*2) % 8];
					shapeCS += directions[(6 + dir*2) % 8];
					shapeCS += directions[(6 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					break;
			}
			break;
		case 14:
			switch(type)
			{
				case 0:
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					break;
				case 1:
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(6 + dir*2) % 8];
					shapeCS += directions[(6 + dir*2) % 8];
					shapeCS += directions[(6 + dir*2) % 8];
					shapeCS += directions[(5 + dir*2) % 8];
					break;
				case 2:
					shapeCS += directions[(6 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					break;
			}
			break;
		case 15:
			switch(type)
			{
				case 0:
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					break;
				case 1:
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(4 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					break;
				case 2:
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(0 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(2 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(3 + dir*2) % 8];
					shapeCS += directions[(1 + dir*2) % 8];
					break;
			}
			break;
		default:
			break;
	}

	return shapeCS;
}
