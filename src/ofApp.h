#pragma once

#include "ofMain.h"
#include "jsonParse.h"

class ofApp: public ofBaseApp
{
    public:
    void setup();
    void update();
    void draw();
    void exit();

    jsonParse jsonParser;
    int bloodGlucoseValue;
    ofVec2f bgTextPosition;
    ofTrueTypeFont font;
};