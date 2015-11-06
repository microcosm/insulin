#include "ofApp.h"

void ofApp::setup() {
    jsonParser.startThread(true, false);
}

void ofApp::update() {
    jsonParser.lock();
    
    jsonParser.unlock();
}

void ofApp::draw() {
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 25, 25);
}

void ofApp::exit() {
    jsonParser.stopThread();
}