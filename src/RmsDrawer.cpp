#include "RmsDrawer.h"

RmsDrawer::RmsDrawer() {
    font.load("SF-Pro-Text-Regular.otf", 11);
    rms_string = ofToString(rms, 3);
    stepx = font.stringWidth(rms_string) + 9;
    stepy = font.stringHeight(rms_string) + 7;
    w_count = 0;
    h_count = 0;
}

size_t RmsDrawer::get_stepx() const {
    return stepx;
}
size_t RmsDrawer::get_stepy() const {
    return stepy;
}

void RmsDrawer::render() {
    //rescale rms value in range [0, 255]
    float rms_scaled = ofMap(rms, 0, 0.25, 0, 255);
    //lr_mapped is a fraction of the total width of the canvas, proportional to the sum of the peaks.
    float drawable_width = ofMap(lpeak+rpeak, 0, 0.3e-5, ofGetWidth()/3, ofGetWidth());
    //convert rms value to string with 3 decimals precision
    rms_string = ofToString(rms, 3);
    
    int h_positions = drawable_width/stepx;
    int v_positions = ofGetHeight()/stepy;
    
    //space left to move drawing places horizontally
    int window = ofGetWidth()/stepx - h_positions;
    //offset_width is set to zero or max.
    int offset_width = lpeak > rpeak ? 0 : window - 30;
    
    for (size_t i = 0; i < 30; ++i) {
        if (rms_scaled > 220) {
            ofSetColor(255, 0, 0);
            if ((int)ofRandom(3) % 2 == 0) {
                font.drawString(rms_string, 10 + (w_count + offset_width)*stepx, 17 + h_count*stepy);
            }
            w_count++;
        } else {
            if (rms_scaled < 200) {
                ofSetColor((rms_scaled/200)*255);
                font.drawString(rms_string, 10 + (w_count + offset_width)*stepx, 17 + h_count*stepy);
            }
             w_count++;
        }
        
         if(lpeak+rpeak < 0.1e-6) {w_count++;}

         if(rms_scaled > 254) {ofBackground(0);}
        
        if (w_count > h_positions) {
            w_count = 0;
            h_count++;
            if (h_count > v_positions) {
                h_count = 0;
                //refresh
                ofBackground(0);
            }
        }
    }
}
