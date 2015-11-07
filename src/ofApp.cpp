#include "ofApp.h"

void ofApp::setup() {
    width = 900; height = 1440;
    ofSetWindowShape(width, height);
    //ofToggleFullscreen();
    bloodGlucoseValue = -1;
    font.loadFont("NovaMono.ttf", 120);
    newValueDetected = false;
    delayBeforeAnimation = 9000;
    animationInSecs = 0.5;
    animationOutSecs = 4.5;

    boxSize.x = 300;
    boxSize.y = 160;
    boxBorderSize = 10;
    bgTextOffset.x = 135;
    bgTextOffset.y = 121;
    bgBoxOffset.x = boxSize.x * 0.5;
    bgBoxOffset.y = boxSize.y + 100;

    boxBorderDouble = boxBorderSize * 2;
    halfWidth = width * 0.5;

    bgTextPosition.x = halfWidth - bgTextOffset.x;
    bgTextPosition.y = height - bgTextOffset.y;
    bgBoxPosition.x = halfWidth - bgBoxOffset.x;
    bgBoxPosition.y = height - bgBoxOffset.y;

    bgOpacity.setRepeatType(PLAY_ONCE);
    bgOpacity.setCurve(EASE_IN_EASE_OUT);
    //jsonParser.startThread(true, false);
    anim.setup(width, height);
}

void ofApp::update() {
    /*jsonParser.lock();
    bloodGlucoseValue = jsonParser.bloodGlucoseValue;
    newValueDetected = jsonParser.newValueDetected;
    jsonParser.newValueDetected = false;
    jsonParser.unlock();*/
    if(newValueDetected) {
        startTimer();
        bgOpacity.animateFromTo(0, 255);
        bgOpacity.setDuration(animationInSecs);
        newValueDetected = false;
    }
    if(timerRunning) {
        animateWhenTimerFinished();
    }
    bgOpacity.update(1.0f / 60.0f);
    anim.update();
}

void ofApp::draw() {
    anim.draw();
    //ofDrawBitmapString(ofToString(ofGetFrameRate()), 25, 25);
    if(bloodGlucoseValue != -1) {
        ofSetColor(ofColor::white, bgOpacity.val());
        ofRect(bgBoxPosition.x, bgBoxPosition.y, boxSize.x, boxSize.y);
        ofSetColor(ofColor::black, bgOpacity.val());
        ofRect(bgBoxPosition.x + boxBorderSize, bgBoxPosition.y + boxBorderSize, boxSize.x - boxBorderDouble, boxSize.y - boxBorderDouble);
        ofSetColor(ofColor::lightGray, bgOpacity.val());
        font.drawString(ofToString(bloodGlucoseValue), bgTextPosition.x, bgTextPosition.y);
    }
}

void ofApp::keyPressed(int key) {
    anim.keyPressed(key);
}

void ofApp::exit() {
    jsonParser.stopThread();
}

void ofApp::startTimer() {
    timerRunning = true;
    timerStartedAt = ofGetElapsedTimeMillis();
}

void ofApp::animateWhenTimerFinished() {
    if(ofGetElapsedTimeMillis() > timerStartedAt + delayBeforeAnimation) {
        bgOpacity.animateFromTo(255, 0);
        bgOpacity.setDuration(animationOutSecs);
        timerRunning = false;
    }
}