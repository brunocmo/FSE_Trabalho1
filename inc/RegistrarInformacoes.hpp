#ifndef REGISTRARINFORMACOES_HPP
#define REGISTRARINFORMACOES_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>

class RegistrarInformacoes {
private:

    char dataHora[18];
    float tempInterna;
    float tempExterna;
    float tempReferencia;
    int valorPotenciometro;

public:
    RegistrarInformacoes();
    ~RegistrarInformacoes();

    void registrarInformacoes();

    char * get_dataHora();
    void set_dataHora( char * dataHora );

    float get_tempInterna();
    void set_tempInterna( float tempInterna );

    float get_tempExterna();
    void set_tempExterna( float tempExterna );

    float get_tempReferencia();
    void set_tempReferencia( float tempReferencia );

    int get_valorPotenciometro();
    void set_valorPotenciometro( int valorPotenciometro );

    
};

#endif