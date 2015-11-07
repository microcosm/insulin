#include "animation.h"

void animation::setup(float _width, float _height){
    width = _width;
    height = _height;
    halfWidth = width * 0.5;
    halfHeight = height * 0.5;
    quarterWidth = width * 0.25;

    bloodGlucoseValue = -1;
    bgLo = 70;
    bgHi = 250;

    wallMaskScaleHi = 0.15;
    wallMaskScaleLo = 0.5;
    wallMaskScale.reset(0.35);
    wallMaskScale.setDuration(120);
    wallMaskScale.setRepeatType(PLAY_ONCE);
    wallMaskScale.setCurve(EASE_IN_EASE_OUT);

    wallMask.setup("wallMask.png", width, height, wallMaskScale.val(), TEXTURE_OFFSET__TOP__RIGHT_TO_CENTER);
    wall.setup("glass-1.png", width, height, 1.0, TEXTURE_OFFSET_TOP_LEFT);
    
    //Blood speed
    layerIncrementHi = 0.15;
    layerIncrementLo = 0.5;
    refLayerIncrement.reset(0.4);
    refLayerIncrement.setDuration(120);
    refLayerIncrement.setRepeatType(PLAY_ONCE);
    refLayerIncrement.setCurve(EASE_IN_EASE_OUT);
    
    maskIncrementHi = 0.005;
    maskIncrementLo = 0.002;
    refMaskIncrement.reset(0.01);
    refMaskIncrement.setDuration(120);
    refMaskIncrement.setRepeatType(PLAY_ONCE);
    refMaskIncrement.setCurve(EASE_IN_EASE_OUT);
    
    refWallIncrementX = 0.2;
    refWallIncrementY = -0.25;
    refWallMaskIncrementY = 0.1;
    
    numLayers = 6;
    numMasksPerLayer = 2;
    masker.setup(width, height, numLayers + 1, ISOLATE_LAYERS);
    
    layer.setup("glass-3.png", width, height);
    for(int i = 0; i < numLayers; i++) {
        scale = ofMap(i, 0, numLayers-1, 4, 2.5);
        layer.setTextureScale(scale);
        layer.setTextureOffset(i % 2 == 0 ? TEXTURE_OFFSET__MIDDLE__LEFT_TO_CENTER : TEXTURE_OFFSET__MIDDLE__RIGHT_TO_CENTER);
        layer.setTextureOffsetY(ofRandom(2));
        layers.push_back(layer);
        
        maskLoader.clear();
        scale = ofMap(i, 0, numLayers-1, 6, 0.33);
        mask.setup("tissue.png", width, height, scale, TEXTURE_OFFSET__MIDDLE__LEFT_TO_CENTER);
        for(int j = 0; j < numMasksPerLayer; j++) {
            mask.setTextureOffsetY(ofRandom(2));
            maskLoader.push_back(mask);
        }
        masks.push_back(maskLoader);
    }
}

void animation::update(){
    wallMaskScale.update(ofGetLastFrameTime());
    refLayerIncrement.update(ofGetLastFrameTime());
    refMaskIncrement.update(ofGetLastFrameTime());

    if(bloodGlucoseValue > -1) {
        ofSetColor(ofColor::white);
        layerIncrement = refLayerIncrement.val() * ofGetLastFrameTime();
        maskIncrement = refMaskIncrement.val() * ofGetLastFrameTime();

        for(int i = 0; i < numLayers; i++) {
            masker.beginLayer(i);
            {
                ofSetColor(ofColor::red);
                increment = ofMap(i, 0, numLayers-1, layerIncrement, -layerIncrement);
                layers.at(i).incrementTextureOffsetY(increment);
                layers.at(i).draw();
            }
            masker.endLayer(i);
            
            masker.beginMask(i);
            {
                ofSetColor(ofColor::white);
                ofBackground(ofColor::white);
                for(int j = 0; j < numMasksPerLayer; j++) {
                    increment = (j+1) * ofMap(i, 0, numLayers-1, maskIncrement, -maskIncrement);
                    masks.at(i).at(j).incrementTextureOffsetY(increment);
                    masks.at(i).at(j).draw();
                }
            }
            masker.endMask(i);
        }
        
        //Walls
        wallIncrementX = refWallIncrementX * ofGetLastFrameTime();
        wallIncrementY = refWallIncrementY * ofGetLastFrameTime();
        wallMaskIncrementY = refWallMaskIncrementY * ofGetLastFrameTime();
        
        masker.beginLayer(numLayers);
        {
            wall.incrementTextureOffset(wallIncrementX, wallIncrementY);
            //wall.incrementTextureScale(0.014);
            wall.draw(-halfWidth, 0);
            wall.draw(halfWidth, 0, TEXTURE_FLIP_VERTICAL);
        }
        masker.endLayer(numLayers);
        
        masker.beginMask(numLayers);
        {
            wallMask.setTextureScale(wallMaskScale.val());
            wallMask.incrementTextureOffsetY(wallMaskIncrementY);
            wallMask.draw();
            ofSetColor(ofColor::white);
            currentMaskWidth = ofMap(wallMaskScale.val(), wallMaskScaleLo, wallMaskScaleHi, -quarterWidth, quarterWidth);
            ofRect(0, 0, currentMaskWidth, height);
            ofRect(width, 0, -currentMaskWidth, height);
        }
        masker.endMask(numLayers);
    }
    cout << wallMaskScale.val() << " " << refLayerIncrement.val() << " " << refMaskIncrement.val() << endl;
}

void animation::draw(){
    if(bloodGlucoseValue > -1) {
        ofSetColor(ofColor::red);
        layers.at(0).draw();
        masker.draw();
        masker.drawOverlay();
    }
}

void animation::keyPressed(int key) {
    if(key == 'o') {
        masker.toggleOverlay();
    }
}

void animation::newBgValue(int _bloodGlucoseValue) {
    bloodGlucoseValue = _bloodGlucoseValue;
    wallMaskScale.animateFromTo(wallMaskScale.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, wallMaskScaleLo, wallMaskScaleHi));
    refLayerIncrement.animateFromTo(refLayerIncrement.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, layerIncrementLo, layerIncrementHi));
    refMaskIncrement.animateFromTo(refMaskIncrement.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, maskIncrementLo, maskIncrementHi));
    cout << "New BG: " << bloodGlucoseValue << endl;
}