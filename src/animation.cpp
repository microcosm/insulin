#include "animation.h"

void animation::setup(float _width, float _height){
    testMode = false;
    testModeIncrement = 0.0003;

    width = _width;
    height = _height;
    halfWidth = width * 0.5;
    halfHeight = height * 0.5;
    quarterWidth = width * 0.25;

    bloodGlucoseValue = -1;
    bgLo = 70; bgHi = 250;
    
    wallMaskScaleLo = 0.4;
    wallMaskScaleHi = 0.15;
    wallMaskScale.reset(0.35);
    wallMaskScale.setDuration(120);
    wallMaskScale.setRepeatType(PLAY_ONCE);
    wallMaskScale.setCurve(EASE_IN_EASE_OUT);

    wallMask.setup("wallMask.png", width, height, wallMaskScale.val(), TEXTURE_OFFSET__TOP__RIGHT_TO_CENTER);
    wall.setup("glass-1.png", width, height, 1.0, TEXTURE_OFFSET_TOP_LEFT);
    
    //Blood speed
    layerIncrementLo = 0.3;
    layerIncrementHi = 0.05;
    refLayerIncrement.reset(0.4);
    refLayerIncrement.setDuration(120);
    refLayerIncrement.setRepeatType(PLAY_ONCE);
    refLayerIncrement.setCurve(EASE_IN_EASE_OUT);
    
    maskIncrementLo = 0.06;
    maskIncrementHi = 0.0005;
    refMaskIncrement.reset(0.01);
    refMaskIncrement.setDuration(120);
    refMaskIncrement.setRepeatType(PLAY_ONCE);
    refMaskIncrement.setCurve(EASE_IN_EASE_OUT);
    
    //Wall speed
    wallIncrementXLo = 0.2;
    wallIncrementXHi = 0.01;
    refWallIncrementX.reset(0.1);
    refWallIncrementX.setDuration(120);
    refWallIncrementX.setRepeatType(PLAY_ONCE);
    refWallIncrementX.setCurve(EASE_IN_EASE_OUT);

    wallIncrementYLo = 0.35;
    wallIncrementYHi = 0.06;
    refWallIncrementY.reset(-0.1);
    refWallIncrementY.setDuration(120);
    refWallIncrementY.setRepeatType(PLAY_ONCE);
    refWallIncrementY.setCurve(EASE_IN_EASE_OUT);
    
    wallMaskIncrementYLo = 0.1;
    wallMaskIncrementYHi = 0.06;
    refWallMaskIncrementY.reset(0.08);
    refWallMaskIncrementY.setDuration(120);
    refWallMaskIncrementY.setRepeatType(PLAY_ONCE);
    refWallMaskIncrementY.setCurve(EASE_IN_EASE_OUT);
    
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
    if(testMode) {
        wallMaskScale.reset(ofMap(sin(ofGetElapsedTimeMillis() * testModeIncrement), -1, 1, wallMaskScaleLo, wallMaskScaleHi));
        refLayerIncrement.reset(ofMap(sin(ofGetElapsedTimeMillis() * testModeIncrement), -1, 1, layerIncrementLo, layerIncrementHi));
        refMaskIncrement.reset(ofMap(sin(ofGetElapsedTimeMillis() * testModeIncrement), -1, 1, maskIncrementLo, maskIncrementHi));
        refWallIncrementX.reset(ofMap(sin(ofGetElapsedTimeMillis() * testModeIncrement), -1, 1, wallIncrementXLo, wallIncrementXHi));
        refWallIncrementY.reset(ofMap(sin(ofGetElapsedTimeMillis() * testModeIncrement), -1, 1, wallIncrementYLo, wallIncrementYHi));
        refWallMaskIncrementY.reset(ofMap(sin(ofGetElapsedTimeMillis() * testModeIncrement), -1, 1, wallMaskIncrementYLo, wallMaskIncrementYHi));
    } else {
        if(ofGetFrameNum() > 0) {
            wallMaskScale.update(ofGetLastFrameTime());
            refLayerIncrement.update(ofGetLastFrameTime());
            refMaskIncrement.update(ofGetLastFrameTime());
            refWallIncrementX.update(ofGetLastFrameTime());
            refWallIncrementY.update(ofGetLastFrameTime());
            refWallMaskIncrementY.update(ofGetLastFrameTime());
        }
    }

    if(bloodGlucoseValue > -1 || testMode) {
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
        wallIncrementX = refWallIncrementX.val() * ofGetLastFrameTime();
        wallIncrementY = refWallIncrementY.val() * ofGetLastFrameTime();
        wallMaskIncrementY = refWallMaskIncrementY.val() * ofGetLastFrameTime();
        
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
            currentMaskWidth = ofMap(wallMaskScale.val(), wallMaskScaleLo, wallMaskScaleHi, 0, quarterWidth);
            ofRect(0, 0, currentMaskWidth, height);
            ofRect(width, 0, -currentMaskWidth, height);
        }
        masker.endMask(numLayers);
    }
    cout << wallMaskScale.val() << " "
         << refLayerIncrement.val() << " "
         << refMaskIncrement.val() << " "
         << refWallIncrementX.val() <<  " "
         << refWallIncrementY.val() <<  " "
         << refWallMaskIncrementY.val() << endl;
}

