#include "ofxLayerMask.h"
#include "ofxTexturePlane.h"
#include "ofxAnimatableFloat.h"

class animation {
    
public:
    void setup(float _width, float _height, bool _testMode);
    void update();
    void draw();
    void keyPressed(int key);
    void newBgValue(int _bloodGlucoseValue);
    int currentTestBg();
    
protected:
    int makeUsable(int _bloodGlucoseValue);
    string className;
    ofxLayerMask masker;
    ofxTexturePlane wall, wallMask, hyper;

    int bloodGlucoseValue, bgLo, bgHi;
    ofxTexturePlane mask, layer;
    vector< vector<ofxTexturePlane> > masks;
    vector<ofxTexturePlane> layers, maskLoader;
    int numBloodLayers, numMasksPerBloodLayer, numOverlayLayers;
    int overlayLayer1, overlayLayer2;
    float scale, opacity, increment;
    float layerIncrement, maskIncrement;
    float wallIncrementX, wallIncrementY, wallMaskIncrementY;

    ofxAnimatableFloat refWallIncrementX, refWallIncrementY, refWallMaskIncrementY;
    float wallIncrementXLo, wallIncrementXHi;
    float wallIncrementYLo, wallIncrementYHi;
    float wallMaskIncrementYLo, wallMaskIncrementYHi;

    ofxAnimatableFloat wallMaskScale;
    float wallMaskScaleHi, wallMaskScaleLo;
    float currentMaskWidth;

    ofxAnimatableFloat refLayerIncrement, refMaskIncrement;
    float layerIncrementLo, layerIncrementHi;
    float maskIncrementLo, maskIncrementHi;

    ofxAnimatableFloat refOverlayIntensity, refOverlayAlpha;
    float overlayIntensityLo, overlayIntensityHi;
    float overlayAlphaLo, overlayAlphaHi;
    
    float width, height, halfWidth, halfHeight, quarterWidth;
    unsigned long long timeSinceLastFrame;

    bool testMode;
    float refTestModeIncrement, sinTestModeIncrement;
};