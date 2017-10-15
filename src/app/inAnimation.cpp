#include "inAnimation.h"

void inAnimation::setup(float _width, float _height, bool _testMode){
    className = "inAnimation";
    testMode = _testMode;
    refTestModeIncrement = 0.0001;

    width = _width;
    height = _height;
    halfWidth = width * 0.5;
    halfHeight = height * 0.5;
    quarterWidth = width * 0.25;

    bloodGlucoseValue = -1;
    bgLo = 40; bgHi = 275;
    bgHypo = 75; bgHyper = 165;
    transitionDuration = 30;
    
    wallMaskScaleLo = 0.45;
    wallMaskScaleHi = 0.06;
    wallMaskScale.reset(0.35);
    wallMaskScale.setDuration(transitionDuration);
    wallMaskScale.setRepeatType(PLAY_ONCE);
    wallMaskScale.setCurve(EASE_IN_EASE_OUT);

    wallMask.setup("wallMask.png", width, height, wallMaskScale.val(), TEXTURE_OFFSET__TOP__RIGHT_TO_CENTER);
    wall.setup("glass-1.png", width, height, 1.0, TEXTURE_OFFSET_TOP_LEFT);
    hyper.setup("glass-3.png", width, height, 1.0, TEXTURE_OFFSET_MIDDLE_CENTER);
    hyperMask.setup("glass-4.png", width, height, 1.0, TEXTURE_OFFSET_MIDDLE_CENTER);
    hypoAlpha = 0;
    hyperAlpha = 255;
    framesSinceOverlayReset = 0;
    framesBeforeRepeat = 20;
    timeOfLastBeat = 0;
    beatAlpha.reset(0);
    beatAlpha.setCurve(LINEAR);
    beatAlpha.setRepeatType(PLAY_ONCE);
    
    //Blood speed
    layerIncrementLo = 0.3;
    layerIncrementHi = 0.05;
    refLayerIncrement.reset(0.4);
    refLayerIncrement.setDuration(transitionDuration);
    refLayerIncrement.setRepeatType(PLAY_ONCE);
    refLayerIncrement.setCurve(EASE_IN_EASE_OUT);
    
    maskIncrementLo = 0.06;
    maskIncrementHi = 0.0005;
    refMaskIncrement.reset(0.01);
    refMaskIncrement.setDuration(transitionDuration);
    refMaskIncrement.setRepeatType(PLAY_ONCE);
    refMaskIncrement.setCurve(EASE_IN_EASE_OUT);
    
    //Wall speed
    wallIncrementXLo = 0.2;
    wallIncrementXHi = 0.01;
    refWallIncrementX.reset(0.1);
    refWallIncrementX.setDuration(transitionDuration);
    refWallIncrementX.setRepeatType(PLAY_ONCE);
    refWallIncrementX.setCurve(EASE_IN_EASE_OUT);

    wallIncrementYLo = 0.35;
    wallIncrementYHi = 0.06;
    refWallIncrementY.reset(-0.1);
    refWallIncrementY.setDuration(transitionDuration);
    refWallIncrementY.setRepeatType(PLAY_ONCE);
    refWallIncrementY.setCurve(EASE_IN_EASE_OUT);
    
    wallMaskIncrementYLo = 0.1;
    wallMaskIncrementYHi = 0.06;
    refWallMaskIncrementY.reset(0.08);
    refWallMaskIncrementY.setDuration(transitionDuration);
    refWallMaskIncrementY.setRepeatType(PLAY_ONCE);
    refWallMaskIncrementY.setCurve(EASE_IN_EASE_OUT);

    //Overlay
    overlayIntensityLo = 1.0;
    overlayIntensityHi = 0.9;
    refOverlayIntensity.reset(1);
    refOverlayIntensity.setDuration(transitionDuration);
    refOverlayIntensity.setRepeatType(PLAY_ONCE);
    refOverlayIntensity.setCurve(EASE_IN_EASE_OUT);

    overlayAlphaLo = -400;
    overlayAlphaHi = 400;
    refOverlayAlpha.reset(0);
    refOverlayAlpha.setDuration(transitionDuration);
    refOverlayAlpha.setRepeatType(PLAY_ONCE);
    refOverlayAlpha.setCurve(EASE_IN_EASE_OUT);

    //Masks and layers
    numBloodLayers = 6;
    numMasksPerBloodLayer = 2;
    numOverlayLayers = 2;
    overlayLayer1 = numBloodLayers;
    overlayLayer2 = numBloodLayers + 1;
    masker.setup(width, height, numBloodLayers + numOverlayLayers, ISOLATE_LAYERS);
    
    layer.setup("glass-3.png", width, height);
    for(int i = 0; i < numBloodLayers; i++) {
        scale = ofMap(i, 0, numBloodLayers-1, 4, 2.5);
        layer.setTextureScale(scale);
        layer.setTextureOffset(i % 2 == 0 ? TEXTURE_OFFSET__MIDDLE__LEFT_TO_CENTER : TEXTURE_OFFSET__MIDDLE__RIGHT_TO_CENTER);
        layer.setTextureOffsetY(ofRandom(2));
        layers.push_back(layer);
        
        maskLoader.clear();
        scale = ofMap(i, 0, numBloodLayers-1, 6, 0.33);
        mask.setup("tissue.png", width, height, scale, TEXTURE_OFFSET__MIDDLE__LEFT_TO_CENTER);
        for(int j = 0; j < numMasksPerBloodLayer; j++) {
            mask.setTextureOffsetY(ofRandom(2));
            maskLoader.push_back(mask);
        }
        masks.push_back(maskLoader);
    }
}

