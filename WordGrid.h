#include "Word.h"

#ifndef WORDGRID_H
#define WORDGRID_H

class WordGrid
{
	private:
		// Determines the category of words used.
		std::string cat;
		// Determines the difficulty option.
		int dif;

		// Specifies the grid size.
		int gridsize;

		// The number of words in this grid.
		int wordnum;

		// The biggest word that can fit into the board.
		// Also acts as a board side length measure.
		int maxwordsize;

		// Used by our code to fetch the particular
		// file that matches the difficulty and category.
		std::string wordlistfile;

		// Self explanatory, the words we have already placed
		// on the board will be put here, so that we can
		// make sure we don't have any repeats.
		std::string usedwords[20];

		/*
			This list holds the words that match our pick
			criteria of the current run of the word generator.
 			That means if we are looking; for example, for a
			5 letter word with 'E' as it's second letter, it
			will put all of the words that match into this array.
			and increment the int below per each word found.
			Once we use the word we will empty the array.
		*/
		std::string WordSubList[100];
		int wordsinsublist;
	
	public:
		// Counts the number of words added.
		int count;

		char* grid;
		Word wlist[20]; // the size is arbitrary	
		WordGrid();
		WordGrid(int x, int y, std::string category, int difficulty);

		// To be reviewed and set to private
		void AddWord(Word aword);
		void GridBuilder();
		void GetWordListFile();
		std::string GetWordShape(int type, int length);
		bool IsLegal(std::string shape, int startpoint, int shapetyp);
		int CoordFromShape(std::string shape, int startpoint, int position);
		void WordCompare(std::string word, std::string comparator);
		int Lengthofsublist();
		void FillTheGrid();
		int TrueModulo(int x, int y);
		WordGrid operator=(const WordGrid& rhs);
		std::string CustomShapes(int length);
};

#endif
