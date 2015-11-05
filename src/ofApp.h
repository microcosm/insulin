#pragma once


#include "ofMain.h"
#include "ofxJSON.h"


class ofApp: public ofBaseApp
{
    public:
    void setup();
    void draw();
    string normalizeFloatingPointStr(double value);
    void printValueTree(Json::Value& value, const std::string& path="root", int depth=0);

    ofxJSONElement response;
    Json::Value root;
    string url;
};
