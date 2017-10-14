#pragma once

#include "ofMain.h"
#include "inJsonParser.h"
#include "inAnimation.h"
#include "ofxAnimatableFloat.h"
#include "ofxVideoRecorder.h"

class ofApp: public ofBaseApp
{
    public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void exit();
    void startTimer();
    void animateWhenTimerFinished();

protected:
    bool recordMode, testMode;
    inJsonParser jsonParser;
    inAnimation anim;
    int bloodGlucoseValue;
    ofVec2f bgTextPosition, bgTextOffset;
    ofVec2f bgBoxPosition, bgBoxOffset;
    ofVec2f boxSize;
    float width, height;
    float halfWidth, boxBorderSize, boxBorderDouble;
    ofTrueTypeFont font, testModeFont;
    ofxAnimatableFloat bgOpacity;
    bool newValueDetected, timerRunning;
    unsigned long long timerStartedAt;
    float delayBeforeAnimation, animationInSecs, animationOutSecs;

    ofxVideoRecorder video;
    ofFbo fbo;
    ofPixels pixels;
};
