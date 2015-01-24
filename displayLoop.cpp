/*
 * displayLoop.cpp
 *
 *  Created on: May 2, 2014
 *      Author: jkrich
 */



//#include <GL/glut.h>
//#include <string>
//#include <iostream>
#include "DisplayGUI.hpp"

using namespace std;



int screenWidth = 800;
int screenHeight = 600;
int startCounter = 0;
int xsize = 15;
int xsizeTemp = 15;
int ysize = 15;
int ysizeTemp = 15;
WordGrid currentGrid;
int wordCount = 0;
int wordsFound = 0;
int currentPage = 1;
int currentLevel = 3;
char currentPlayer[] = "INT";
int highScore = 0;
string currentScore = "";
int scoreint = 0;
string score;
string currentTheme = randomz();
string currentWords[] = {"Word 1", "Word 2", "Word 3", "Word 4"};
float xLeft = 0.2 ;
float xRight = 0.8 ;
float yTop = 0.96;
float yBottom = 0.16 ;
string gridArray = "";
vector<wordTagged> gridLetters;
vector<wordTagged*> activeVector;
vector<completedWords> wordsList;
string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.< +";

GLint gridWidth = screenWidth * xRight - screenWidth * xLeft , gridHeight = screenHeight * yTop - screenHeight * yBottom, ix, iy;
int nextLetter;

int gridSizeX, gridSizeY;

// Stores the total seconds of the timer
int totSecs=120;

// Flag to pause/restart the timer
bool isPaused=false;

// Variables to calculate hours, minutes, seconds from total seconds
int h,m,s;

void Draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	screenWidth = glutGet(GLUT_WINDOW_WIDTH);
	screenHeight = glutGet(GLUT_WINDOW_HEIGHT);
	drawBackground();
	renderCurrentPage();
	glutSwapBuffers();
}

void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void renderCurrentPage()
{
	if (currentPage == 1)
	{
		start();
	}
	else if (currentPage == 2)
	{
		difficulty();
	}
	else if (currentPage == 3)
	{
		name();
	}
	else if (currentPage ==5)
	{
		showHelp();
	}
	else if (currentPage == 6)
	{
		gameOver();
	}
	else
	{
		if (startCounter == 0)
		{
			activeVector.clear();
			gridLetters.clear();
			wordsList.clear();
			//int lvl = (int) currentLevel[0] - 48;
			cout << currentLevel;
			WordGrid griddata(xsize, ysize, currentTheme, currentLevel);
			//WordGrid griddata(xsize, ysize, "candy", 1);
			gridArray = griddata.grid;
			for(int i = 0; i < xsize; i++)
			{
				for(int j = 0; j < xsize; j++)
				{
					cout << griddata.grid[(xsize*i)+j] << ' ';
				}
				cout << "\n";
			}


			for(int j=0; j<griddata.count; j++)
			{
				int len = griddata.wlist[j].length;
				int loc = griddata.wlist[j].location;
				string shp = griddata.wlist[j].shape;
				string wordput = "";
				cout << "--------------\n";
				cout << "Word location: " << loc << "\n";
				cout << "Word shape: " << shp << "\n";
				cout << "Word length: " << len << "\n";
				cout << "Word points: " << griddata.wlist[j].points << "\n";
				for(int k=0; k<len; k++)
				{
					wordput += griddata.grid[griddata.CoordFromShape(shp, loc, k)];
				}
				cout << "The Word: " << wordput << "\n";
				completedWords temp;
				temp.word = wordput;
				temp.complete = false;
				wordsList.push_back(temp);
				//wordsList[j].word = wordput;
			}
			cout << griddata.count << "\n";
			for(int l=0; l<xsize*ysize; l++)
				cout << griddata.grid[l];
			cout << "\n";
			wordCount = griddata.count;
			startCounter++;
			for (int k = 0 ; k < gridArray.length(); k++)
				{
				wordTagged tempWord;
				tempWord.letter = gridArray[k];
				tempWord.finished = false;
				tempWord.active = false;
				tempWord.wordEnd = false;
				tempWord.ready = false;
				tempWord.next = 0;
				tempWord.word = 0;
				gridLetters.push_back(tempWord);
				}
			for(int j=0; j<griddata.count; j++)
			{
				int len = griddata.wlist[j].length;
				int loc = griddata.wlist[j].location;
				string shp = griddata.wlist[j].shape;
				int pnts = griddata.wlist[j].points;

				for(int k=0; k<len; k++)
				{
					int m = griddata.CoordFromShape(shp, loc, k);

					if (k < len - 1)
					{
					int n = griddata.CoordFromShape(shp, loc, k + 1);
					gridLetters[m].next = n;
					gridLetters[m].word = j;
					}
					else
					{
						gridLetters[m].wordEnd = true;
						gridLetters[m].word = j;
					}
					gridLetters[m].points = pnts;
					gridLetters[m].length = len;
					gridLetters[m].shape = shp;



				}

			}
			gameBoard(wordsList, gridLetters );
		}
		else
			gameBoard(wordsList, gridLetters);
	}
}

