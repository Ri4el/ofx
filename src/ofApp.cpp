#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true); //VSYNC
    ofEnableAlphaBlending();
    ofBackground(0,0,0);
    ofSetBackgroundAuto(false);
    
    border_width = 60;
    is_debug_on = false;
    blend_mode = OF_BLENDMODE_ADD;
    
    buffer_size = 256;
    l_buff.assign(buffer_size, 0);
    r_buff.assign(buffer_size, 0);
    rms = 0.0;
    buffer_counter = 0.0;
    vector<ofSoundDevice> devices = sound_stream.getDeviceList();
    
    sample_rate = 44100;
    ofxMaxiSettings::setup(sample_rate, 2, buffer_size);
    fft_size = 1024;
    n_bands = 64;
    fft.setup(fft_size, 512, 256);
    oct.setup(sample_rate, 1024, n_bands);
    
    ofSoundStreamSettings settings;
    settings.setInDevice(devices[2]);
    settings.setInListener(this);
    settings.sampleRate = 44100;
    settings.numOutputChannels = 0;
    settings.numInputChannels = 2;
    settings.bufferSize = buffer_size;
    
    sound_stream.setup(settings);
    
    getSoundStreamInfo(sound_stream);
    
    //init automaton
    automaton = new CA(70, 7, n_bands);
    
    //init queue
    for (unsigned int i = 0; i < sound_poly.getSize(); ++i) {
        sound_poly.enqueue();
    }
    
    //allocate and clean framebuffers
    fb1.allocate(ofGetWidth()-border_width*2, ofGetHeight()-border_width*2, GL_RGBA, 8); //MSAA x8
    fb2.allocate(ofGetWidth()-border_width*2, ofGetHeight()-border_width*2, GL_RGBA, 8); //MSAA x8
    
    fb1.begin();
        ofClear(255, 255, 255, 0);
    fb1.end();
    
    fb2.begin();
        ofClear(255, 255, 255, 0);
    fb2.end();
    
}

//--------------------------------------------------------------
void ofApp::update(){
//    fb1.begin();
//        renderRmsDrawer();
//    fb1.end();
//    fb2.begin();
//        renderCA();
//    fb2.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
//    ofEnableBlendMode(blend_mode);
//    fb1.draw(border_width, border_width);
//    fb2.draw(border_width, border_width);
//    ofDisableBlendMode();
    sound_poly.render();
    //PRESS D TO ENABLE DEBUG
    if (is_debug_on) {
        debugPanelDraw();
    }
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'd') {
        is_debug_on = !is_debug_on;
    }
    switch (key) {
        case 49:
            blend_mode = OF_BLENDMODE_ALPHA;
            break;
        case 50:
            blend_mode = OF_BLENDMODE_ADD;
            break;
        case 51:
            blend_mode = OF_BLENDMODE_MULTIPLY;
            break;
        case 52:
            blend_mode = OF_BLENDMODE_SUBTRACT;
            break;
        case 53:
            blend_mode = OF_BLENDMODE_SCREEN;
            break;
        case 54:
           blend_mode = OF_BLENDMODE_DISABLED;
           break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer & input) {
    //input size in samples corresponds to num_frames*num_channels, samples are interleaved
    //LRLRLRLR...
    //input values lie in [-1, 1]
    rms = 0.0;
    for (size_t i = 0; i < input.getNumFrames(); ++i) {
        l_buff[i] = input[i*2];
        r_buff[i] = input[i*2 + 1];
        rms += pow(l_buff[i], 2);
        rms += pow(r_buff[i], 2);
        if(fft.process(l_buff[i] + r_buff[i])){
            oct.calculate(fft.magnitudes);
        }
    }
    rms /= (float)input.getNumFrames()*2; //divide by the total number of samples
    rms = sqrt(rms);
    lpeak = getPeakValue(l_buff);
    rpeak = getPeakValue(r_buff);
    ++buffer_counter;
}
//--------------------------------------------------------------
inline void ofApp::getSoundStreamInfo(ofSoundStream &input) {
    std::cout << "######################" << std::endl;
    std::cout << "SOUND STREAM DATA" << std::endl;
    std::cout << "Buffer size: " << input.getBufferSize() << std::endl;
    std::cout << "Input channels: " << input.getNumInputChannels() << std::endl;
    std::cout << "Output channels: " << input.getNumOutputChannels() << std::endl;
    std::cout << "Sample rate: " << input.getSampleRate() << std::endl;
    std::cout << "######################" << std::endl;
}
//--------------------------------------------------------------
inline bool bufferValueCompare(float x, float y) {
    return pow(x, 2) > pow(y, 2);
}
//--------------------------------------------------------------
inline float ofApp::getPeakValue(vector<float>& buff) {
    return pow(*std::max_element(buff.begin(), buff.end(), bufferValueCompare), 2);
}
//--------------------------------------------------------------
void ofApp::exit() {
    sound_stream.close();
}
//--------------------------------------------------------------
inline void ofApp::renderCA() {
    ofBackground(0);
    ofPushMatrix();
        //CENTER AND ROTATE
        ofTranslate(ofGetWidth()/2-border_width, ofGetHeight()/2-border_width); //look at FBO size to translate accurately
        
        //UPDATE BLOCK
        if (ofGetFrameNum() % 3 == 0) {
            automaton->update(oct.averages);
        }
        
        //Shapes are rendered in 3d, if two shapes overlap the one that lies behind is not rendered.
        ofEnableDepthTest();
        automaton->render();
        ofDisableDepthTest();
    ofPopMatrix();
}
//--------------------------------------------------------------
inline void ofApp::renderRmsDrawer() {
    rms_drawer.render();
}
//--------------------------------------------------------------
inline void ofApp::debugPanelDraw() {
    ofDrawBitmapStringHighlight(ofToString(buffer_counter), 10, 15);
    ofDrawBitmapStringHighlight("lpeak: " + ofToString(lpeak), 10, 35);
    ofDrawBitmapStringHighlight("rpeak: " + ofToString(rpeak), 10, 55);
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 75);
    ofDrawBitmapStringHighlight("FFT", 10, 95);
    ofDrawBitmapStringHighlight("RMS: " + ofToString(ofMap(rms, 0, 0.25, 0, 255)), 10, 691);
    for (size_t i = 0; i < n_bands/2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            ofDrawBitmapStringHighlight(ofToString(floor(oct.averages[i + j])), 10 + 30 * j, 115 + i*18);
        }
    }
}
//--------------------------------------------------------------
