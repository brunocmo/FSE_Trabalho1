#include "../inc/CommsProtocol.hpp"


CommsProtocol::CommsProtocol() {
    this->uart0_filestream = -1;
    this->palavra = "";
    this->enderecoDispositivo = 0x01;
    this->matricula[0] = 7;
    this->matricula[1] = 5;
    this->matricula[2] = 4;
    this->matricula[3] = 8;

    init();

}

CommsProtocol::~CommsProtocol() {
    close(get_uart0_filestream());
}

bool CommsProtocol::init() {
    set_uart0_filestream( open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY) );

    if ( get_uart0_filestream() == -1 ) {
        printf("Erro na abertura da UART!\n");
        return EXIT_FAILURE;
    }

    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    // printf("Abertura e configuração realizadas!\n");
    return EXIT_SUCCESS;
}

bool CommsProtocol::solicitar(int flag) {
    
    switch( flag ) {
        case 1 : 
            set_codigoFuncao((char)0x23);  
            set_subCodigo((char)0xC1); 
            break;
        case 2 : 
            set_codigoFuncao((char)0x23);  
            set_subCodigo((char)0xC2);   
            break;
        case 3 : 
            set_codigoFuncao((char)0x23);  
            set_subCodigo((char)0xC3); 
            break;
        default: return EXIT_FAILURE;
    }    
    
    unsigned char insercaoCRC[7] {
        (unsigned char)get_enderecoDispositivo(),
        (unsigned char)get_codigoFuncao(),
        (unsigned char)get_subCodigo(),
        (unsigned char)*(get_matricula()+0),
        (unsigned char)*(get_matricula()+1),
        (unsigned char)*(get_matricula()+2),
        (unsigned char)*(get_matricula()+3)
    };
    short crcDeEnvio = calcula_CRC(&insercaoCRC[0], 7);
    unsigned char crcEmChar[2];

    std::memcpy(crcEmChar, &crcDeEnvio, sizeof(short));

    for(int i{0}; i<7; i++) {
        this->palavraDeEnvio[i] = insercaoCRC[i];
    }

    this->palavraDeEnvio[7] = crcEmChar[0];
    this->palavraDeEnvio[8] = crcEmChar[1];
    this->palavraDeEnvio[9] = '\0';

    enviarInformacao(9);

    return EXIT_SUCCESS;
}

bool CommsProtocol::solicitarTemperaturaInterna() {
    return solicitar(1);
}

bool CommsProtocol::solicitarTemperaturaPotenciometro() {
    return solicitar(2);
}

bool CommsProtocol::lerComandosDoUsuario() {
    return solicitar(3);
}

bool CommsProtocol::enviarSinal(int flag) {

    unsigned char valorSinal[4];

    switch( flag ) {
        case 1 : 
            set_codigoFuncao((char)0x16);  
            set_subCodigo((char)0xD1); 
            std::memcpy(valorSinal, &valorSinalControle, sizeof(int));
            break;
        case 2 : 
            set_codigoFuncao((char)0x16);  
            set_subCodigo((char)0xD2);
            std::memcpy(valorSinal, &valorSinalReferencia, sizeof(int));  
            break;
        default: return EXIT_FAILURE;
    }  
  
    unsigned char insercaoCRC[11] {
        (unsigned char)get_enderecoDispositivo(),
        (unsigned char)get_codigoFuncao(),
        (unsigned char)get_subCodigo(),
        (unsigned char)*(get_matricula()+0),
        (unsigned char)*(get_matricula()+1),
        (unsigned char)*(get_matricula()+2),
        (unsigned char)*(get_matricula()+3),
        valorSinal[0],
        valorSinal[1],
        valorSinal[2],
        valorSinal[3]
    };
    short crcDeEnvio = calcula_CRC(&insercaoCRC[0], 11);
    unsigned char crcEmChar[2];

    std::memcpy(crcEmChar, &crcDeEnvio, sizeof(short));

    for(int i{0}; i<11; i++) {
        this->palavraDeEnvio[i] = insercaoCRC[i];
    }

    this->palavraDeEnvio[11] = crcEmChar[0];
    this->palavraDeEnvio[12] = crcEmChar[1];
    this->palavraDeEnvio[13] = '\0';

    enviarInformacao(13);

    return EXIT_SUCCESS;
}

bool CommsProtocol::enviarSinalDeControle( int valorSinalControle ) {
    this->valorSinalControle = valorSinalControle;
    enviarSinal(1);

    return EXIT_SUCCESS;
}

bool CommsProtocol::enviarSinalDeReferencia( float valorSinalReferencia ) {
    this->valorSinalReferencia = valorSinalReferencia;
    enviarSinal(2);

    return EXIT_SUCCESS;
}

bool CommsProtocol::enviarDisplay( int flag ) {
    switch( flag ) {
        case 1 : 
            set_codigoFuncao((char)0x16);  
            set_subCodigo((char)0xD3); 
            break;
        case 2 : 
            set_codigoFuncao((char)0x16);  
            set_subCodigo((char)0xD4);
            break;
        default: return EXIT_FAILURE;
    }  
  
    unsigned char insercaoCRC[8] {
        (unsigned char)get_enderecoDispositivo(),
        (unsigned char)get_codigoFuncao(),
        (unsigned char)get_subCodigo(),
        (unsigned char)*(get_matricula()+0),
        (unsigned char)*(get_matricula()+1),
        (unsigned char)*(get_matricula()+2),
        (unsigned char)*(get_matricula()+3),
        get_modo()
    };
    short crcDeEnvio = calcula_CRC(&insercaoCRC[0], 8);
    unsigned char crcEmChar[2];

    std::memcpy(crcEmChar, &crcDeEnvio, sizeof(short));

    for(int i{0}; i<11; i++) {
        this->palavraDeEnvio[i] = insercaoCRC[i];
    }

    this->palavraDeEnvio[8] = crcEmChar[0];
    this->palavraDeEnvio[9] = crcEmChar[1];
    this->palavraDeEnvio[10] = '\0';

    enviarInformacao(10);

    return EXIT_SUCCESS;
}

