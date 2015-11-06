#pragma once

#include "ofMain.h"
#include "jsonParse.h"
#include "animation.h"
#include "ofxAnimatableFloat.h"

class ofApp: public ofBaseApp
{
    public:
    void setup();
    void update();
    void draw();
    void exit();

    jsonParse jsonParser;
    animation anim;
    int bloodGlucoseValue;
    ofVec2f bgTextPosition, bgTextOffset;
    ofVec2f bgBoxPosition, bgBoxOffset;
    ofVec2f boxSize;
    float halfWidth, boxBorderSize, boxBorderDouble;
    ofTrueTypeFont font;
    ofxAnimatableFloat bgOpacity;
    bool newValueDetected;
};