#include "ofApp.h"

void ofApp::setup() {
    bloodGlucoseValue = -1;
    jsonParser.startThread(true, false);
}

void ofApp::update() {
    jsonParser.lock();
    bloodGlucoseValue = jsonParser.bloodGlucoseValue;
    jsonParser.unlock();
}

void ofApp::draw() {
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 25, 25);
    ofDrawBitmapString(ofToString(bloodGlucoseValue), 25, 100);
}

void ofApp::exit() {
    jsonParser.stopThread();
}