#include <iostream>
#include <signal.h>
#include "../inc/CommsProtocol.hpp"
#include "../inc/TemperatureStatus.hpp"
#include "../inc/ShowInfoLCD.hpp"
#include "../inc/ControleTemperatura.hpp"
#include "../inc/pid.hpp"
#include "../inc/CurvaReflow.hpp"

bool executar{true};

void tratarSinal(int s){
	printf(" Fechando o programa... \n");
	executar = false;
}

int main() {

	ControleTemperatura controleDaTemperatura;
	ShowInfoLCD * lcd = new ShowInfoLCD();
	TemperatureStatus * tempAmbiente = new TemperatureStatus();
	CommsProtocol * uart = new CommsProtocol();
	CurvaReflow referenciaReflow;

	int iteradorReflow{0};
	int tempoReflow{0};

	char telaModoUart[16] = "Modo: UART     ";
	char telaModoTerminal[16] = "Modo: Terminal ";

	char sistemaTelaAcima[16] = "  Modo: UART   ";
	char sistemaTelaAbaixo[16];

	char sistemaDesligadoAcima[16] = "   Desligado   ";
	char sistemaDesligadoAbaixo[16] = "      OFF      ";

	float temperaturaReferencia{28.00f};
	bool modoUART = true;

	float valorTemperaturaAmbiente{(float)tempAmbiente->get_temperatura()};

	int sinalControle{0};
	bool sistemaLigado{false};
	signal(SIGINT, tratarSinal);

	pid_configura_constantes( 30.0, 0.2, 400.0 );

	lcd->set_mensagemAcima16( sistemaDesligadoAcima );
	lcd->set_mensagemAbaixo16( sistemaDesligadoAbaixo );
	lcd->mostrarMensagem();

	uart->enviarDisplayControle( 0x00 );
	
	lcd->set_mensagemAcima16(sistemaTelaAcima);
	uart->enviarSinalDeReferencia( temperaturaReferencia );

	referenciaReflow.carregarValores();

	while(executar) {

		uart->lerComandosDoUsuario();
		uart->solicitarTemperaturaInterna();
		uart->solicitarTemperaturaPotenciometro();

		sleep(1);

		uart->receberInformacao( 0xC3 );
		uart->receberInformacao( 0xC1 );
		uart->receberInformacao( 0xC2 );

		switch( uart->get_codigoRetorno() ) {
			case 1:
				uart->enviarDisplayEstadoSistema( 0x01 );
				sleep(1);
				uart->receberInformacao( 0xD3 );
				sistemaLigado = true;
				break;
			case 2: 
				uart->enviarDisplayEstadoSistema( 0x00 );
				sleep(1);
				uart->receberInformacao( 0xD3 );
				lcd->set_mensagemAcima16(sistemaDesligadoAcima);
				lcd->set_mensagemAbaixo16(sistemaDesligadoAbaixo);
				lcd->mostrarMensagem();
				sistemaLigado = false;
				break;
			case 3:
				uart->enviarDisplayControle( 0x00 );
				sleep(1);
				uart->receberInformacao( 0xD4 );
				std::strcpy( sistemaTelaAcima, telaModoUart );
				lcd->set_mensagemAcima16(sistemaTelaAcima);
				modoUART = true;
				break;
			case 4:
				uart->enviarDisplayControle( 0x01 );
				sleep(1);
				uart->receberInformacao( 0xD4 );
				std::strcpy( sistemaTelaAcima, telaModoTerminal );
				lcd->set_mensagemAcima16(sistemaTelaAcima);
				tempoReflow = 0;
				iteradorReflow = 0;
				modoUART = false;
			default: break;	
		}

		if( sistemaLigado ) {

			// uart->solicitarTemperaturaInterna();
			if (!modoUART) {
			
				if( tempoReflow == referenciaReflow.tempo.at(iteradorReflow) ) {
					std::cout << "Mudando temperatura!" << '\n';
					temperaturaReferencia = referenciaReflow.temperatura.at(iteradorReflow);
					uart->enviarSinalDeReferencia(temperaturaReferencia);

					iteradorReflow++;
				}				

				uart->set_temperaturaReferencia(temperaturaReferencia);
				
			}

			pid_atualiza_referencia( uart->get_temperaturaReferencia() );
			sinalControle = (int)pid_controle( uart->get_temperaturaInterna() );

			uart->enviarSinalDeControle( sinalControle );

			controleDaTemperatura.mudarTemperatura(sinalControle);

			std::sprintf( sistemaTelaAbaixo, "%.1f %.1f %.1f", 
				uart->get_temperaturaInterna(),
				uart->get_temperaturaReferencia(), 
				valorTemperaturaAmbiente
			);

			// lcd->set_mensagemAbaixo16(sistemaTelaAbaixo);
			// lcd->mostrarMensagem();
			
			std::cout << "Tempo: " << tempoReflow << "\n";

			tempoReflow++;

		}
	}

	delete(lcd);
	delete(tempAmbiente);
	delete(uart);

	return EXIT_SUCCESS;
}