void animation::draw() {
    if(bloodGlucoseValue > -1 || testMode) {
        ofSetColor(ofColor::red);
        layers.at(0).draw();
        masker.draw();
        masker.drawOverlay();
    }

    if(testMode) {
        ofSetColor(ofColor::gray);
        ofDrawBitmapString(ofToString(ofMap(sin(ofGetElapsedTimeMillis() * testModeIncrement), -1, 1, bgLo, bgHi)), 25, 100);
    }
}

void animation::keyPressed(int key) {
    if(key == 'o') {
        masker.toggleOverlay();
    }
}

void animation::newBgValue(int _bloodGlucoseValue) {
    cout << "Animation recieved new BG: " << _bloodGlucoseValue << endl;
    if(!testMode) {
        if(bloodGlucoseValue == -1) {
            bloodGlucoseValue = makeUsable(_bloodGlucoseValue);
            wallMaskScale.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, wallMaskScaleLo, wallMaskScaleHi));
            refLayerIncrement.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, layerIncrementLo, layerIncrementHi));
            refMaskIncrement.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, maskIncrementLo, maskIncrementHi));
            refWallIncrementX.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, wallIncrementXLo, wallIncrementXHi));
            refWallIncrementY.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, wallIncrementYLo, wallIncrementYHi));
            refWallMaskIncrementY.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, wallMaskIncrementYLo, wallMaskIncrementYHi));
        } else {
            bloodGlucoseValue = makeUsable(_bloodGlucoseValue);
            wallMaskScale.animateFromTo(wallMaskScale.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, wallMaskScaleLo, wallMaskScaleHi));
            refLayerIncrement.animateFromTo(refLayerIncrement.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, layerIncrementLo, layerIncrementHi));
            refMaskIncrement.animateFromTo(refMaskIncrement.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, maskIncrementLo, maskIncrementHi));
            refWallIncrementX.animateFromTo(refWallIncrementX.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, wallIncrementXLo, wallIncrementXHi));
            refWallIncrementY.animateFromTo(refWallIncrementY.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, wallIncrementYLo, wallIncrementYHi));
            refWallMaskIncrementY.animateFromTo(refWallMaskIncrementY.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, wallMaskIncrementYLo, wallMaskIncrementYHi));
        }
    }
}

int animation::makeUsable(int _bloodGlucoseValue) {
    if(_bloodGlucoseValue < 20) {
        return 20;
    } else if(_bloodGlucoseValue > 270) {
        return 270;
    }
    return _bloodGlucoseValue;
}