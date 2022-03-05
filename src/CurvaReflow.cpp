#include "../inc/CurvaReflow.hpp"

CurvaReflow::CurvaReflow() {
    tempo.clear();
    temperatura.clear();
}

CurvaReflow::~CurvaReflow() {}

void CurvaReflow::carregarValores() {

    char c;
    std::string palavra;

    std::ifstream arquivo("doc/curva_reflow.csv");

    if( arquivo.is_open()) {

        arquivo.seekg(0, arquivo.end);
        arquivo.seekg(0, arquivo.beg);

        arquivo.seekg( 23, arquivo.cur );

        while( arquivo.get(c) ) {
            
            if ( c != ',' && c != '\n')
                palavra.push_back(c);
            else {
                if ( c == ',' ) {
                    tempo.push_back( std::stoi(palavra) );
                    palavra.clear();
                } else {
                    temperatura.push_back( std::stof(palavra) );
                    palavra.clear();
                }
            }

        }

        arquivo.close();
    } else {
        std::cout << "Não achei o arquivo!" << '\n';
    }
}

void CurvaReflow::mostrarValores() {
    for( int tp : tempo) {
        std::cout << "Tempo: " << tp << '\n';
    }
    for( int tempe : temperatura) {
        std::cout << "Temperatura: " << tempe << '\n';
    }
}