#include "ofApp.h"

void ofApp::setup() {
    recordMode = false;
    testMode = true;

    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_NOTICE);

    width = 720; height = 1280;
    video.setVideoCodec("mpeg4");
    video.setVideoBitrate("10000k");
    video.setup("insulin-render.mov", width, height, 60, 44100, 0);
    video.start();
    fbo.allocate(width, height, GL_RGB);

    ofLogToConsole();
    ofSetLogLevel("inAnimation", OF_LOG_NOTICE);
    ofSetWindowShape(width, height);

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
    ofBackground(ofColor::black);
    ofSetColor(ofColor::white);

    if(recordMode) {
        fbo.begin();
    }

    {
        anim.draw();
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

    if(recordMode) {
        fbo.end();
        ofDrawBitmapString("RECORDING at framerate: " + ofToString(ofGetFrameRate()), 25, 25);

        fbo.readToPixels(pixels);
        bool success = video.addFrame(pixels);
        if (!success) {
            ofLogWarning("Frame " + ofToString(ofGetFrameNum()) + " was not added");
        }

        if (video.hasVideoError()) {
            ofLogWarning("The video recorder failed to write some frames");
        }

        if (video.hasAudioError()) {
            ofLogWarning("The video recorder failed to write some audio samples");
        }
    } else {
        ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), 25, 25);
    }
}

void ofApp::keyPressed(int key) {
    anim.keyPressed(key);
}

void ofApp::exit() {
    jsonParser.stopThread();
    video.close();
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
