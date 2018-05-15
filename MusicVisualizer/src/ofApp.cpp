#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetDataPathRoot("../Resources/data/");
	filePicked = chooseFile();
	beat.play();
	beat.setLoop(true);
	time.load("Raleway-Regular.ttf", 12);
	settings.load("Raleway-Regular.ttf", 12);
	
	fftSmooth = new float [BANDS];
	for (int i = 0; i < BANDS; i++) {
		fftSmooth[i] = 0;
	}
	
	//set random positions and movements
	for (int i = 0; i < BANDS; i++) {
		xValues.push_back(rand() % ofGetWidth());
		int tempHeight = rand() % ofGetHeight();
		if (tempHeight < OVERLAP) {
			tempHeight += OVERLAP;
		}
		yValues.push_back(tempHeight);
		movePattern.push_back(rand() % DIRECTIONS);
	}
	duration = beat.length / beat.internalFreq;
	
	logo.load("pb_logo.png");
	bg.load("backgrounds/castle.jpg");
	
	//create post processing effects
	post.init(ofGetWidth(), ofGetHeight());
	post.createPass<BloomPass>();
	
	zoomPost.init(ofGetWidth(), ofGetHeight());
	zoomPost.createPass<ZoomBlurPass>();
}

//--------------------------------------------------------------
void ofApp::update(){
	//DERIVED FROM soundPlayerFFTExample within of/examples/sound/
	ofSoundUpdate();
	
	//update frequency values with smoothing factor
	float * value = ofSoundGetSpectrum(BANDS);
	for (int i = 0; i < BANDS; i++) {
		fftSmooth[i] *= smoothing;
		if (fftSmooth[i] < value[i]) {
			fftSmooth[i] = value[i];
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(backgroundColor);
	
	ofSetColor(MAX_COLOR);
	
	
	if (fftSmooth[BASS_FFT] * (smoothMultiplier) > 75) {
		zoomPost.begin();
	} else {
		post.begin();
	}
	
	if (pianoSong) {
		drawCircles(MAX_COLOR / pianoMult);
	} else {
		drawCircles(MAX_COLOR);
	}
	
	if (fftSmooth[BASS_FFT] * (smoothMultiplier) > 75) {
		zoomPost.end();
	} else {
		post.end();
	}
	
	//non-rainbow preset, show transparent background
	if (!rainbowSpec) {
		ofSetColor(MAX_COLOR, MAX_COLOR, MAX_COLOR, 2 * MAX_COLOR / 3);
		bg.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
	
	ofSetColor(MAX_COLOR);
	
	if (showGui) {
		gui.draw();
	}
	
	if (showHist) {
		updateHistory();
	}
	
	drawRects();
	
	if (showLogo) {
		if (swapColors) {
			//automatically switch base colors with time delay
			if (fftSmooth[BASS_FFT] * (smoothMultiplier) > 50 && colorSwitchGap > 25) {
				colorTint = rand() % 255;
				colorSwitchGap = 0;
			}
			colorSwitchGap++;
		}
		
		ofSetColor(MAX_COLOR);
		
		logo.setAnchorPoint(logo.getWidth() / 4, logo.getHeight() / 4);
		logo.draw((ofGetWidth() - fftSmooth[BASS_FFT] * smoothMultiplier) / 2, (ofGetHeight() - fftSmooth[BASS_FFT] * smoothMultiplier) / 2 + (TOP_PANEL_HEIGHT), (logo.getWidth() / 2) + fftSmooth[BASS_FFT] * smoothMultiplier, (logo.getHeight() / 2) + fftSmooth[BASS_FFT] * (smoothMultiplier));
	}
	drawUI();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key){
		case ' ':
			//play/stop song
			if(beat.isPlaying()){
				beat.stop();
			} else {
				beat.play();
			}
			break;
		case OF_KEY_RIGHT:
			//fast forward song by ten seconds
			beat.setPositionMS( beat.getPositionMS() + TIMESHIFT);
			break;
		case OF_KEY_LEFT:
			//rewind song by ten seconds
			beat.setPositionMS( beat.getPositionMS() - TIMESHIFT);
			break;
		case 's':
			//toggle GUI
			showGui = !showGui;
			break;
		case 'h':
			//toggle history graph
			showHist = !showHist;
			break;
		case 'n':
			//choose new song
			beat.stop();
			filePicked = chooseFile();
			beat.play();
			beat.setLoop(true);
			break;
		case 'p':
			//toggle piano preset
			pianoSong = !pianoSong;
			break;
		case 'g':
			//toggle guide text
			showGuide = !showGuide;
			break;
		case 'l':
			//toggle logo display
			showLogo = !showLogo;
			break;
		case 'b':
			//choose new background
			beat.stop();
			bgPicked = chooseBG();
			beat.play();
			beat.setLoop(true);
			break;
		case '1':
			//manually change base colors
			colorTint = rand() % (MAX_COLOR / 4);
			break;
		case '2':
			colorTint = (rand() % (MAX_COLOR / 4)) + (MAX_COLOR / 4);
			break;
		case '3':
			colorTint = (rand() % (MAX_COLOR / 4)) + (MAX_COLOR / 2);
			break;
		case '4':
			colorTint = (rand() % (MAX_COLOR / 4)) + ((3 * MAX_COLOR) / 4);
			break;
		case 'r':
			//toggle rainbow preset
			rainbowSpec = !rainbowSpec;
			break;
			
	}
}

//--------------------------------------------------------------
float ofApp::setXY(int i) {
	float xPos;
	float yPos;
	float radius = (fftSmooth[i] * (smoothMultiplier / 16)) * log(i + 0.1) + MIN_SIZE;
	
	float beatMult = beatSpeed;
	
	//increase speed of larger particles
	if (radius > fastThreshold) {
		beatMult = beatSpeed * 4;
	} else if (radius > fastThreshold / 2) {
		beatMult = beatSpeed * 2;
	}
	
	switch (movePattern[i]) {
		case 0:
			xPos = (xValues[i] - beatMult);
			yPos = (yValues[i] - beatMult);
			break;
		case 1:
			xPos = (xValues[i] - beatMult);
			yPos = (yValues[i]);
			break;
		case 2:
			xPos = (xValues[i] - beatMult);
			yPos = (yValues[i] + beatMult);
			break;
		case 3:
			xPos = (xValues[i]);
			yPos = (yValues[i] + beatMult);
			break;
		case 4:
			xPos = (xValues[i] + beatMult);
			yPos = (yValues[i] + beatMult);
			break;
		case 5:
			xPos = (xValues[i] + beatMult);
			yPos = (yValues[i]);
			break;
		case 6:
			xPos = (xValues[i] + beatMult);
			yPos = (yValues[i] - beatMult);
			break;
		default:
			xPos = (xValues[i]);
			yPos = (yValues[i] - beatMult);
			break;
	}
	
	//move particle to other side of screen if off-screen
	if (xPos < 0) {
		xPos+=ofGetWidth();
	}
	if (yPos < 0) {
		yPos+=ofGetHeight();
	}
	if (xPos > ofGetWidth()) {
		xPos-=ofGetWidth();
	}
	if (yPos > ofGetHeight()) {
		yPos-=ofGetHeight();
	}
	
	xValues[i] = xPos;
	yValues[i] = yPos;
	
	return radius;
}

//--------------------------------------------------------------
void ofApp::drawCircles(int count) {
	for (int i = 0; i < count; i++) {
		ofColor currentColor;
		
		//color based on rainbow and piano presets
		if (rainbowSpec && pianoSong) {
			currentColor.setHsb(i * pianoMult, (fftSmooth[i] * 500 + 1) / 1.2, (fftSmooth[i] * 500 + 1) / 1.2);
		} else if (rainbowSpec) {
			currentColor.setHsb(i, (fftSmooth[i] * 500 + 1) / 1.2, (fftSmooth[i] * 500 + 1) / 1.2);
		} else {
			currentColor.setHsb(colorTint, colorTint, (MAX_COLOR / 2) + ((fftSmooth[i] * 500 + 1) / 1.2));
		}
		
		float radius = setXY(i);
		
		ofSetColor(currentColor, MAX_COLOR);
		
		if (radius > minShow) {
			ofSetCircleResolution(circleRes);
			ofDrawCircle(xValues[i], yValues[i], radius);
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawRects() {
	int whiteCount = 0;
	if (pianoSong) {
		for (int i = 0; i < MAX_COLOR / pianoMult; i++) {
			ofColor currentColor;
			
			if (rainbowSpec) {
				currentColor.setHsb(i * pianoMult, MAX_COLOR, MAX_COLOR);
			} else {
				currentColor.setHsb(colorTint, colorTint, (MAX_COLOR / 2) + whiteCount * pianoMult);
			}
			ofSetColor(currentColor);
			
			float height = (fftSmooth[i] * smoothMultiplier) * log(i + 1) + MIN_SIZE;
			if (i < 2) {
				height = (fftSmooth[i] * smoothMultiplier) + MIN_SIZE;
			}
			if (!rainbowSpec && height > minShow * 10) {
				//color rainbow if height large enough
				currentColor.setHsb(i * pianoMult, MAX_COLOR, (MAX_COLOR / 2) + whiteCount * pianoMult);
				ofSetColor(currentColor);
			}
			
			float width = 9;
			
			ofDrawRectRounded(i * 2 * width, 25, width, height, 4);
			
			whiteCount++;
		}
	} else {
		for (int i = 0; i < MAX_COLOR; i++) {
			ofColor currentColor;
			
			if (rainbowSpec) {
				currentColor.setHsb(i, MAX_COLOR, MAX_COLOR);
			} else {
				currentColor.setHsb(colorTint, colorTint, (MAX_COLOR / 2) + whiteCount);
			}
			ofSetColor(currentColor);
			//display every second frequency
			if (i % 2 == 0) {
				float height = (fftSmooth[i] * smoothMultiplier) * log(i + 1) + MIN_SIZE;
				if (i < 2) {
					height = (fftSmooth[i] * smoothMultiplier) + MIN_SIZE;
				}
				float width = 8;
				
				if (!rainbowSpec && height > minShow * 10) {
					//color rainbow if height large enough
					currentColor.setHsb(i, MAX_COLOR, (MAX_COLOR / 2) + whiteCount);
					ofSetColor(currentColor);
				}
				
				ofDrawRectRounded((i / 1.4) * width, 25, width, height, 4);
				
				whiteCount++;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::makeGui() {
	if (firstTime) {
		gui.setup("panel");
	}
	gui.setPosition(ofPoint(ofGetWidth() - 300, 150));
	
	colors.setName("Color Settings");
	colors.add(backgroundColor.set("Background Color", 25, 0, MAX_COLOR));
	colors.add(lineColor.set("Line Color", 100, 0, MAX_COLOR));
	colors.add(timeColor.set("Time Color", 200, 0, MAX_COLOR));
	colors.add(topColor.set("Panel Color", 50, 0, MAX_COLOR));
	colors.add(colorTint.set("Color Tint", 20, 0, MAX_COLOR));
	colors.add(swapColors.set("Light Show", false));
	
	visuals.setName("Visualization Settings");
	visuals.add(smoothing.set("Smoothing", 0.9, 0.01, 0.99));
	visuals.add(smoothMultiplier.set("Smooth Multiplier", 120, 1, 999));
	visuals.add(beatSpeed.set("Beat Speed", 0.5, 0, 10));
	visuals.add(fastThreshold.set("Fast Threshold", 20, 1, 50));
	visuals.add(minShow.set("Minimum Circle Radius", 12, 1, 200));
	
	uiPanel.setName("UI Settings");
	uiPanel.add(showHist.set("Show History 'H'", false));
	uiPanel.add(pianoSong.set("Piano Preset 'P'", false));
	uiPanel.add(showGuide.set("Show Guide 'G'", true));
	uiPanel.add(showLogo.set("Show Logo 'L'", true));
	uiPanel.add(rainbowSpec.set("Rainbow Spectrum 'R'", false));
	
	if (firstTime) {
		//only draw gui groups once
		firstTime = false;
		gui.add(colors);
		gui.add(visuals);
		gui.add(uiPanel);
	}
}

//--------------------------------------------------------------
void ofApp::makeHistory() {
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	for (int i = 0; i < totalPlots; i++) {
		int numSamples = 600;
		ofxHistoryPlot* plot = new ofxHistoryPlot( NULL, "", numSamples, false);
		plot->setRange(0, 3);
		plot->setShowNumericalInfo(false); //don't show text
		plot->setLineWidth(3);
		plot->setBackgroundColor(ofColor(backgroundColor, 0)); //transparent background
		plot->setDrawGrid(false);
		plots.push_back(plot);
	}
}

//--------------------------------------------------------------
void ofApp::updateHistory() {
	int plotPoint = 20;
	int plotMult = 1;
	for (ofxHistoryPlot* plot : plots) {
		plot->update(fftSmooth[plotPoint * plotMult]);
		ofColor col;
		col.setHsb(plotPoint * plotMult, MAX_COLOR, MAX_COLOR);
		plot->setColor(col);
		plot->draw(0, ofGetHeight() - 250, ofGetWidth(), 240);
		plotMult++;
	}
}

//--------------------------------------------------------------
void ofApp::resetup() {
	time.load("Raleway-Regular.ttf", 12);
	settings.load("Raleway-Regular.ttf", 12);
	
	fftSmooth = new float [BANDS];
	for (int i = 0; i < BANDS; i++) {
		fftSmooth[i] = 0;
	}
	
	ofBackground(backgroundColor);
	
	for (int i = 0; i < BANDS; i++) {
		xValues.push_back(rand() % ofGetWidth());
		int tempHeight = rand() % ofGetHeight();
		if (tempHeight < OVERLAP) {
			tempHeight += OVERLAP;
		}
		yValues.push_back(tempHeight);
		movePattern.push_back(rand() % DIRECTIONS);
	}
	duration = beat.length / beat.internalFreq;
	
	makeGui();
	
	plots.clear(); //remove previous graphs
	makeHistory();
}

//--------------------------------------------------------------
bool ofApp::chooseFile() {
	//Open the Open File Dialog
	ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a new audio file");
	
	//Check if the user opened a file
	if (openFileResult.bSuccess){
		ofFile file(openFileResult.getPath());
		if (ofToUpper(file.getExtension()) == "MP3" || ofToUpper(file.getExtension()) == "WAV") {
			beat.load(openFileResult.getPath());
			resetup();
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------
bool ofApp::chooseBG() {
	//Open the Open File Dialog
	ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a new background file");
	
	//Check if the user opened a file
	if (openFileResult.bSuccess){
		ofFile file(openFileResult.getPath());
		if (ofToUpper(file.getExtension()) == "PNG" || ofToUpper(file.getExtension()) == "JPG") {
			bg.load(openFileResult.getPath());
			resetup();
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------
void ofApp::drawUI() {
	ofSetColor(topColor);
	ofDrawRectangle(0, 0, ofGetWidth(), TOP_PANEL_HEIGHT);
	
	ofSetColor(MAX_COLOR);
	
	std::string timeStr = ofToString(beat.getPositionMS() / SECOND / MINUTE); //minutes
	if ((beat.getPositionMS() / SECOND) % MINUTE < 10) {
		timeStr += ":0" + ofToString((beat.getPositionMS() / SECOND) % MINUTE); //seconds if less than ten
	} else {
		timeStr += ":" + ofToString((beat.getPositionMS() / SECOND) % MINUTE); //seconds
	}
	time.drawString(timeStr, WORD_WIDTH - 5, WORD_HEIGHT);
	
	if (showGuide) {
		settings.drawString("settings = 's'", ofGetWidth() - WORD_WIDTH + 5, WORD_HEIGHT);
		settings.drawString("new song = 'n'", 5, WORD_HEIGHT);
	}
	
	//static timeline length
	ofSetLineWidth(10);
	ofSetColor(lineColor);
	ofDrawLine(TOP_PANEL_WIDTH + 5, 12, ofGetWidth() - TOP_PANEL_WIDTH, 12);
	
	//movement based on time duration
	ofSetColor(timeColor);
	msecondGain = (ofGetWidth() - (TOP_PANEL_WIDTH * 2)) / (duration * SECOND);
	ofDrawCircle(TOP_PANEL_WIDTH + msecondGain * beat.getPositionMS(), WORD_HEIGHT - 5, MIN_SIZE - 1 + fftSmooth[BASS_FFT] * 10);
}
