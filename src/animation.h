#include "ofxLayerMask.h"
#include "ofxTexturePlane.h"
#include "ofxAnimatableFloat.h"

class animation {
    
public:
    void setup(float _width, float _height);
    void update();
    void draw();
    void keyPressed(int key);
    void newBgValue(int _bloodGlucoseValue);
    
    ofxLayerMask masker;
    ofxTexturePlane wall, wallMask;

    //Blood work
    int bloodGlucoseValue, bgLo, bgHi;
    ofxTexturePlane mask, layer;
    vector< vector<ofxTexturePlane> > masks;
    vector<ofxTexturePlane> layers, maskLoader;
    int numLayers, numMasksPerLayer;
    float scale, opacity, increment;
    float layerIncrement, maskIncrement;
    float wallIncrementX, wallIncrementY, wallMaskIncrementY;
    float refWallIncrementX, refWallIncrementY, refWallMaskIncrementY;

    ofxAnimatableFloat wallMaskScale;
    float wallMaskScaleHi, wallMaskScaleLo;
    float currentMaskWidth;

    ofxAnimatableFloat refLayerIncrement, refMaskIncrement;
    float layerIncrementLo, layerIncrementHi;
    float maskIncrementLo, maskIncrementHi;
    
    float width, height, halfWidth, halfHeight, quarterWidth;
    unsigned long long timeSinceLastFrame;
};