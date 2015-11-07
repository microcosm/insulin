#include "animation.h"

void animation::setup(){
    halfWidth = ofGetWidth() * 0.5;

    wallMask.setup("wallMask.png", 0.3, TEXTURE_OFFSET__TOP__RIGHT_TO_CENTER);
    wall.setup("glass-1.png", 1.0, TEXTURE_OFFSET_TOP_LEFT);

    refLayerIncrement = 0.5;
    refMaskIncrement = 0.02;
    
    refWallIncrementX = 0.2;
    refWallIncrementY = -0.25;
    refWallMaskIncrementY = 0.1;
    
    numLayers = 6;
    numMasksPerLayer = 2;
    masker.setup(numLayers + 1, ISOLATE_LAYERS);
    
    layer.setup("glass-3.png");
    for(int i = 0; i < numLayers; i++) {
        scale = ofMap(i, 0, numLayers-1, 4, 2.5);
        layer.setTextureScale(scale);
        layer.setTextureOffset(i % 2 == 0 ? TEXTURE_OFFSET__MIDDLE__LEFT_TO_CENTER : TEXTURE_OFFSET__MIDDLE__RIGHT_TO_CENTER);
        layer.setTextureOffsetY(ofRandom(2));
        layers.push_back(layer);
        
        maskLoader.clear();
        scale = ofMap(i, 0, numLayers-1, 6, 0.33);
        mask.setup("tissue.png", scale, TEXTURE_OFFSET__MIDDLE__LEFT_TO_CENTER);
        for(int j = 0; j < numMasksPerLayer; j++) {
            mask.setTextureOffsetY(ofRandom(2));
            maskLoader.push_back(mask);
        }
        masks.push_back(maskLoader);
    }
}

void animation::update(){
    ofSetColor(ofColor::white);
    layerIncrement = refLayerIncrement * ofGetLastFrameTime();
    maskIncrement = refMaskIncrement * ofGetLastFrameTime();

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
        //wallMask.incrementTextureScale(0.0075);
        wallMask.incrementTextureOffsetY(wallMaskIncrementY);
        wallMask.draw();
    }
    masker.endMask(numLayers);
}

void animation::draw(){
    ofSetColor(ofColor::red);
    layers.at(0).draw();
    masker.draw();
    masker.drawOverlay();
}

void animation::keyPressed(int key) {
    if(key == 'o') {
        masker.toggleOverlay();
    }
}