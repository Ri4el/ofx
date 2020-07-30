//
//  soundPolyLine.hpp
//  of004
//
//  Created by Gabriele on 28/07/2020.
//

#ifndef soundPolyLine_h
#define soundPolyLine_h

#include <stdio.h>
#include "ofMain.h"

class soundPolyLine {
    typedef queue<vector<float>> bufferQueue;
private:
    bufferQueue _q;
    int _size;
public:
    soundPolyLine();
    soundPolyLine(int);
    void enqueue();
    void render();
    void dequeue();
    int getSize();
};

#endif /* soundPolyLine_hpp */