void renderBitmapString(float x, float y, string string)
{
	glColor3f(1.0,1.0,1.0);
    glRasterPos2f(x ,y);
    for (int i=0, len = string.length() ; i<len ; i++)
    {
    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

void renderBitmapStringR(float x, float y, string string)
{
	glColor3f(1.0, 0, 0);
    glRasterPos2f(x ,y);
    for (int i=0, len = string.length() ; i<len ; i++)
    {
    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

void renderBitmapStringB(float x, float y, string string)
{
	glColor3f(0, 0, 1.0);
    glRasterPos2f(x ,y);
    for (int i=0, len = string.length() ; i<len ; i++)
    {
    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

void start()
{
	drawWordSearch();
 	drawLevelButton();
 	drawInitialButton();
 	drawStartButton();
 	drawLevelBlockBackground();
 	drawPlayerIniBackground();
}

void difficulty()
{
	drawSelectDifficulty();
	drawEasyButton();
	drawModerateButton();
	drawHardButton();
	drawLevel1Background();
	drawLevel5Background();
	drawLevel10Background();
}

// Function to display the Help page
void showHelp()
{
	glColor3f(0.2, 0.2, 0.2);
	displayHelpText();
	drawDoneButton();
}

// Function to display the text on the Help Page
void displayHelpText()
{

		renderBitmapString(0.05, 0.95, "The letters are jumbled and placed on the grid.");
		renderBitmapString(0.05, 0.90, "The words to be found are displayed at the bottom of grid.");
		renderBitmapString(0.05, 0.85, "Stare at a letter to start carving path to find a word.");
		renderBitmapString(0.05, 0.80, "Move your eyes to trace the word in a particular direction.");
		renderBitmapString(0.05, 0.75, "The letter, you select, will be highlighted.");
		renderBitmapString(0.05, 0.70, "The word will be selected once you carve and finish the path properly.");
		renderBitmapString(0.05, 0.65, "Find words quickly to get extra points based on time.");
		renderBitmapString(0.05, 0.60, "Place your eyes in the rest area to pause the game and the timer.");
		renderBitmapString(0.05, 0.55, "Your score will be displayed on the right side of the grid.");
		renderBitmapString(0.05, 0.50, "The words are arranged in a variety of shapes. Some are -");
		glBegin(GL_LINES);
		{
			glVertex2d(0.10,0.45);
			glVertex2d(0.10,0.35);
		}
		glEnd();
		glBegin(GL_LINES);
			{
				glVertex2d(0.15,0.40);
				glVertex2d(0.25,0.40);
			}
		glEnd();
		glBegin(GL_LINES);
			{
				glVertex2d(0.30,0.45);
				glVertex2d(0.40,0.35);
			}
		glEnd();
		glBegin(GL_LINES);
			{
				glVertex2d(0.45,0.45);
				glVertex2d(0.45,0.35);
			}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex2d(0.45,0.35);
			glVertex2d(0.55,0.35);
		}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex2d(0.60,0.45);
			glVertex2d(0.65,0.40);
		}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex2d(0.65,0.40);
			glVertex2d(0.70,0.45);
		}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex2d(0.70,0.45);
			glVertex2d(0.75,0.40);
		}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex2d(0.80,0.45);
			glVertex2d(0.80,0.35);
		}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex2d(0.80,0.35);
			glVertex2d(0.85,0.35);
		}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex2d(0.85,0.35);
			glVertex2d(0.85,0.45);
		}
		glEnd();
		renderBitmapString(0.05, 0.25, "GOOD LUCK!!");

}

// Function to display the Done button on the Help page. This button redirects user to game page
void drawDoneButton()
{
	glColor3f(0.6, 0.63, 0.9);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.30, 0.22, 0);
		glVertex3f(0.70, 0.22, 0);
		glVertex3f(0.69, 0.20, 0);
		glVertex3f(0.31, 0.20, 0);
	}
	glEnd();
	glColor3f(0.5, 0.55, 0.9);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.30, 0.22, 0);
		glVertex3f(0.31, 0.20, 0);
		glVertex3f(0.31, 0.05, 0);
		glVertex3f(0.30, 0.03, 0);
	}
	glEnd();
	glColor3f(0.25, 0.25, 0.8);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.69, 0.20, 0);
		glVertex3f(0.70, 0.22, 0);
		glVertex3f(0.70, 0.03, 0);
		glVertex3f(0.69, 0.05, 0);
	}
	glEnd();
	glColor3f(0.2, 0.2, 0.7);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.31, 0.05, 0);
		glVertex3f(0.69, 0.05, 0);
		glVertex3f(0.70, 0.03, 0);
		glVertex3f(0.30, 0.03, 0);
	}
	glEnd();
	glColor3f(0.36, 0.4, 0.9);
	glBegin(GL_POLYGON);
	{
		glVertex3f(0.31, 0.20, 0);
		glVertex3f(0.69, 0.20, 0);
		glVertex3f(0.69, 0.05, 0);
		glVertex3f(0.31, 0.05, 0);
	}
	glEnd();
	renderBitmapString(0.45, 0.13, "Done");
}

void gameOver(){
	drawGameOverBackground();
	drawHighScoreBackground();
	drawPlayerScoreBackground();
	drawContinueButton();
	drawEndGameButton();
}

void name()
{
	vector<wordTagged> alphaTag (30);
	for (int k = 0 ; k < alphabet.length() ; k++)
	{
		alphaTag[k].letter = alphabet[k];
		cout << alphaTag[k].letter;
	}
	xsizeTemp = xsize;
	ysizeTemp = ysize;
	xsize = 5;
	ysize = 6;
	gameGrid(xsize, ysize, alphaTag);
	drawLongBlockBackground();
  	drawRestArea();
}



void gameBoard(vector<completedWords> wordsList, vector<wordTagged> gridLetters)
{
    //converting int to string
	ostringstream conv;
	conv << scoreint;
	score = conv.str();
	currentScore = score;

	gameGrid(xsize, ysize, gridLetters);
	wordList(wordsList);
	//themeItems(theme)
	drawRestArea();
	drawCurrentLevelBackground();
	drawTimerBackground();
	drawScoreBoardBackground();
	drawThemeBackground();
	drawHelpBackground();
}

void gameGrid(float xSize, float ySize, vector<wordTagged> tagged)
{
	int gridCounter = 0;
	int gridSize = xSize * ySize;
	float xStep  = (xRight - xLeft) / xSize; //.12
	float yStep  = (yTop - yBottom) / ySize; //.14
	float x1 = xLeft ; //.2
	float x2 = x1 + xStep; // .32
	float y1 = yTop; //.95
	float y2 = y1 - yStep;  //.81

// 			using copyArray (new array with inuse and complete flags)
/*	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
			{
				glVertex3f(x1, y1, 0);
				glVertex3f(x2, y1, 0);
				glVertex3f(x2, y2, 0);
				glVertex3f(x1, y2, 0);
			}
			glEnd();

			glColor3f(0, 0, 1);
			glBegin(GL_POLYGON);
					{
						glVertex3f(0.2, 0.95, 0);
						glVertex3f(0.32, 0.95, 0);
						glVertex3f(0.32, 0.81, 0);
						glVertex3f(0.2, 0.81, 0);
					}
					glEnd();
}
*/
	for ( int i = 0 ; i < gridSize; i++)
	{
		if (gridCounter == xSize - 1)
		{
				gridCounter = 0;
				drawBlock(x1, x2, y1, y2);
				if (tagged[i].finished == true)
				{
					letterBlockB(x1, x2, y1, y2, tagged[i].letter);
				}
					else if (tagged[i].active == true)
				{
					letterBlockR(x1, x2, y1, y2, tagged[i].letter);
				}
					else
				{
				letterBlock(x1, x2, y1, y2, tagged[i].letter);
				}
				y1 = y1 - yStep;
				y2 = y2 - yStep;
				x1 = xLeft;
				x2 = xLeft + xStep;
		}
		else
		{
				drawBlock(x1, x2, y1, y2);
				if (tagged[i].finished == true)
						{
							letterBlockB(x1, x2, y1, y2, tagged[i].letter);
						}
							else if (tagged[i].active == true)
						{
							letterBlockR(x1, x2, y1, y2, tagged[i].letter);
						}
							else
						{
						letterBlock(x1, x2, y1, y2, tagged[i].letter);
						}
				x1 = x1 + xStep;
				x2 = x2 + xStep;
				gridCounter++;
		}
	}
}

