#ifndef CURVAREFLOW_HPP
#define CURVAREFLOW_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class CurvaReflow {

public:
    std::vector<int> tempo;
    std::vector<float> temperatura;

    CurvaReflow();
    ~CurvaReflow();

    void carregarValores();
    void mostrarValores();

};

#endif