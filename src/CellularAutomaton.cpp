#include <stdio.h>
#include "CellularAutomaton.h"
#include "ofApp.h"
#include "AudioGlobalVariables.h"

#define LIVE 1
#define DYING 2
#define DEAD 3

CellularAutomaton::CellularAutomaton() : _gen{0}, _n_elem{10}, _state{0}, _size{10}, _n_bands{64} {}


CellularAutomaton::CellularAutomaton(int n_elem) : _gen{0}, _n_elem{n_elem}, _state{0}, _size{10}, _n_bands{64} {
    _state = new int** [n_elem];
    for (int i = 0; i < n_elem; ++i) {
        _state[i] = new int* [n_elem];
    }
    for (int i = 0; i < n_elem; ++i) {
        for (int j = 0; j < n_elem; ++j) {
            _state[i][j] = new int[n_elem];
        }
    }
    for (int i = 0; i < n_elem; ++i) {
        for (int j = 0; j < n_elem; ++j) {
            for (int k = 0; k < n_elem; ++k) {
                _state[i][j][k] = ofRandom(1, 4);
            }
        }
    }
}


CellularAutomaton::CellularAutomaton(int n_elem, int size, int bands)
    : _gen{0}, _n_elem{n_elem}, _state{0}, _size{size}, _n_bands{bands} {
    _state = new int** [n_elem];
    for (int i = 0; i < n_elem; ++i) {
        _state[i] = new int* [n_elem];
    }
    for (int i = 0; i < n_elem; ++i) {
        for (int j = 0; j < n_elem; ++j) {
            _state[i][j] = new int[n_elem];
        }
    }
    for (int i = 0; i < n_elem; ++i) {
        for (int j = 0; j < n_elem; ++j) {
            for (int k = 0; k < n_elem; ++k) {
                _state[i][j][k] = ofRandom(1, 4);
            }
        }
    }
}


int CellularAutomaton::get_elem() const {
    return _n_elem;
}


int CellularAutomaton::get_gen() const {
    return _gen;
}


int**& CellularAutomaton::operator[](unsigned int ind) {
    return _state[ind];
}


int** const& CellularAutomaton::operator[](unsigned int ind) const {
    return _state[ind];
}


//Checks neighborhood applying custom rule
bool CellularAutomaton::check_neigh(int i, int j, int k) const {
    int c = 0;
    for (int n = -1; n < 2; ++n) {
        for (int m = -1; m < 2; ++m) {
            for (int h = -1; h < 2; ++h) {
                if (_state[(i + n + _n_elem) % _n_elem]
                          [(j + m + _n_elem) % _n_elem]
                          [(k + h + _n_elem) % _n_elem] == LIVE) {
                    ++c;
                }
            }
        }
    }
    return c > 5;
}


void CellularAutomaton::render() const {
    
    float rms_scaled = ofMap(rms, 0, 0.25, 0, 255);
    
    //initialize random offsets
    double offset_vec[18] = {
        ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3),
        ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3),
        ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3), ofRandom(-3, 3)
    };

    int width, height, depth;
    width = height = depth = _size * _n_elem;

    //draw cells
    for (int i = 0; i != _n_elem; ++i) {
        for (int j = 0; j != _n_elem; ++j) {
            for (int k = 0; k != _n_elem; ++k) {
                switch (_state[i][j][k]) {
                       case LIVE:
                           ofSetColor(255);
                           ofDrawTriangle(-width  / 2 + (width  / _n_elem) * i - offset_vec[12],
                                          -height / 2 + (height / _n_elem) * j - offset_vec[13],
                                          -depth  / 2 + (depth  / _n_elem) * k - offset_vec[14],
                                          -width  / 2 + (width  / _n_elem) * i - offset_vec[0],
                                          -height / 2 + (height / _n_elem) * j - offset_vec[1],
                                          -depth  / 2 + (depth  / _n_elem) * k + offset_vec[2],
                                          -width  / 2 + (width  / _n_elem) * i - offset_vec[3],
                                          -height / 2 + (height / _n_elem) * j - offset_vec[4],
                                          -depth  / 2 + (depth  / _n_elem) * k - offset_vec[5]);
                           break;
                       case DYING:
                        ofSetColor(255, 0, 0);
                        if (rms_scaled > 210) {
                             ofDrawTriangle(-width  / 2 + (width  / _n_elem) * i - offset_vec[15],
                                            -height / 2 + (height / _n_elem) * j - offset_vec[16],
                                            -depth  / 2 + (depth  / _n_elem) * k - offset_vec[17],
                                            -width  / 2 + (width  / _n_elem) * i - offset_vec[6],
                                            -height / 2 + (height / _n_elem) * j - offset_vec[7],
                                            -depth  / 2 + (depth  / _n_elem) * k + offset_vec[8],
                                            -width  / 2 + (width  / _n_elem) * i - offset_vec[9],
                                            -height / 2 + (height / _n_elem) * j - offset_vec[10],
                                            -depth  / 2 + (depth  / _n_elem) * k - offset_vec[11]);
                        }
                           break;
                        case DEAD:
                            break;
                       default:
                           break;
                }
            }
        }
    }
}


void CellularAutomaton::update(float* freq) {
    _gen++;
    float rms_scaled = ofMap(rms, 0, 0.25, 0, 255);
    for (int i = 0; i != _n_elem; ++i) {
        for (int j = 0; j != _n_elem; ++j) {
            for (int k = 0; k != _n_elem; ++k) {
                switch (_state[i][j][k]) {
                       case LIVE:  //LIVE  -> DYING
                            _state[i][j][k] = DYING;
                            break;
                       case DYING: //DYING -> DEAD
                            _state[i][j][k] = DEAD;
                            break;
                        case DEAD: //DEAD  -> DEAD/ALIVE
                            _state[i][j][k] = check_neigh(i, j, k) ? LIVE : DEAD;
                            break;
                       default:
                           break;
                }
            }
        }
    }
    if (rms_scaled > 190) writeSpectrum(freq);
}


void CellularAutomaton::reset() {
    _gen = 0;
    for (int i = 0; i < _n_elem; ++i) {
        for (int j = 0; j < _n_elem; ++j) {
            for (int k = 0; k < _n_elem; ++k) {
                _state[i][j][k] = ofRandom(1, 4);
            }
        }
    }
}


void CellularAutomaton::writeSpectrum(float* freq) {
    static double c = 0;
    int s = _n_elem/(_n_bands);
    int r = ofRandom(3);
    int st = 0;
    for (int i = 0; i < _n_bands; i++) {
        if (abs(freq[i]) > c) {
            st = LIVE;
            c = abs(freq[i]);
        }
        else {
            st = DEAD;
            if (c > 1) c -= 1;
        }
        for(int j = 0; j < _n_elem; ++j) {
            for (int k = 0; k < _n_elem; ++k) {
                switch (r) {
                    case 0:
                        _state[s * (i)][j][k] = st;
                        break;
                    case 1:
                        _state[j][k][s * (i)] = st;
                        break;
                    case 2:
                        _state[j][s * (i)][k] = st;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

CellularAutomaton::~CellularAutomaton() {
    for (int i = 0; i < _n_elem; ++i) {
        for (int j = 0; j < _n_elem; ++j) {
            delete[] _state[i][j];
        }
    }
    for (int i = 0; i < _n_elem; ++i) {
        delete[] _state[i];
    }
    delete[] _state;
}
