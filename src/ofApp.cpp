#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	//cam.setup(640, 480);
    gui.setup();
    gui.add(minArea.set("Min area", 10, 1, 100));
    gui.add(maxArea.set("Max area", 200, 1, 500));
    gui.add(threshold.set("Threshold", 128, 0, 255));
    gui.add(holes.set("Holes", false));
    gui.add(smoothingSize.set("smoothing size", 1, 0, 40));
    gui.add(maxNumLines.set("max numlines", 10, 10, 10000));
    gui.add(blurPasses.set("blur passes", 1, 0,10));
    gui.add(blurOffset.set("blur offset", 20, 0, 130));
    
    img.load("wrist.jpg");
    img.resize(640*2,480*2);
    
    ofFbo::Settings s;
    s.width = ofGetWidth();
    s.height = ofGetHeight();
    s.internalformat = GL_RGBA;
    s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    s.maxFilter = GL_LINEAR; GL_NEAREST;
    s.numSamples = 0;
    s.numColorbuffers = 1;
    s.useDepth = false;
    s.useStencil = false;
    
    gpuBlur.setup(s, false);
    bShowGui = true;

}

void ofApp::update() {
	//cam.update();
    //if(cam.isFrameNew()) {
    
    
    gpuBlur.blurOffset = blurOffset;
    //gpuBlur.blurOffset = 15;
    gpuBlur.blurPasses = blurPasses;
    //gpuBlur.blurPasses = 1;
    gpuBlur.numBlurOverlays = 1;
    gpuBlur.blurOverlayGain = 255;
    
    
    contourFinder.setMinAreaRadius(minArea);
    contourFinder.setMaxAreaRadius(maxArea);
    contourFinder.setThreshold(threshold);
    contourFinder.findContours(img);
    contourFinder.setFindHoles(holes);
    
//    lines.clear();
    for(int i = 0; i < contourFinder.size(); i++){
        ofPolyline p;
        p = contourFinder.getPolyline(i);
        lines.push_back(p);
    }
    
    if(lines.size() > maxNumLines){
        int diff = lines.size() - maxNumLines;
        for(int i = 0; i < maxNumLines; i++){
            lines.pop_front();
        }
    }
    

	//}
}

void ofApp::draw() {
    //img.draw(0, 0);
    ofBackground(0);
    ofImage grey = img;
    grey.setImageType(OF_IMAGE_GRAYSCALE);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    gpuBlur.beginDrawScene();
    ofClear(0, 0, 0, 0);
    ofSetColor(255);
//    ofCircle(ofGetWidth()/2, ofGetHeight()/2,  100);
    grey.draw(0,0);
    gpuBlur.endDrawScene();
    
    gpuBlur.performBlur();

    ofSetColor(255);
    //overlay the blur on top
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); //pre-multiplied alpha
    gpuBlur.drawBlurFbo();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    
//    ofSetColor(80);
    for(auto& p : lines){
        ofPolyline sp = p.getSmoothed(smoothingSize);
        ofSetColor(200, 255, 240);
        sp.draw();
        ofSetColor(255);
//        for(auto& pt : sp.getVertices()){
//            float rando = ofRandom(1.0);
//            if(rando > 0.95)  ofDrawCircle(pt, 2);
//        }
        
    }
    
	//contourFinder.draw();
    if(bShowGui)  {
       gui.draw();
        ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, ofGetHeight() - 20);
    }
}

void ofApp::keyPressed(int key){
    if(key == 'g'){
        bShowGui = !bShowGui;
    }
    
    if(key == 's'){
        ofSaveFrame();
    }
}
