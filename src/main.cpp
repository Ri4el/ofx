#include "ofMain.h"
#include "ofApp.h"

vector<float> l_buff;
vector<float> r_buff;
ofSoundStream sound_stream;
float rms;
float lpeak;
float rpeak;
int buffer_counter;
int sample_rate;
int fft_size;
int n_bands;
int buffer_size;

//========================================================================
int main( ){
    
	ofSetupOpenGL(1024,768, OF_FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