void drawBlock(float x1, float x2, float y1, float y2)
{
	glColor3f(.35, .35, .35);
	glBegin(GL_POLYGON);
	{
		glVertex3f(x1, y1, 0);
		glVertex3f(x2, y1, 0);
		glVertex3f(x2, y2, 0);
		glVertex3f(x1, y2, 0);
	}
	glEnd();
}
//temporary letters using font until drawletter is done

void letterBlock(float x1, float x2, float y1, float y2, char letter)
{
	float x = (x1 + x2) / 2;
	float y = (y1 + y2) / 2;
	string letterString;
	letterString = letter;
	renderBitmapString(x, y, letterString);

	float xScaler = (x2 - x1) / 10;
	float yScaler = (y1 - y2 ) / 10;

	float a1 = x1 + xScaler;
	float a2 = x1 + 5 * xScaler;
	float a3 = x1 + 9 * xScaler;
	float b1 = y1 - 9 * yScaler;
	float b2 = y1 - yScaler;
	float b3 = y1 - 9 * yScaler;
	//glColor3f(0, 0, 1);
	//glBegin(GL_POLYGON);
	//		{
	//			glVertex3f(a1, b1, 0);
	//			glVertex3f(a2, b2, 0);
				//glVertex3f(a2, b2, 0);
	//			glVertex3f(a3, b3, 0);
	//		}
	//		glEnd();
}

void letterBlockR(float x1, float x2, float y1, float y2, char letter)
{
	float x = (x1 + x2) / 2;
	float y = (y1 + y2) / 2;
	string letterString;
	letterString = letter;
	renderBitmapStringR(x, y, letterString);
}

void letterBlockB(float x1, float x2, float y1, float y2, char letter)
{
	float x = (x1 + x2) / 2;
	float y = (y1 + y2) / 2;
	string letterString;
	letterString = letter;
	renderBitmapStringB(x, y, letterString);
}

/*		void drawblock(blocksize, atblockspace)
* 			draw block()
*
* 		void letterblock(size, space, copyLetterObject)
* 			if (object.letter == 'A')
* 				if (object.inuse == true)
* 					drawLetterA(size, space, red)
* 				else if (object.complete == true)
* 					drawLetterA(size, space, blue)
* 				else drawLetter(size, space, black)
* 			else if (object.letter == 'B')
* 				...
* 				...
* 				...
*/

void wordList(vector<completedWords> wordsList)
{
	drawLongBlockBackground();
	float x = .21;
	float y = .09;
	for (int j = 0 ; j <  wordCount ; j++)
	{
		if ( x < .7)
		{
		if (wordsList[j].complete == true)
		{renderBitmapStringB(x, y, wordsList[j].word);}
		else
		{renderBitmapString(x, y, wordsList[j].word);}
		x = x + .15;
		}
		else
		{
			y = y - 0.05;
			x = 0.21;
			if (wordsList[j].complete == true)
			{renderBitmapStringB(x, y, wordsList[j].word);}
			else
			{renderBitmapString(x, y, wordsList[j].word);}
			x = x + .15;
		}
	}
}

void drawWordSearch() 
{
	glColor3f(0.6, 0.63, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.20, 0.95, 0);
				glVertex3f(0.80, 0.95, 0);
				glVertex3f(0.79, 0.93, 0);
				glVertex3f(0.21, 0.93, 0);
			}
			glEnd();
	glColor3f(0.5, 0.55, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.20, 0.95, 0);
				glVertex3f(0.21, 0.93, 0);
				glVertex3f(0.21, 0.67, 0);
				glVertex3f(0.20, 0.65, 0);
			}
			glEnd();
	glColor3f(0.25, 0.25, 0.8);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.79, 0.93, 0);
				glVertex3f(0.80, 0.95, 0);
				glVertex3f(0.80, 0.65, 0);
				glVertex3f(0.79, 0.67, 0);
			}
			glEnd();
	glColor3f(0.2, 0.2, 0.7);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.21, 0.67, 0);
				glVertex3f(0.79, 0.67, 0);
				glVertex3f(0.80, 0.65, 0);
				glVertex3f(0.20, 0.65, 0);
			}
			glEnd();
	glColor3f(0.36, 0.4, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.21, 0.93, 0);
				glVertex3f(0.79, 0.93, 0);
				glVertex3f(0.79, 0.67, 0);
				glVertex3f(0.21, 0.67, 0);
			}
			glEnd();
			renderBitmapString(0.45, 0.90, "Word Search");//still needs string word search
}

void drawSelectDifficulty()
{
	glColor3f(0.6, 0.63, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.20, 0.95, 0);
				glVertex3f(0.80, 0.95, 0);
				glVertex3f(0.79, 0.93, 0);
				glVertex3f(0.21, 0.93, 0);
			}
			glEnd();
	glColor3f(0.5, 0.55, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.20, 0.95, 0);
				glVertex3f(0.21, 0.93, 0);
				glVertex3f(0.21, 0.67, 0);
				glVertex3f(0.20, 0.65, 0);
			}
			glEnd();
	glColor3f(0.25, 0.25, 0.8);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.79, 0.93, 0);
				glVertex3f(0.80, 0.95, 0);
				glVertex3f(0.80, 0.65, 0);
				glVertex3f(0.79, 0.67, 0);
			}
			glEnd();
	glColor3f(0.2, 0.2, 0.7);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.21, 0.67, 0);
				glVertex3f(0.79, 0.67, 0);
				glVertex3f(0.80, 0.65, 0);
				glVertex3f(0.20, 0.65, 0);
			}
			glEnd();
 	glColor3f(0.36, 0.4, 0.9);
 			glBegin(GL_POLYGON);
 			{
 				glVertex3f(0.21, 0.93, 0);
 				glVertex3f(0.79, 0.93, 0);
 				glVertex3f(0.79, 0.67, 0);
 				glVertex3f(0.21, 0.67, 0);
 			}
 			glEnd();
 			renderBitmapString(0.45, 0.90, "Select Difficulty");//still needs string select difficulty
 }

