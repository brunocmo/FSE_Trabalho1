#include "../inc/RegistrarInformacoes.hpp"

RegistrarInformacoes::RegistrarInformacoes() {
    std::strcpy( dataHora, "                   ");
    this->tempInterna = 0;
    this->tempExterna = 0;
    this->tempReferencia = 0;
    this->valorPotenciometro = 0;


    std::ofstream arquivo ("doc/log.csv", std::ios::trunc );

    if(arquivo.is_open()) {
        arquivo << "Data/Hora" << ", ";
        arquivo << "Temperatura Interna" << ", ";
        arquivo << "Temperatura Externa" << ", ";
        arquivo << "Temperatura Referencia" << ", ";
        arquivo << "Valor Potenciômetro" << '\n';
        arquivo.close();
    } else std::cout << "Impossibilitado de abrir o arquivo!" << '\n';


}

RegistrarInformacoes::~RegistrarInformacoes() {}

void RegistrarInformacoes::registrarInformacoes() {

    std::ofstream arquivo ("doc/log.csv", std::ios::app );

    if(arquivo.is_open()) {
        arquivo << get_dataHora() << ",";
        arquivo << std::fixed << std::setprecision(1) << get_tempInterna() << ",";
        arquivo << std::fixed << std::setprecision(1) << get_tempExterna() << ",";
        arquivo << std::fixed << std::setprecision(1) << get_tempReferencia() << ",";
        arquivo << std::fixed << std::setprecision(1) << get_valorPotenciometro() << '\n';
        arquivo.close();
    } else std::cout << "Impossibilitado de abrir o arquivo!" << '\n';

}

char * RegistrarInformacoes::get_dataHora() {
    return this->dataHora;
}

void RegistrarInformacoes::set_dataHora( char * dataHora ) {
    std::strcpy( this->dataHora, dataHora );
}

float RegistrarInformacoes::get_tempInterna() {
    return this->tempInterna;
}

void RegistrarInformacoes::set_tempInterna( float tempInterna ) {
    this->tempInterna = tempInterna;
}

float RegistrarInformacoes::get_tempExterna() {
    return this->tempExterna;
}

void RegistrarInformacoes::set_tempExterna( float tempExterna ) {
    this->tempExterna = tempExterna;
}

float RegistrarInformacoes::get_tempReferencia() {
    return this->tempReferencia;
}

void RegistrarInformacoes::set_tempReferencia( float tempReferencia ) {
    this->tempReferencia = tempReferencia;
}

int RegistrarInformacoes::get_valorPotenciometro() {
    return this->valorPotenciometro;
}

void RegistrarInformacoes::set_valorPotenciometro( int valorPotenciometro ) {
    this->valorPotenciometro = valorPotenciometro;
}