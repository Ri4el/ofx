//
//  soundPolyLine.cpp
//  of004
//
//  Created by Gabriele on 28/07/2020.
//

#include "soundPolyLine.h"

soundPolyLine::soundPolyLine() : _size{15} {}
soundPolyLine::soundPolyLine(int size) : _size{size} {}

void soundPolyLine::render() {
    ofBackground(0);
    float rms_scaled = ofMap(rms, 0, 0.25, 0, 255);
    float ang_step = 2*PI/(buffer_size/4);
    float c = rms_scaled;
    for (size_t x = 0; x < _q.size(); ++x) {
        vector<float> buff = _q.front();
        dequeue();
        enqueue();
        ofNoFill();
        ofPushMatrix();
            ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
            ofSetColor(255);
            ofSetLineWidth(2);
            if(rms_scaled > 200) {ofSetColor(255, 0, 0);}
            if(rms_scaled > 250) {ofFill();}
            if ((int)ofRandom(37) % 6 == 0) {
                ofBeginShape();
                for (size_t i = 0; i < buffer_size; ++i) {
                    float a = cos(ang_step*i);
                    float b = sin(ang_step*i);
                    if (abs(a) < 1e-3) a = 0;
                    if (abs(b) < 1e-3) b = 0;
                    //std::cout << a*((buff[i])*100 + c/10 + (x*50)) << " " << b*((buff[i])*100 + c/10 + (x*50))  << std::endl;
                    ofVertex(a*((buff[i])*100 + c/10 + (x*50)), b*((buff[i])*100 + c/10 + (x*50)));
                }
                ofVertex(cos(ang_step*256)*((buff[255])*100 + c/10 + (x*50)), sin(ang_step*256)*((buff[255])*100 + c/10 + (x*50)));
                ofEndShape();
            }
        ofPopMatrix();
    }
}

void soundPolyLine::enqueue() {
    std::cout << _q.size() << std::endl;
    vector<float> tmp = r_buff;
    transform(l_buff.begin(), l_buff.end(), r_buff.begin(), tmp.begin(), plus<float>());
    _q.push(tmp);
}

void soundPolyLine::dequeue() {
    if (!_q.empty()) {
        _q.pop();
    }
}

int soundPolyLine::getSize() {
    return _size;
}