void drawLevelButton() 
{
	glColor3f(0.6, 0.63, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.30, 0.62, 0);
				glVertex3f(0.70, 0.62, 0);
				glVertex3f(0.69, 0.60, 0);
				glVertex3f(0.31, 0.60, 0);
			}
			glEnd();
	glColor3f(0.5, 0.55, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.30, 0.62, 0);
				glVertex3f(0.31, 0.60, 0);
				glVertex3f(0.31, 0.45, 0);
				glVertex3f(0.30, 0.43, 0);
			}
			glEnd();
	glColor3f(0.25, 0.25, 0.8);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.69, 0.60, 0);
				glVertex3f(0.70, 0.62, 0);
				glVertex3f(0.70, 0.43, 0);
				glVertex3f(0.69, 0.45, 0);
			}
			glEnd();
	glColor3f(0.2, 0.2, 0.7);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.31, 0.45, 0);
				glVertex3f(0.69, 0.45, 0);
				glVertex3f(0.70, 0.43, 0);
				glVertex3f(0.30, 0.43, 0);
			}
			glEnd();
	glColor3f(0.36, 0.4, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.31, 0.60, 0);
			 	glVertex3f(0.69, 0.60, 0);
			 	glVertex3f(0.69, 0.45, 0);
			 	glVertex3f(0.31, 0.45, 0);
			}
			glEnd();
			renderBitmapString(0.45, 0.53, "Select Level");//still needs string select level
}

void drawInitialButton() {
	glColor3f(0.6, 0.63, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.30, 0.42, 0);
				glVertex3f(0.70, 0.42, 0);
				glVertex3f(0.69, 0.40, 0);
				glVertex3f(0.31, 0.40, 0);
			}
			glEnd();
	glColor3f(0.5, 0.55, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.30, 0.42, 0);
				glVertex3f(0.31, 0.40, 0);
				glVertex3f(0.31, 0.25, 0);
				glVertex3f(0.30, 0.23, 0);
			}
			glEnd();
	glColor3f(0.25, 0.25, 0.8);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.69, 0.40, 0);
				glVertex3f(0.70, 0.42, 0);
				glVertex3f(0.70, 0.23, 0);
				glVertex3f(0.69, 0.25, 0);
			}
			glEnd();
	glColor3f(0.2, 0.2, 0.7);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.31, 0.25, 0);
				glVertex3f(0.69, 0.25, 0);
				glVertex3f(0.70, 0.23, 0);
				glVertex3f(0.30, 0.23, 0);
			}
			glEnd();
	glColor3f(0.36, 0.4, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.31, 0.40, 0);
				glVertex3f(0.69, 0.40, 0);
				glVertex3f(0.69, 0.25, 0);
				glVertex3f(0.31, 0.25, 0);
			}
			glEnd();
			renderBitmapString(0.45, 0.33, "Enter Initials"); //still needs string enter initials
}

void drawStartButton() {
	glColor3f(0.6, 0.63, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.30, 0.22, 0);
				glVertex3f(0.70, 0.22, 0);
				glVertex3f(0.69, 0.20, 0);
				glVertex3f(0.31, 0.20, 0);
			}
			glEnd();
	glColor3f(0.5, 0.55, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.30, 0.22, 0);
				glVertex3f(0.31, 0.20, 0);
				glVertex3f(0.31, 0.05, 0);
				glVertex3f(0.30, 0.03, 0);
			}
			glEnd();
	glColor3f(0.25, 0.25, 0.8);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.69, 0.20, 0);
				glVertex3f(0.70, 0.22, 0);
				glVertex3f(0.70, 0.03, 0);
				glVertex3f(0.69, 0.05, 0);
			}
			glEnd();
	glColor3f(0.2, 0.2, 0.7);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.31, 0.05, 0);
				glVertex3f(0.69, 0.05, 0);
				glVertex3f(0.70, 0.03, 0);
				glVertex3f(0.30, 0.03, 0);
			}
			glEnd();
	glColor3f(0.36, 0.4, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.31, 0.20, 0);
				glVertex3f(0.69, 0.20, 0);
				glVertex3f(0.69, 0.05, 0);
				glVertex3f(0.31, 0.05, 0);
			}
			glEnd();
			renderBitmapString(0.45, 0.13, "Start");//still needs string START
}

void drawEasyButton() {
	glColor3f(0.6, 0.63, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.30, 0.62, 0);
				glVertex3f(0.70, 0.62, 0);
				glVertex3f(0.69, 0.60, 0);
				glVertex3f(0.31, 0.60, 0);
			}
			glEnd();
	glColor3f(0.5, 0.55, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.30, 0.62, 0);
				glVertex3f(0.31, 0.60, 0);
				glVertex3f(0.31, 0.45, 0);
				glVertex3f(0.30, 0.43, 0);
			}
			glEnd();
	glColor3f(0.25, 0.25, 0.8);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.69, 0.60, 0);
				glVertex3f(0.70, 0.62, 0);
				glVertex3f(0.70, 0.43, 0);
				glVertex3f(0.69, 0.45, 0);
			}
			glEnd();
	glColor3f(0.2, 0.2, 0.7);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.31, 0.45, 0);
				glVertex3f(0.69, 0.45, 0);
				glVertex3f(0.70, 0.43, 0);
				glVertex3f(0.30, 0.43, 0);
			}
			glEnd();
	glColor3f(0.36, 0.4, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.31, 0.60, 0);
			 	glVertex3f(0.69, 0.60, 0);
			 	glVertex3f(0.69, 0.45, 0);
			 	glVertex3f(0.31, 0.45, 0);
			}
			glEnd();
			renderBitmapString(0.45, 0.53, "Easy");//still needs string easy
}

void drawModerateButton() {
	glColor3f(0.6, 0.63, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.30, 0.42, 0);
				glVertex3f(0.70, 0.42, 0);
				glVertex3f(0.69, 0.40, 0);
				glVertex3f(0.31, 0.40, 0);
			}
			glEnd();
	glColor3f(0.5, 0.55, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.30, 0.42, 0);
				glVertex3f(0.31, 0.40, 0);
				glVertex3f(0.31, 0.25, 0);
				glVertex3f(0.30, 0.23, 0);
			}
			glEnd();
	glColor3f(0.25, 0.25, 0.8);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.69, 0.40, 0);
				glVertex3f(0.70, 0.42, 0);
				glVertex3f(0.70, 0.23, 0);
				glVertex3f(0.69, 0.25, 0);
			}
			glEnd();
	glColor3f(0.2, 0.2, 0.7);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.31, 0.25, 0);
				glVertex3f(0.69, 0.25, 0);
				glVertex3f(0.70, 0.23, 0);
				glVertex3f(0.30, 0.23, 0);
			}
			glEnd();
	glColor3f(0.36, 0.4, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.31, 0.40, 0);
				glVertex3f(0.69, 0.40, 0);
				glVertex3f(0.69, 0.25, 0);
				glVertex3f(0.31, 0.25, 0);
			}
			glEnd();
			renderBitmapString(0.45, 0.33, "Moderate");//still needs string moderate
}

