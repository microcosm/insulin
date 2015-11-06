#include "ofxLayerMask.h"
#include "ofxTexturePlane.h"

class animation {
    
public:
    void setup();
    void update();
    void draw();
    
    ofxLayerMask masker;
    ofxTexturePlane card1, card2, card3, card4;
    ofxTexturePlane glass1, glass2, glass3, glass4;
    float halfHeight;
};