#ifndef FORNO_HPP
#define FORNO_HPP

#include <iostream>
#include <signal.h>
#include <chrono>
#include <ctime>
#include "CommsProtocol.hpp"
#include "TemperatureStatus.hpp"
#include "ShowInfoLCD.hpp"
#include "ControleTemperatura.hpp"
#include "pid.hpp"
#include "CurvaReflow.hpp"
#include "RegistrarInformacoes.hpp"

extern bool executar;

void tratarSinal(int s);

class Forno {

public:

    static const unsigned char DESLIGAR{0x00};
    static const unsigned char LIGAR{0x01};
    static const unsigned char MODO_UART{0x00};
    static const unsigned char MODO_TERMINAL{0x01};

    ControleTemperatura * controleDaTemperatura = new ControleTemperatura();
	ShowInfoLCD * lcd = new ShowInfoLCD();
	TemperatureStatus * tempAmbiente = new TemperatureStatus();
	CommsProtocol * uart = new CommsProtocol();
	CurvaReflow referenciaReflow;
	RegistrarInformacoes registro;

	int iteradorReflow{0};

	char tempoString[18];
	struct tm * timeinfo;

	char telaModoUart[16];
	char telaModoTerminal[16];

	char sistemaTelaAcima[16];
	char sistemaTelaAbaixo[16];

	char sistemaDesligadoAcima[16];
	char sistemaDesligadoAbaixo[16];

    float curvaTemperaturaAnterior;
	float temperaturaReferencia;
	bool modoUART = true;

    double pid_Kp;
    double pid_Ki;
    double pid_Kd;

	std::chrono::time_point<std::chrono::system_clock> comecaCronometro;
	std::chrono::time_point<std::chrono::system_clock>  cronometroReflow;
	std::chrono::time_point<std::chrono::system_clock>  voltaCronometro;
	std::time_t comeca_tempo;
	std::chrono::duration<double> tempoPassado;

	int sinalControle;
	bool sistemaLigado;

    Forno();
    ~Forno();

    bool executarSistema();
    bool executarSistema( float temperaturaReferenciaMenu );

    void configurarParametrosPID();

    void configurarParametrosViaHostname(  char * hostname );

    float interpolarReferencia( int x_anterior, int x_posterior, int x_atual, float t_anterior, float t_posterior);

    double get_pid_Kp();
    void set_pid_Kp( double pid_Kp );

    double get_pid_Ki();
    void set_pid_Ki( double pid_Ki );

    double get_pid_Kd();
    void set_pid_Kd( double pid_Kd );

};

#endif