void drawHardButton() {
	glColor3f(0.6, 0.63, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.30, 0.22, 0);
				glVertex3f(0.70, 0.22, 0);
				glVertex3f(0.69, 0.20, 0);
				glVertex3f(0.31, 0.20, 0);
			}
			glEnd();
	glColor3f(0.5, 0.55, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.30, 0.22, 0);
				glVertex3f(0.31, 0.20, 0);
				glVertex3f(0.31, 0.05, 0);
				glVertex3f(0.30, 0.03, 0);
			}
			glEnd();
	glColor3f(0.25, 0.25, 0.8);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.69, 0.20, 0);
				glVertex3f(0.70, 0.22, 0);
				glVertex3f(0.70, 0.03, 0);
				glVertex3f(0.69, 0.05, 0);
			}
			glEnd();
	glColor3f(0.2, 0.2, 0.7);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.31, 0.05, 0);
				glVertex3f(0.69, 0.05, 0);
				glVertex3f(0.70, 0.03, 0);
				glVertex3f(0.30, 0.03, 0);
			}
			glEnd();
	glColor3f(0.36, 0.4, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.31, 0.20, 0);
				glVertex3f(0.69, 0.20, 0);
				glVertex3f(0.69, 0.05, 0);
				glVertex3f(0.31, 0.05, 0);
			}
			glEnd();
			renderBitmapString(0.45, 0.13, "HARD");//still needs string hard
}

void drawLevelBlockBackground() {
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.805, 0.59, 0);
			glVertex3f(0.955, 0.59, 0);
			glVertex3f(0.955, 0.44, 0);
			glVertex3f(0.805, 0.44, 0);
		}
		glEnd();
	glColor3f(0.5, 0.9, 0.75);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.80, 0.60, 0);
			glVertex3f(0.95, 0.60, 0);
			glVertex3f(0.95, 0.45, 0);
			glVertex3f(0.80, 0.45, 0);
		}
		glEnd();
		string level;
		ostringstream conv;
		conv << currentLevel;
		level = conv.str();

		renderBitmapString(0.85, 0.55, "Level:");
		renderBitmapString(0.85, 0.50, level);//still needs string level: 0
}

void drawPlayerIniBackground() {
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.805, 0.39, 0);
			glVertex3f(0.955, 0.39, 0);
			glVertex3f(0.955, 0.24, 0);
			glVertex3f(0.805, 0.24, 0);
		}
		glEnd();
	glColor3f(0.5, 0.9, 0.75);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.80, 0.40, 0);
			glVertex3f(0.95, 0.40, 0);
			glVertex3f(0.95, 0.25, 0);
			glVertex3f(0.80, 0.25, 0);
		}
		glEnd();
		renderBitmapString(0.85, 0.35, "Player:");
		renderBitmapString(0.85, 0.30, currentPlayer);//still needs string player: initial
}

void drawLevel1Background() {
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.805, 0.59, 0);
			glVertex3f(0.955, 0.59, 0);
			glVertex3f(0.955, 0.44, 0);
			glVertex3f(0.805, 0.44, 0);
		}
		glEnd();
	glColor3f(0.5, 0.9, 0.75);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.80, 0.60, 0);
			glVertex3f(0.95, 0.60, 0);
			glVertex3f(0.95, 0.45, 0);
			glVertex3f(0.80, 0.45, 0);
		}
		glEnd();
		renderBitmapString(0.85, 0.55, "Level:");
		renderBitmapString(0.85, 0.50, "1");//still needs string level: 1
}

void drawLevel5Background() {
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.805, 0.39, 0);
			glVertex3f(0.955, 0.39, 0);
			glVertex3f(0.955, 0.24, 0);
			glVertex3f(0.805, 0.24, 0);
		}
		glEnd();
	glColor3f(0.5, 0.9, 0.75);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.80, 0.40, 0);
			glVertex3f(0.95, 0.40, 0);
			glVertex3f(0.95, 0.25, 0);
			glVertex3f(0.80, 0.25, 0);
		}
		glEnd();
		renderBitmapString(0.85, 0.35, "Level:");
		renderBitmapString(0.85, 0.30, "5");//still needs string level: 5
}

void drawLevel10Background() {
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.805, 0.19, 0);
			glVertex3f(0.955, 0.19, 0);
			glVertex3f(0.955, 0.04, 0);
			glVertex3f(0.805, 0.04, 0);
		}
		glEnd();
	glColor3f(0.5, 0.9, 0.75);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.80, 0.20, 0);
			glVertex3f(0.95, 0.20, 0);
			glVertex3f(0.95, 0.05, 0);
			glVertex3f(0.80, 0.05, 0);
		}
		glEnd();
		renderBitmapString(0.85, 0.15, "Level:");
		renderBitmapString(0.85, 0.10, "10");//still needs string level: 10
}

void drawLongBlockBackground() {
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.205, 0.14, 0);
			glVertex3f(0.805, 0.14, 0);
			glVertex3f(0.805, 0.01, 0);
			glVertex3f(0.205, 0.01, 0);
		}
		glEnd();
	glColor3f(0.5, 0.35, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.20, 0.15, 0);
			glVertex3f(0.80, 0.15, 0);
			glVertex3f(0.80, 0.02, 0);
			glVertex3f(0.20, 0.02, 0);
		}
		glEnd(); //still needs string for both name input page and playing page
}

void drawRestArea() {
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.025, 0.64, 0);
			glVertex3f(0.185, 0.64, 0);
			glVertex3f(0.185, 0.34, 0);
			glVertex3f(0.025, 0.34, 0);
		}
		glEnd();
	glColor3f(0.5, 0.35, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.02, 0.65, 0);
			glVertex3f(0.18, 0.65, 0);
			glVertex3f(0.18, 0.35, 0);
			glVertex3f(0.02, 0.35, 0);
		}
		glEnd();
		renderBitmapString(0.05, 0.60, "Rest Area:");
		renderBitmapString(0.05, 0.50, "Place");
		renderBitmapString(0.05, 0.45, "Pointer");
		renderBitmapString(0.05, 0.40, "Here");//still needs string place pointer here/ take a rest here
}

