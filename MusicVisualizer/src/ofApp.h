#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxHistoryPlot.h"
#include "ofxPostProcessing.h"
#include <vector>

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	float setXY(int i);
	void drawCircles(int count);
	void drawRects();
	void makeGui();
	void makeHistory();
	void updateHistory();
	void resetup();
	bool chooseFile();
	bool chooseBG();
	void drawUI();
	
	ofFmodSoundPlayer beat;
	ofTrueTypeFont time;
	ofTrueTypeFont settings;
	
	std::vector<ofxHistoryPlot*> plots;
	int totalPlots = 6;
	
	ofxPanel gui;
	
	ofParameterGroup colors;
	ofParameter<int> backgroundColor;
	ofParameter<int> lineColor;
	ofParameter<int> timeColor;
	ofParameter<int> topColor;
	ofParameter<int> colorTint;
	ofParameter<bool> swapColors;
	
	ofParameterGroup visuals;
	ofParameter<float> smoothing;
	ofParameter<int> smoothMultiplier;
	ofParameter<float> beatSpeed;
	ofParameter<int> fastThreshold;
	ofParameter<int> minShow;
	
	ofParameterGroup uiPanel;
	ofParameter<bool> showHist;
	ofParameter<bool> pianoSong;
	ofParameter<bool> showGuide;
	ofParameter<bool> showLogo;
	ofParameter<bool> rainbowSpec;
	
	float duration;
	float* fftSmooth;
	float msecondGain;
	
	const int OVERLAP = 50;
	const int BANDS = 1530;
	const int DIRECTIONS = 8;
	const int MAX_COLOR = 255;
	const int MIN_SIZE = 5;
	
	const int TOP_PANEL_HEIGHT = 25;
	const int TOP_PANEL_WIDTH = 150;
	const int WORD_WIDTH = 120;
	const int WORD_HEIGHT = 17;
	
	const int SECOND = 1000;
	const int MINUTE = 60;
	const int TIMESHIFT = 10000;
	
	const int BASS_FFT = 10;
	
	std::vector<float> xValues;
	std::vector<float> yValues;
	std::vector<int> movePattern;
	std::string fileName;
	
	bool showGui = false;
	
	float pianoMult = 3;
	
	bool filePicked = false;
	bool bgPicked = false;
	
	int colorSwitchGap = 0;
	
	ofImage logo;
	ofImage bg;
	
	int circleRes = 100;
	bool firstTime = true;
	
	ofxPostProcessing post;
	ofxPostProcessing zoomPost;
};
