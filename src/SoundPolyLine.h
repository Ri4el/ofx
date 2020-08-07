#ifndef soundPolyLine_h
#define soundPolyLine_h

#include <stdio.h>
#include "ofMain.h"

class SoundPolyLine {
    typedef queue<vector<float>> bufferQueue;
private:
    bufferQueue _q;
    int _size;

public:
    SoundPolyLine();
    SoundPolyLine(int);
    void enqueue();
    void render();
    void dequeue();
    int getSize();
};

#endif /* soundPolyLine_hpp */