void drawCurrentLevelBackground() {
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.825, 0.97, 0);
			glVertex3f(0.985, 0.97, 0);
			glVertex3f(0.985, 0.77, 0);
			glVertex3f(0.825, 0.77, 0);
		}
		glEnd();
	glColor3f(0.5, 0.35, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.82, 0.98, 0);
			glVertex3f(0.98, 0.98, 0);
			glVertex3f(0.98, 0.78, 0);
			glVertex3f(0.82, 0.78, 0);
		}
		glEnd();
		string level;
		ostringstream conv;
		conv << currentLevel;
		level = conv.str();
//		currentScore = score;
		renderBitmapString(0.85, 0.93, "Level:");
		renderBitmapString(0.85, 0.87, level);
}

void drawTimerBackground()
{
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.825, 0.75, 0);
			glVertex3f(0.985, 0.75, 0);
			glVertex3f(0.985, 0.60, 0);
			glVertex3f(0.825, 0.60, 0);
		}
		glEnd();
	glColor3f(0.5, 0.35, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.82, 0.76, 0);
			glVertex3f(0.98, 0.76, 0);
			glVertex3f(0.98, 0.61, 0);
			glVertex3f(0.82, 0.61, 0);
		}
		glEnd();
		h=int(totSecs/3600);
		s=totSecs-(h*3600);
		m=int(s/60);
		s=s-(m*60);
		stringstream hh;
		hh<<h;
		stringstream mm;
		mm<<m;
		stringstream ss;
		ss<<s;
		string secs=/*hh.str()+"h"+":"+*/mm.str()+"m"+":"+ss.str()+"s";
		cout<<secs;
		renderBitmapString(0.88,0.70,"Time");
		renderBitmapString(0.85, 0.66, secs);
}

void pauseClock()
{
	isPaused=true;
}

void restartClock()
{
	isPaused=false;
	glutTimerFunc(1000, addSeconds, 0);
	glutPostRedisplay();
}

void addSeconds(int extra)
{
	if(!isPaused)
	{   if (totSecs == 0)
		{
			if (wordsFound > wordCount - 2)
			{
			startCounter = 0;
			currentLevel++;
			currentPage = 4;
			glutPostRedisplay();
			glutTimerFunc(1000, addSeconds, 0);
			}
			else{
			startCounter = 0;
			activeVector.clear();
			wordsList.clear();
			totSecs = 120;
			currentPage = 6;
			glutPostRedisplay();
			glutTimerFunc(1000, addSeconds, 0);
			}
}
	else{
		totSecs=totSecs -1;
		cout<<"Time:"<<totSecs;
		//glutSwapBuffers();
		glutPostRedisplay();
		glutTimerFunc(1000, addSeconds, 0);}
	}
}

void drawHelpBackground()
{
	glColor3f(0.35, 0.35, 0.35);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.985, 0.19, 0);
				glVertex3f(0.825, 0.19, 0);
				glVertex3f(0.825, 0.10, 0);
				glVertex3f(0.985, 0.10, 0);
			}
			glEnd();
		glColor3f(0.5, 0.35, 0.9);
			glBegin(GL_POLYGON);
			{
				glVertex3f(0.82, 0.20, 0);
				glVertex3f(0.98, 0.20, 0);
				glVertex3f(0.98, 0.11, 0);
				glVertex3f(0.82, 0.11, 0);
			}
			glEnd();
			renderBitmapString(0.86, 0.15, "Help");
}

void drawScoreBoardBackground()
{
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.825, 0.58, 0);
			glVertex3f(0.985, 0.58, 0);
			glVertex3f(0.985, 0.21, 0);
			glVertex3f(0.825, 0.21, 0);
		}
		glEnd();
	glColor3f(0.5, 0.35, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.82, 0.59, 0);
			glVertex3f(0.98, 0.59, 0);
			glVertex3f(0.98, 0.22, 0);
			glVertex3f(0.82, 0.22, 0);
		}
		glEnd();
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.83, 0.41, 0);
			glVertex3f(0.97, 0.41, 0);
			glVertex3f(0.97, 0.40, 0);
			glVertex3f(0.83, 0.40, 0);
		}
		glEnd();
		string high;
		ostringstream conv;
		conv << highScore;
		high = conv.str();
		renderBitmapString(0.85, 0.53, "High Score");
		renderBitmapString(0.85, 0.47, high);
		renderBitmapString(0.85, 0.36, "Your Score:");
		renderBitmapString(0.85, 0.31, currentScore);//high score on top and initial score on the bottom
}

void drawThemeBackground() {
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.025, 0.29, 0);
			glVertex3f(0.185, 0.29, 0);
			glVertex3f(0.185, 0.01, 0);
			glVertex3f(0.025, 0.01, 0);
		}
		glEnd();
	glColor3f(0.5, 0.35, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.02, 0.30, 0);
			glVertex3f(0.18, 0.30, 0);
			glVertex3f(0.18, 0.02, 0);
			glVertex3f(0.02, 0.02, 0);
		}
		glEnd();
		renderBitmapString(0.05, 0.25, "Theme:");
		renderBitmapString(0.05, 0.10, currentTheme);//needs the string of whatever category the player is currently playing
}

void drawGameOverBackground() {
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.215, 0.92, 0);
			glVertex3f(0.795, 0.92, 0);
			glVertex3f(0.795, 0.66, 0);
			glVertex3f(0.215, 0.66, 0);
		}
		glEnd();
	glColor3f(0.5, 0.35, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.21, 0.93, 0);
			glVertex3f(0.79, 0.93, 0);
			glVertex3f(0.79, 0.67, 0);
			glVertex3f(0.21, 0.67, 0);
		}
		glEnd();
		renderBitmapString(0.45, 0.80, "GAME OVER");
}

void drawHighScoreBackground() {
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.185, 0.63, 0);
			glVertex3f(0.485, 0.63, 0);
			glVertex3f(0.485, 0.29, 0);
			glVertex3f(0.185, 0.29, 0);
		}
		glEnd();
	glColor3f(0.5, 0.9, 0.75);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.18, 0.64, 0);
			glVertex3f(0.48, 0.64, 0);
			glVertex3f(0.48, 0.30, 0);
			glVertex3f(0.18, 0.30, 0);
		}
		glEnd();
		string high;
		ostringstream conv;
		conv << highScore;
		high = conv.str();
		renderBitmapString(0.23, 0.59, "High Score:");
		renderBitmapString(0.23, 0.49, high);

}

