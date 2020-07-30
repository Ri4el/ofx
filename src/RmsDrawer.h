#ifndef RmsDrawer_h
#define RmsDrawer_h

#include "ofMain.h"

class RmsDrawer {
    private:
        size_t stepx;
        size_t stepy;
        size_t w_count;
        size_t h_count;
        string rms_string;
        ofTrueTypeFont font;
    public:
        RmsDrawer();
        size_t get_stepx() const;
        size_t get_stepy() const;
        void render();
};

#endif /* RmsDrawer_h */
