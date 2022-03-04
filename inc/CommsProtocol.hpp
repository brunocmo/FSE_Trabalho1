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
#include <thread>
#include <chrono>
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
        float temperaturaInterna;
        float temperaturaReferencia;
        int valorSinalControle;
        float valorSinalReferencia;
        unsigned char modo;
        int codigoRetorno;

    public:

        CommsProtocol();
        ~CommsProtocol();

        bool init();

        bool solicitar(int flag);
        bool solicitarTemperaturaInterna();
        bool solicitarTemperaturaPotenciometro();
        bool lerComandosDoUsuario();

        bool enviarSinal( int flag );
        bool enviarSinalDeControle(int valorSinalControle);
        bool enviarSinalDeReferencia(float valorSinalReferencia);

        bool enviarDisplay( int flag );
        bool enviarDisplayEstadoSistema( unsigned char modo );
        bool enviarDisplayControle( unsigned char modo );

        bool enviarInformacao(int numeroCaracteres);
        bool receberInformacao(unsigned char flag);

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

        float get_temperaturaInterna();
        void set_temperaturaInterna( float temperaturaInterna );

        float get_temperaturaReferencia();
        void set_temperaturaReferencia( float temperaturaReferencia );

        unsigned char get_modo();
        void set_modo( unsigned char modo );

        int get_codigoRetorno();
        void set_codigoRetorno( int codigoRetorno );
};




#endif