void drawPlayerScoreBackground() {
	glColor3f(0.35, 0.35, 0.35);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.525, 0.63, 0);
			glVertex3f(0.825, 0.63, 0);
			glVertex3f(0.825, 0.29, 0);
			glVertex3f(0.525, 0.29, 0);
		}
		glEnd();
	glColor3f(0.5, 0.9, 0.75);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.52, 0.64, 0);
			glVertex3f(0.82, 0.64, 0);
			glVertex3f(0.82, 0.30, 0);
			glVertex3f(0.52, 0.30, 0);
		}
		glEnd();
		renderBitmapString(0.57, 0.59, "Your Score:");
		renderBitmapString(0.57, 0.49, currentScore);
}

void drawContinueButton() {
	glColor3f(0.6, 0.63, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.24, 0.27, 0);
			glVertex3f(0.25, 0.25, 0);
			glVertex3f(0.25, 0.08, 0);
			glVertex3f(0.24, 0.06, 0);
		}
		glEnd();
	glColor3f(0.5, 0.55, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.24, 0.27, 0);
			glVertex3f(0.46, 0.27, 0);
			glVertex3f(0.45, 0.25, 0);
			glVertex3f(0.25, 0.25, 0);
		}
		glEnd();
	glColor3f(0.25, 0.25, 0.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.45, 0.25, 0);
			glVertex3f(0.46, 0.27, 0);
			glVertex3f(0.46, 0.06, 0);
			glVertex3f(0.45, 0.08, 0);
		}
		glEnd();
	glColor3f(0.2, 0.2, 0.7);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.25, 0.08, 0);
			glVertex3f(0.45, 0.08, 0);
			glVertex3f(0.46, 0.06, 0);
			glVertex3f(0.24, 0.06, 0);
		}
		glEnd();
	glColor3f(0.36, 0.4, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.25, 0.25, 0);
			glVertex3f(0.45, 0.25, 0);
			glVertex3f(0.45, 0.08, 0);
			glVertex3f(0.25, 0.08, 0);
		}
		glEnd();
		renderBitmapString(0.30, 0.15, "Continue");
}

void drawEndGameButton() {
	glColor3f(0.6, 0.63, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.54, 0.27, 0);
			glVertex3f(0.55, 0.25, 0);
			glVertex3f(0.55, 0.08, 0);
			glVertex3f(0.54, 0.06, 0);
		}
		glEnd();
	glColor3f(0.5, 0.55, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.54, 0.27, 0);
			glVertex3f(0.76, 0.27, 0);
			glVertex3f(0.75, 0.25, 0);
			glVertex3f(0.55, 0.25, 0);
		}
		glEnd();
	glColor3f(0.25, 0.25, 0.8);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.75, 0.25, 0);
			glVertex3f(0.76, 0.27, 0);
			glVertex3f(0.76, 0.06, 0);
			glVertex3f(0.75, 0.08, 0);
		}
		glEnd();
	glColor3f(0.2, 0.2, 0.7);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.55, 0.08, 0);
			glVertex3f(0.75, 0.08, 0);
			glVertex3f(0.76, 0.06, 0);
			glVertex3f(0.54, 0.06, 0);
		}
		glEnd();
	glColor3f(0.36, 0.4, 0.9);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0.55, 0.25, 0);
			glVertex3f(0.75, 0.25, 0);
			glVertex3f(0.75, 0.08, 0);
			glVertex3f(0.55, 0.08, 0);
		}
		glEnd();
		renderBitmapString(0.60, 0.15, "End Game");
}

void drawBackground() {
	glColor3f(0.2, 0.2, 0.2);
		glBegin(GL_POLYGON);
		{
			glVertex3f(0, 0, 0.0);
			glVertex3f(1, 0, 0.0);
			glVertex3f(1, 1, 0.0);
			glVertex3f(0, 1, 0.0);
		}
		glEnd();
}

