#include "ofApp.h"

void ofApp::setup() {
    bloodGlucoseValue = -1;
    jsonParser.startThread(true, false);
    font.loadFont("NovaMono.ttf", 120);
    bgTextPosition.x = (ofGetWidth() * 0.5) - 150;
    bgTextPosition.y = ofGetHeight() - 300;
}

void ofApp::update() {
    jsonParser.lock();
    bloodGlucoseValue = jsonParser.bloodGlucoseValue;
    jsonParser.unlock();
}

void ofApp::draw() {
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 25, 25);
    if(bloodGlucoseValue > 0) {
        font.drawString(ofToString(bloodGlucoseValue), bgTextPosition.x, bgTextPosition.y);
    }
}

void ofApp::exit() {
    jsonParser.stopThread();
}