#pragma once

#include "ofMain.h"
#include "RmsDrawer.h"
#include "ofxMaxim.h"
#include "CA.h"
#include "soundPolyLine.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
    
        void keyPressed(int key);
        void debugPanelDraw();
    
        void audioIn(ofSoundBuffer & input);
        void getSoundStreamInfo(ofSoundStream &input);
        float getPeakValue(vector<float>& buff);
        
        void renderCA();
        void renderRmsDrawer();
        void renderSoundPolyLine();
        
        maxiFFT fft;
        maxiFFTOctaveAnalyzer oct;
        CA* automaton;
        RmsDrawer rms_drawer;
        soundPolyLine sound_poly;
    
        ofFbo fb1;
        ofFbo fb2;
        ofFbo fb3;
        
        ofBlendMode blend_mode;
        bool is_debug_on;
        int border_width;
    
};
