#include "ofApp.h"

void ofApp::setup() {
    bloodGlucoseValue = -1;
    font.loadFont("NovaMono.ttf", 120);
    halfWidth = ofGetWidth() * 0.5;

    boxSize.x = 300;
    boxSize.y = 160;
    bgTextOffset.x = 135;
    bgTextOffset.y = 121;
    bgBoxOffset.x = boxSize.x * 0.5;
    bgBoxOffset.y = boxSize.y + 100;

    bgTextPosition.x = halfWidth - bgTextOffset.x;
    bgTextPosition.y = ofGetHeight() - bgTextOffset.y;
    bgBoxPosition.x = halfWidth - bgBoxOffset.x;
    bgBoxPosition.y = ofGetHeight() - bgBoxOffset.y;

    jsonParser.startThread(true, false);
}

void ofApp::update() {
    jsonParser.lock();
    bloodGlucoseValue = jsonParser.bloodGlucoseValue;
    jsonParser.unlock();
}

void ofApp::draw() {
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 25, 25);
    if(bloodGlucoseValue != -1) {
        ofSetColor(ofColor::white);
        ofRect(bgBoxPosition.x, bgBoxPosition.y, boxSize.x, boxSize.y);
        ofSetColor(ofColor::black);
        font.drawString(ofToString(bloodGlucoseValue), bgTextPosition.x, bgTextPosition.y);
    }
}

void ofApp::exit() {
    jsonParser.stopThread();
}