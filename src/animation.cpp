#include "animation.h"

void animation::setup(){
    halfWidth = ofGetWidth() * 0.5;
    card1.setup("card-1.png", 1.0, TEXTURE_OFFSET_MIDDLE_RIGHT);
    card2.setup("card-1.png", 1.0, TEXTURE_OFFSET_MIDDLE_RIGHT);
    card3.setup("card-1.png", 1.0, TEXTURE_OFFSET_MIDDLE_RIGHT);
    card4.setup("card-mask.png", 0.75, TEXTURE_OFFSET__TOP__RIGHT_TO_CENTER);
    glass1.setup("glass-1.png", 0.4, TEXTURE_OFFSET_TOP_LEFT);
    glass2.setup("glass-2.png", 150.0, TEXTURE_OFFSET_BOTTOM_LEFT);
    glass3.setup("glass-2.png", 110.0, TEXTURE_OFFSET_BOTTOM_RIGHT);
    glass4.setup("glass-2.png", 120.0, TEXTURE_OFFSET_TOP_LEFT);
    masker.setup(4, ISOLATE_LAYERS);
}

void animation::update(){
    ofSetColor(ofColor::white);
    masker.beginLayer(0);
    {
        ofSetColor(ofColor::red);
        card1.incrementTextureOffsetY(0.003);
        card1.draw();
    }
    masker.endLayer(0);
    
    masker.beginMask(0);
    {
        glass2.incrementTextureOffsetY(0.005);
        glass2.incrementTextureScale(0.1);
        glass2.draw();
    }
    masker.endMask(0);
    
    masker.beginLayer(1);
    {
        ofSetColor(ofColor::red);
        card2.incrementTextureOffsetY(0.002);
        card2.draw();
    }
    masker.endLayer(1);
    
    masker.beginMask(1);
    {
        glass3.incrementTextureOffsetY(0.002);
        glass3.incrementTextureScale(0.05);
        glass3.draw();
    }
    masker.endMask(1);
    
    masker.beginLayer(2);
    {
        ofSetColor(ofColor::red);
        card3.incrementTextureOffsetY(0.005);
        card3.draw();
    }
    masker.endLayer(2);
    
    masker.beginMask(2);
    {
        glass4.incrementTextureOffsetY(0.008);
        glass4.incrementTextureScale(0.14);
        glass4.draw();
    }
    masker.endMask(2);
    
    masker.beginLayer(3);
    {
        glass1.incrementTextureOffset(0.004, -0.0025);
        glass1.incrementTextureScale(0.014);
        glass1.draw(-halfWidth, 0);
        glass1.draw(halfWidth, 0, TEXTURE_FLIP_VERTICAL);
    }
    masker.endLayer(3);
    
    masker.beginMask(3);
    {
        card4.incrementTextureScale(0.0075);
        card4.incrementTextureOffsetY(0.004);
        card4.draw();
    }
    masker.endMask(3);
}

void animation::draw(){
    ofSetColor(ofColor::white);
    card1.draw();
    masker.draw();
    masker.drawOverlay();
}

void animation::keyPressed(int key) {
    if(key == 'o') {
        masker.toggleOverlay();
    }
}