bool CommsProtocol::enviarDisplayEstadoSistema( unsigned char modo ) {
    set_modo(modo);
    enviarDisplay(1);

    return EXIT_SUCCESS;
}

bool CommsProtocol::enviarDisplayControle( unsigned char modo ) {
    set_modo(modo);
    enviarDisplay(2);

    return EXIT_SUCCESS;
}

bool CommsProtocol::enviarInformacao(int numeroCaracteres) {

    // for( unsigned char letra: palavraDeEnvio) {
    //     if( letra == '\0') break;
    //     printf("%X \n", letra);
    // }

    int contador = write( get_uart0_filestream(), &(this->palavraDeEnvio), numeroCaracteres);

    if(contador < 0) {
        printf("Erro no envio de dados - TX\n");
    }

    if( palavraDeEnvio[2] == 0xD1 || palavraDeEnvio[2] == 0xD2 ) 
        return EXIT_SUCCESS;
    else receberInformacao(palavraDeEnvio[2]);

    return EXIT_SUCCESS;
}

bool CommsProtocol::receberInformacao(unsigned char flag) {
    
    int valorInteiro{0};
    float valorPontoFlut{0.0f};
    short crc;
    unsigned char crcPreVerificador[255];
    unsigned char verificador[2];
    short crcRecebido;
    int rx_length;
    unsigned char rx_buffer[100];

    if(get_uart0_filestream() != -1) {

        usleep(600);

        rx_length = read(get_uart0_filestream(), (void*)rx_buffer, 100);
        if(rx_length < 0){
            printf("Erro na leitura da UART - RX\n");
        }
        else if(rx_length == 0){
            printf("Nenhum dado disponível\n");
        } else {

            for(int k{0}; k<(rx_length-2); k++) {
                crcPreVerificador[k] = (unsigned char)rx_buffer[k];
            }

            verificador[0] = rx_buffer[(rx_length-2)];
            verificador[1] = rx_buffer[(rx_length-1)];

            std::memcpy(&crcRecebido, &verificador[0], sizeof(short));

            crc = calcula_CRC(&crcPreVerificador[0], (rx_length-2));
                    
            if( crcRecebido == crc ) {

                if( (flag >> 4) == 0x0C ) {

                    if ( flag == 0xC3 ) {
                        std::memcpy(&valorInteiro, &rx_buffer[3], sizeof(int));
                        set_codigoRetorno(valorInteiro);
                    } else {
                        std::memcpy(&valorPontoFlut, &rx_buffer[3], sizeof(int));
                        if( flag == (unsigned char)0xC1) {
                            set_temperaturaInterna(valorPontoFlut);
                        } else {
                            set_temperaturaReferencia(valorPontoFlut);
                        }
                    }

                } else {
                    std::memcpy(&valorInteiro, &rx_buffer[3], sizeof(int));

                    printf("Mensagem de comprimento %d: %d\n", rx_length, valorInteiro);
                }

            } else {
                printf("CRC falhou na verificao");
            }

        }
    }

    return EXIT_SUCCESS;
}



int CommsProtocol::get_uart0_filestream() {
    return uart0_filestream;
}

void CommsProtocol::set_uart0_filestream( int uart0_filestream ) {
    this->uart0_filestream = uart0_filestream;
}

char CommsProtocol::get_enderecoDispositivo() {
    return this->enderecoDispositivo;
}

void CommsProtocol::set_enderecoDispositivo( char enderecoDispositivo ) {
    this->enderecoDispositivo = enderecoDispositivo;
}

char CommsProtocol::get_codigoFuncao() {
    return this->codigoFuncao;
}

void CommsProtocol::set_codigoFuncao( char codigoFuncao ) {
    this->codigoFuncao = codigoFuncao;
}

char CommsProtocol::get_subCodigo() {
    return this->subCodigo;
}

void CommsProtocol::set_subCodigo( char subCodigo ) {
    this->subCodigo = subCodigo;
}

char* CommsProtocol::get_matricula() {
    return this->matricula;
}

void CommsProtocol::set_matricula( char *matricula ) {
    std::strcpy( this->matricula, matricula );
}

float CommsProtocol::get_temperaturaInterna() {
    return this->temperaturaInterna;
}

void CommsProtocol::set_temperaturaInterna( float temperaturaInterna ) {
    this->temperaturaInterna = temperaturaInterna;
}

float CommsProtocol::get_temperaturaReferencia() {
    return this->temperaturaReferencia;
}

void CommsProtocol::set_temperaturaReferencia( float temperaturaReferencia ) {
    this->temperaturaReferencia = temperaturaReferencia;
}

unsigned char CommsProtocol::get_modo() {
    return this->modo;
}

void CommsProtocol::set_modo( unsigned char modo ) {
    this->modo = modo;
}

int CommsProtocol::get_codigoRetorno() {
    return this->codigoRetorno;
}

void CommsProtocol::set_codigoRetorno( int codigoRetorno ) {
    this->codigoRetorno = codigoRetorno;
}
