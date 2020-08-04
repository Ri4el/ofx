//
//  CA.h
//  of002
//

#ifndef CA_h
#define CA_h

class CellularAutomaton {
private:
    int _n_elem; //n of elements x side
    int _gen; //generation
    int ***_state; //state
    int _size; //size of the elements
    void writeSpectrum(float*);
    int _n_bands;
public:
    CellularAutomaton();
    CellularAutomaton(int);
    CellularAutomaton(int, int, int);
    int get_elem() const;
    int get_gen() const;
    bool check_neigh(int, int, int) const;
    void update(float*);
    void reset();
    void render() const;
    int**& operator[](unsigned int);
    int** const& operator[](unsigned int) const;
    ~CellularAutomaton();
};

#endif /* CA_h */
