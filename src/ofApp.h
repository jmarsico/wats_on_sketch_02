#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxFboBlur.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    void keyPressed(int key);
	
	ofVideoGrabber cam;
    ofImage img;
	ofxCv::ContourFinder contourFinder;
    
    ofxPanel gui;
    ofParameter<float> minArea, maxArea, threshold;
    ofParameter<bool> holes;
    ofParameter<int> smoothingSize;
    ofParameter<int> maxNumLines;
    ofParameter<float> blurOffset;
    ofParameter<float> blurPasses;
    
    deque<ofPolyline> lines;
    
    ofxFboBlur gpuBlur;
    bool bShowGui;
    
    
};
