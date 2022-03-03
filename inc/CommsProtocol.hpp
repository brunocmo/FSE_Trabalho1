#ifndef COMMSPROTOCOL_HPP
#define COMMSPROTOCOL_HPP

#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <termios.h>
#include <bitset>
#include <iostream>
#include "crc16.h"

class CommsProtocol {

    private:
        int uart0_filestream;
        std::string palavra;
        struct termios options;
        char enderecoDispositivo;
        char codigoFuncao;
        char subCodigo;
        char matricula[4];
        unsigned char palavraDeEnvio[255];

    public:

        CommsProtocol();
        ~CommsProtocol();

        bool init();

        bool solicitar(int flag);
        bool solicitarTemperaturaInterna();
        bool solicitarTemperaturaPotenciometro();
        bool lerComandosDoUsuario();

        bool enviarInformacao(int numeroCaracteres);
        bool receberInformacao(int flag);

        void pedidoInteiro();
        void pedidoReal();
        void pedidoString();

        void enviarInteiro(int inteiroEnviado);
        void enviarReal(float floatEnviado);
        void enviarString(std::string stringEnviado);

        void escolherDispositivo(char numeroDispositivo);
        void escolherFuncao(char codigoFuncao);


        void enviar(std::string solicitacao);
        

        // Getters and Setters
        int get_uart0_filestream();
        void set_uart0_filestream( int uart0_filestream );

        char get_enderecoDispositivo();
        void set_enderecoDispositivo( char enderecoDispositivo );

        char get_codigoFuncao();
        void set_codigoFuncao( char codigoFuncao );

        char get_subCodigo();
        void set_subCodigo( char subCodigo );

        char* get_matricula();
        void set_matricula( char *matricula );
};




#endif