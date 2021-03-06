#include "ofApp.h"

void ofApp::setup() {
    productionMode = false;
    testMode = false;

    width = 900; height = 1440;
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_NOTICE);

    if(productionMode) {
        ofLogToFile("log.txt", true);
        ofToggleFullscreen();
    } else {
        ofLogToConsole();
        ofSetLogLevel("inAnimation", OF_LOG_NOTICE);
        ofSetWindowShape(width, height);
    }

    bloodGlucoseValue = -1;
    font.load("NovaMono.ttf", 120);
    newValueDetected = false;
    delayBeforeAnimation = 220000;
    animationInSecs = 0.5;
    animationOutSecs = 5.5;

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
    jsonParser.startThread(true);

    anim.setup(width, height, testMode);
    if(testMode) {
        testModeFont.load("NovaMono.ttf", 30);
        bgOpacity.reset(255);
    }
}

void ofApp::update() {
    jsonParser.lock();
    bloodGlucoseValue = jsonParser.bloodGlucoseValue;
    newValueDetected = jsonParser.newValueDetected;
    jsonParser.newValueDetected = false;
    jsonParser.unlock();
    if(!testMode && newValueDetected) {
        startTimer();
        bgOpacity.animateFromTo(0, 255);
        bgOpacity.setDuration(animationInSecs);
        anim.newBgValue(bloodGlucoseValue);
        newValueDetected = false;
    }
    if(timerRunning) {
        animateWhenTimerFinished();
    }
    if(ofGetFrameNum() > 0) {
        bgOpacity.update(ofGetLastFrameTime());
    }
    anim.update();
}

void ofApp::draw() {
    anim.draw();
    if(!productionMode) {
        ofSetColor(ofColor::white);
        ofDrawBitmapString(ofToString(ofGetFrameRate()), 25, 25);
    }
    if(testMode) {
        bloodGlucoseValue = floor(anim.currentTestBg() * 0.1) * 10;
    }
    if(bloodGlucoseValue != -1) {
        ofSetColor(ofColor::white, bgOpacity.val());
        ofDrawRectangle(bgBoxPosition.x, bgBoxPosition.y, boxSize.x, boxSize.y);
        ofSetColor(ofColor::black, bgOpacity.val());
        ofDrawRectangle(bgBoxPosition.x + boxBorderSize, bgBoxPosition.y + boxBorderSize, boxSize.x - boxBorderDouble, boxSize.y - boxBorderDouble);
        ofSetColor(ofColor::lightGray, bgOpacity.val());
        font.drawString(ofToString(bloodGlucoseValue), bgTextPosition.x, bgTextPosition.y);
        if(testMode) {
            testModeFont.drawString("~ TEST MODE ~", bgTextPosition.x - 10, bgTextPosition.y - 174);
        }
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
