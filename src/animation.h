#include "ofxLayerMask.h"
#include "ofxTexturePlane.h"

class animation {
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    ofxLayerMask masker;
    ofxTexturePlane wall, wallMask;

    //Blood work
    ofxTexturePlane mask, layer;
    vector< vector<ofxTexturePlane> > masks;
    vector<ofxTexturePlane> layers, maskLoader;
    int numLayers, numMasksPerLayer;
    float scale, opacity, increment, layerIncrement, maskIncrement;
    
    float halfWidth;
};