void inAnimation::update(){
    if(testMode) {
        sinTestModeIncrement = sin(ofGetElapsedTimeMillis() * refTestModeIncrement);
        bloodGlucoseValue = ofMap(sinTestModeIncrement, -1, 1, bgLo, bgHi);
        wallMaskScale.reset(ofMap(sinTestModeIncrement, -1, 1, wallMaskScaleLo, wallMaskScaleHi));
        refLayerIncrement.reset(ofMap(sinTestModeIncrement, -1, 1, layerIncrementLo, layerIncrementHi));
        refMaskIncrement.reset(ofMap(sinTestModeIncrement, -1, 1, maskIncrementLo, maskIncrementHi));
        refWallIncrementX.reset(ofMap(sinTestModeIncrement, -1, 1, wallIncrementXLo, wallIncrementXHi));
        refWallIncrementY.reset(ofMap(sinTestModeIncrement, -1, 1, wallIncrementYLo, wallIncrementYHi));
        refWallMaskIncrementY.reset(ofMap(sinTestModeIncrement, -1, 1, wallMaskIncrementYLo, wallMaskIncrementYHi));
        refOverlayIntensity.reset(ofMap(sinTestModeIncrement, -1, 1, overlayIntensityLo, overlayIntensityHi));
        refOverlayAlpha.reset(ofMap(sinTestModeIncrement, -1, 1, overlayAlphaLo, overlayAlphaHi));
    } else if(ofGetFrameNum() > 0) {
        wallMaskScale.update(ofGetLastFrameTime());
        refLayerIncrement.update(ofGetLastFrameTime());
        refMaskIncrement.update(ofGetLastFrameTime());
        refWallIncrementX.update(ofGetLastFrameTime());
        refWallIncrementY.update(ofGetLastFrameTime());
        refWallMaskIncrementY.update(ofGetLastFrameTime());
        refOverlayIntensity.update(ofGetLastFrameTime());
        refOverlayAlpha.update(ofGetLastFrameTime());
    }
    beatAlpha.update(ofGetLastFrameTime());

    if(bloodGlucoseValue > -1 || testMode) {
        ofSetColor(ofColor::white);
        layerIncrement = refLayerIncrement.val() * ofGetLastFrameTime();
        maskIncrement = refMaskIncrement.val() * ofGetLastFrameTime();

        for(int i = 0; i < numBloodLayers; i++) {
            masker.beginLayer(i);
            {
                ofSetColor(ofColor::red);
                increment = ofMap(i, 0, numBloodLayers-1, layerIncrement, -layerIncrement);
                layers.at(i).incrementTextureOffsetY(increment);
                layers.at(i).draw();
            }
            masker.endLayer(i);
            
            masker.beginMask(i);
            {
                ofSetColor(ofColor::white);
                ofBackground(ofColor::white);
                for(int j = 0; j < numMasksPerBloodLayer; j++) {
                    increment = (j+1) * ofMap(i, 0, numBloodLayers-1, maskIncrement, -maskIncrement);
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
        
        masker.beginLayer(overlayLayer1);
        {
            ofSetColor(ofColor::white, (refOverlayAlpha.val() + 340) * 3);
            wall.incrementTextureOffset(wallIncrementX, wallIncrementY);
            //wall.incrementTextureScale(0.014);
            wall.draw(-halfWidth, 0);
            wall.draw(halfWidth, 0, TEXTURE_FLIP_VERTICAL);
            ofSetColor(ofColor::red, refOverlayAlpha.val() * 0.34);
            ofDrawRectangle(0, 0, width, height);
        }
        masker.endLayer(overlayLayer1);
        
        masker.beginMask(overlayLayer1);
        {
            wallMask.setTextureScale(wallMaskScale.val());
            wallMask.incrementTextureOffsetY(wallMaskIncrementY);
            wallMask.draw();
            ofSetColor(ofColor::white);
            currentMaskWidth = ofMap(wallMaskScale.val(), wallMaskScaleLo, wallMaskScaleHi, -190, 370);
            ofDrawRectangle(0, 0, currentMaskWidth, height);
            ofDrawRectangle(width, 0, -currentMaskWidth, height);
        }
        masker.endMask(overlayLayer1);

        //Overlays
        if(bloodGlucoseValue > bgHyper) {
            //Animate hyper
            if(refOverlayIntensity.val() < ofRandom(1) && framesSinceOverlayReset > framesBeforeRepeat) {
                hyperMask.setTexturePosition(ofRandom(2), ofRandom(2));
                hyperMask.setTextureScale(ofRandom(3.5, 7.5));
                hyper.setTexturePosition(ofRandom(2), ofRandom(2));
                hyper.setTextureScale(ofRandom(2.5, 6.5));
                hyperAlpha = 255;
                framesSinceOverlayReset = 0;
            } else {
                framesSinceOverlayReset++;
                hyperAlpha -= 5;
                hyper.incrementTextureScale(0.45 * ofGetLastFrameTime());
                hyper.incrementTextureOffsetY(-0.02 * ofGetLastFrameTime());
                hyperMask.incrementTextureScale(0.7 * ofGetLastFrameTime());
                hyperMask.incrementTextureOffsetY(0.02 * ofGetLastFrameTime());
            }

            //Draw hyper
            masker.beginLayer(overlayLayer2);
            {
                ofSetColor(170, 255, 255, hyperAlpha);
                hyper.draw();
            }
            masker.endLayer(overlayLayer2);

        } else if (bloodGlucoseValue < bgHypo) {
            //Animate hypo
            timeBetweenBeats = ofMap(bloodGlucoseValue, bgLo, bgHi, 500, 2700);
            beatDuration = ofMap(bloodGlucoseValue, bgLo, bgHi, 0.4, -1.0);
            if(ofGetElapsedTimeMillis() > timeOfLastBeat + timeBetweenBeats) {
                timeOfLastBeat = ofGetElapsedTimeMillis();
                beatAlpha.setDuration(0.05);
                beatAlpha.animateFromTo(0, 255);
            } else if(beatAlpha.val() == 255) {
                beatAlpha.setDuration(beatDuration);
                beatAlpha.animateFromTo(255, 0);
            }

            //Draw hypo
            masker.beginLayer(overlayLayer2);
            {
                ofBackground(ofColor::red);
            }
            masker.endLayer(overlayLayer2);
        }

        //Draw hypo and hyper masks
        masker.beginMask(overlayLayer2);
        {
            if(bloodGlucoseValue > bgHyper) {
                ofBackground(ofColor::black);
                ofSetColor(ofColor::white, refOverlayAlpha.val());
                hyperMask.draw();
            } else {
                ofBackground(ofColor::black);
                ofSetColor(ofColor(ofColor::white, beatAlpha.val()));
                ofDrawRectangle(0, 0, width, height);
            }
        }
        masker.endMask(overlayLayer2);
    }

    ofLogVerbose(className)
         << wallMaskScale.val() << " "
         << refLayerIncrement.val() << " "
         << refMaskIncrement.val() << " "
         << refWallIncrementX.val() <<  " "
         << refWallIncrementY.val() <<  " "
         << refWallMaskIncrementY.val();
}

void inAnimation::draw() {
    if(bloodGlucoseValue > -1 || testMode) {
        ofSetColor(ofColor::red);
        layers.at(0).draw();
        masker.draw();
        masker.drawOverlay();
    }
}

void inAnimation::keyPressed(int key) {
    if(key == 'o') {
        masker.toggleOverlay();
    }
}

void inAnimation::newBgValue(int _bloodGlucoseValue) {
    ofLogNotice(className) << "Recieved new value: " << _bloodGlucoseValue;
    if(!testMode) {
        if(bloodGlucoseValue == -1) {
            bloodGlucoseValue = makeUsable(_bloodGlucoseValue);
            wallMaskScale.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, wallMaskScaleLo, wallMaskScaleHi));
            refLayerIncrement.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, layerIncrementLo, layerIncrementHi));
            refMaskIncrement.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, maskIncrementLo, maskIncrementHi));
            refWallIncrementX.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, wallIncrementXLo, wallIncrementXHi));
            refWallIncrementY.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, wallIncrementYLo, wallIncrementYHi));
            refWallMaskIncrementY.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, wallMaskIncrementYLo, wallMaskIncrementYHi));
            refOverlayIntensity.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, overlayIntensityLo, overlayIntensityHi));
            refOverlayAlpha.reset(ofMap(bloodGlucoseValue, bgLo, bgHi, overlayAlphaLo, overlayAlphaHi));
        } else {
            bloodGlucoseValue = makeUsable(_bloodGlucoseValue);
            wallMaskScale.animateFromTo(wallMaskScale.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, wallMaskScaleLo, wallMaskScaleHi));
            refLayerIncrement.animateFromTo(refLayerIncrement.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, layerIncrementLo, layerIncrementHi));
            refMaskIncrement.animateFromTo(refMaskIncrement.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, maskIncrementLo, maskIncrementHi));
            refWallIncrementX.animateFromTo(refWallIncrementX.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, wallIncrementXLo, wallIncrementXHi));
            refWallIncrementY.animateFromTo(refWallIncrementY.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, wallIncrementYLo, wallIncrementYHi));
            refWallMaskIncrementY.animateFromTo(refWallMaskIncrementY.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, wallMaskIncrementYLo, wallMaskIncrementYHi));
            refOverlayIntensity.animateFromTo(refOverlayIntensity.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, overlayIntensityLo, overlayIntensityHi));
            refOverlayAlpha.animateFromTo(refOverlayAlpha.val(), ofMap(bloodGlucoseValue, bgLo, bgHi, overlayAlphaLo, overlayAlphaHi));
        }
    }
}

int inAnimation::makeUsable(int _bloodGlucoseValue) {
    if(_bloodGlucoseValue < 20) {
        return 20;
    } else if(_bloodGlucoseValue > 270) {
        return 270;
    }
    return _bloodGlucoseValue;
}

int inAnimation::currentTestBg() {
    return bloodGlucoseValue;
}
