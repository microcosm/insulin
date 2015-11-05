#pragma once


#include "ofMain.h"
#include "ofxJSON.h"


class ofApp: public ofBaseApp
{
    public:
    void setup();
    void draw();
    string normalizeFloatingPointStr(double value);
    void printValueTree(Json::Value& value, const std::string& path = ".");

    ofxJSONElement response;
    Json::Value root;
    string url;
};
