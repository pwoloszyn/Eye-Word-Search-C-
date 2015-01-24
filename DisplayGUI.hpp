/*
 * DisplayGUI.hpp
 *
 *  Created on: May 2, 2014
 *      Author: jkrich
 */

#ifndef DISPLAYGUI_HPP_
#define DISPLAYGUI_HPP_


#include <GL/glut.h>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <math.h>
#include "Word.h"
#include "WordGrid.h"
#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<dirent.h>
#include <time.h>


using namespace std;

struct wordTagged{
	char letter;
	bool active ;
	bool finished ;
	int points;
	int length;
	string shape;
	bool wordEnd;
	int next;
	bool ready;
	int word;

};

struct completedWords{
	string word;
	bool complete;
};

void renderCurrentPage();
void renderBitmapString(float x, float y, string string);
void renderBitmapStringR(float x, float y, string string);
void renderBitmapStringB(float x, float y, string string);
void start();
void difficulty();
void name();
void showHelp();
void displayHelpText();
void drawDoneButton();
void gameBoard(vector<completedWords> wordsList, vector<wordTagged> gridLetters);
void largeButton(int);
void screenTitle(int);
void levelDisplay(int);
void nameDisplay(string);
void block(string, string);
void themeItems(string);
//void pictures(string){}
void drawCountryPicture1();
void drawCountryPicture2();
void wordList(vector<completedWords> wordsList);
void gameGrid(float, float, vector<wordTagged>);
void drawBlock(float, float, float, float);
void letterBlock(float x1, float x2, float y1, float y2, char letter);
void letterBlockR(float x1, float x2, float y1, float y2, char letter);
void letterBlockB(float x1, float x2, float y1, float y2, char letter);
void drawLetterA(int, int, int);
void timer();
void scoreDisplay(int, int);
void drawBackground();
void drawRestArea();
void drawLevelButton();
void drawInitialButton();
void drawStartButton();
void drawEasyButton();
void drawModerateButton();
void drawHardButton();
void drawWordSearch();
void drawSelectDifficulty();
void drawLevelBlockBackground();
void drawLongBlockBackground();
void drawPlayerIniBackground();
void drawLevel1Background();
void drawLevel5Background();
void drawLevel10Background();
void drawCurrentLevelBackground();
void drawTimerBackground();
void pauseClock();
void restartClock();
void addSeconds(int extra);
void drawScoreBoardBackground();
void drawThemeBackground();
void drawHelpBackground();
void drawContinueButton();
void drawGameOverBackground();
void drawHighScoreBackground();
void drawPlayerScoreBackground();
void drawEndGameButton();
void gameOver();
//void drawBlockTitle(string);
//void drawBlockContent(string);
void drawDefaultPicture1();
void drawDefaultPicture2();
void drawGameBlock();

void myMouseMove(int x, int y);
static void Timer(int value);
string randomz();

#endif /* DISPLAYGUI_HPP_ */