int main(int iArgc, char** cppArgv) {

	glutInit(&iArgc, cppArgv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("EYE GAME");
	Initialize();
	glutDisplayFunc(Draw);
	glutPassiveMotionFunc(myMouseMove); //added for mouse
	glutTimerFunc(1000, addSeconds, 0);
	glutMainLoop();
	return 0;
}

int tempX, tempY;
int xi, yi;
int timercounter = 0;

static void Timer(int value) {
timercounter += 1;
// 100 milliseconds
if (timercounter > 300 && timercounter <= 301)
{
	if (currentPage == 1){
		if (xi == tempX && yi == tempY && tempX >= screenWidth * 0.31 && tempX <= screenWidth * .69
				&& tempY >= screenHeight - screenHeight * .60 && tempY <= screenHeight - screenHeight * .45) // comparing the  current cursor(mouse pointer) co-ordinates with the co-ordinates of the button.
		{

			currentPage = 2;
		}
		/*if (xi == tempX && yi == tempY && tempX >= screenWidth * 0.31 && tempX <= screenWidth * .69
				&& tempY >= screenHeight - screenHeight * .40 && tempY <= screenHeight - screenHeight * .25)
		{

			currentPage = 3;
		}*/
		if (xi == tempX && yi == tempY && tempX >= screenWidth * 0.31 && tempX <= screenWidth * .69
				&& tempY >= screenHeight - screenHeight * .20 && tempY <= screenHeight - screenHeight * .05)
		{

			totSecs=120;
			currentPage = 4;
			
		}
	}
	else if (currentPage == 2){
		if (xi == tempX && yi == tempY && tempX >= screenWidth * 0.31 && tempX <= screenWidth * .69
				&& tempY >= screenHeight - screenHeight * .60 && tempY <= screenHeight - screenHeight * .45) // comparing the  current cursor(mouse pointer) co-ordinates with the co-ordinates of the button.
		{
			currentLevel = 3;
			xsize = 7;
			ysize = 7;

			currentPage = 1;
		}
		if (xi == tempX && yi == tempY && tempX >= screenWidth * 0.31 && tempX <= screenWidth * .69
				&& tempY >= screenHeight - screenHeight * .40 && tempY <= screenHeight - screenHeight * .25)
		{
			currentLevel = 4;
			xsize = 10;
			ysize = 10;
			currentPage = 1;
		}
		if (xi == tempX && yi == tempY && tempX >= screenWidth * 0.31 && tempX <= screenWidth * .69
				&& tempY >= screenHeight - screenHeight * .20 && tempY <= screenHeight - screenHeight * .05)
		{
			currentLevel = 5;
			xsize = 15;
			ysize = 15;
			currentPage = 1;
		}
	}
	else if (currentPage == 3){
		if (xi == tempX && yi == tempY && tempX >= screenWidth * 0.82 && tempX <= screenWidth * .98
				&& tempY >= screenHeight - screenHeight * .76 && tempY <= screenHeight - screenHeight * .61)
		{
			xsize = xsizeTemp;
			ysize = ysizeTemp;
			currentPage = 1;
		}
	}
	else if(currentPage==4){
		if(xi == tempX && yi == tempY && tempX >= screenWidth * 0.82 && tempX <= screenWidth * .98
				&& tempY >= screenHeight - screenHeight * .20 && tempY <= screenHeight - screenHeight * .11)
		{
			currentPage=5;
			pauseClock();
		}
		else if(xi==tempX && yi==tempY && tempX >= screenWidth * xLeft && tempX <= screenWidth * xRight
				&& tempY >= screenHeight - screenHeight * yTop && tempY <= screenHeight - screenHeight * yBottom)
		{
		int gridRow=0,gridColumn=0;
		gridSizeX = xsize;
		gridSizeY = ysize;

		GLint gridWidth = screenWidth * xRight - screenWidth * xLeft , gridHeight = screenHeight * yTop - screenHeight * yBottom, ix, iy;
		float cellwidth = gridWidth / gridSizeX;
		float cellheight = gridHeight / gridSizeY;

		cout<<"Current mouse pointer position"<<endl;
		cout<<"X: "<<tempX<<endl;
		cout<<"Y: "<<tempY<<endl;

		tempY = tempY - .04 * screenHeight;
		tempX = tempX - .2 * screenWidth;
//.04 down
// .2 right

		gridRow=floor(tempY/cellheight);  //Calculates gridRow for the current mouse position
		gridColumn= floor(tempX/cellwidth); //Calculates gridColumn for the current mouse position




		cout<<"Respective Grid co-ordinates"<<endl;
		cout<<"X: "<<gridRow<<endl;
		cout<<"Y: "<<gridColumn<<endl<<"--------"<<endl;
		int cord = xsize * gridRow + gridColumn;
		if (gridLetters[cord].ready == true)
		{
			for (int v = 0 ; v < gridLetters.size(); v++){
			cout << gridLetters[v].ready;}
			int currentCord = cord;
			int currentPoint = gridLetters[currentCord].points;
			gridLetters[currentCord].finished = true;
			wordsList[gridLetters[currentCord].word].complete = true;
			scoreint += currentPoint;
			if (scoreint > highScore)
			{highScore = scoreint;}
			gridLetters[cord].ready = false;
			for (int e = 0 ; e < activeVector.size() ; e++)
				{
				activeVector[e]->finished = true;
				cout << "BLUE";
					}
			activeVector.clear();
		}
		if (!activeVector.empty())
			{
			if (cord == activeVector.back()->next)
			{
				if (gridLetters[nextLetter].wordEnd == true)
				{
				gridLetters[nextLetter].ready = true;
				 gridLetters[cord].active = true;
				 nextLetter = gridLetters[cord].next;
				 activeVector.push_back(&gridLetters[cord]);
				}
				 else
				 {
					 gridLetters[cord].active = true;
					 nextLetter = gridLetters[cord].next;
					 activeVector.push_back(&gridLetters[cord]);
					 cout << "RED1";
					 cout << gridLetters[cord].letter;
				 }
			}
			else
				{gridLetters[cord].active = true;
				for (int e = 0 ; e < activeVector.size() ; e++)
				{
					activeVector[e]->active = false;
				}
				activeVector.clear();
				nextLetter = gridLetters[cord].next;
				activeVector.push_back(&gridLetters[cord]);
				cout << "WHITE";
				}
			}
		else
			{
			gridLetters[cord].active = true;
			nextLetter = gridLetters[cord].next;
			activeVector.push_back(&gridLetters[cord]);
//			cout << activeVector.back()->letter;
			cout << "RED 2";
			}
		// display the letter on the grid
		//cout<<"Letter: "<<getGridChar((gridSizeY*gridRow)+gridColumn);
		// change color to red
		//checkcolor=0.0;

		// display the character again
		//renderBitmapString(DisplayWidth/(gridSizeX*4)+(gridColumn)*DisplayWidth/(gridSizeX),DisplayHeight/(gridSizeY*2)+(gridRow)*DisplayHeight/(gridSizeY),getGridChar((gridSizeY*gridRow)+gridColumn));
		glutSwapBuffers();
		}
	}
	else if(currentPage==5){
		if(xi == tempX && yi == tempY && tempX >= screenWidth * 0.30 && tempX <= screenWidth * .70
						&& tempY >= screenHeight - screenHeight * .22 && tempY <= screenHeight - screenHeight * .03)
				{
					restartClock();
					currentPage=4;
				}
	}
	else if (currentPage == 6){
		if (xi == tempX && yi == tempY && tempX >= screenWidth * 0.55 && tempX <= screenWidth * .75
				&& tempY >= screenHeight - screenHeight * .25 && tempY <= screenHeight - screenHeight * .08) // comparing the  current cursor(mouse pointer) co-ordinates with the co-ordinates of the button.
		{


			startCounter = 0;
			scoreint = 0;
			currentPage = 1;
		}
		if (xi == tempX && yi == tempY && tempX >= screenWidth * 0.25 && tempX <= screenWidth * .45
				&& tempY >= screenHeight - screenHeight * .25 && tempY <= screenHeight - screenHeight * .08)
		{

			startCounter = 0;
			scoreint = 0;

			currentPage = 4;
		}

	}
	else
	{
		if (xi == tempX && yi == tempY && tempX >= screenWidth * 0.82 && tempX <= screenWidth * .98
				&& tempY >= screenHeight - screenHeight * .76 && tempY <= screenHeight - screenHeight * .61)
				{
			timercounter = 0;
			startCounter = 0;
			currentPage = 1;
				}
	}
}
glutTimerFunc(500, Timer, timercounter);

}


void myMouseMove(int x, int y)
{
timercounter = 0; //timercounter is set to 0, every mousepointer position it sets 0
tempX = x; // current mousepointer x co-ordinate
tempY = y;// current mousepointer y co-ordinate
xi = tempX;
yi = tempY; // I am setting current mouse pointer x,y co-ordinate to the xi,yi. THis helps to compare whether mouse pointer is in the same place for certain time.
Timer(timercounter); //calling the timer with timecounter
glutPostRedisplay();

}

string randomz(){
	DIR *dpdf;
	struct dirent *epdf;
	vector<string> x;
	int i = 0;

	dpdf = opendir("./WordLists");
	if (dpdf != NULL){
	   while (epdf = readdir(dpdf)){
		   string str(epdf->d_name);
		   x.push_back(str);
		   i++;
	   	   	   	   	   	   	   	   }
						}
	srand(time(NULL));
	int randNum = rand()% (x.size()-2)+ 1;
	cout << x.size() << endl;
	cout << randNum << endl;
	return x[randNum];
}




















