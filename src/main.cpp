#include <iostream>
#include <signal.h>
#include "../inc/CommsProtocol.hpp"
#include "../inc/TemperatureStatus.hpp"
#include "../inc/ShowInfoLCD.hpp"
#include "../inc/ControleTemperatura.hpp"
#include "../inc/pid.hpp"

bool executar{true};

void tratarSinal(int s){
	printf(" Fechando o programa... \n");
	executar = false;
}

int main() {

	// ControleTemperatura controleDaTemperatura;
	ShowInfoLCD * lcd = new ShowInfoLCD();
	TemperatureStatus * tempAmbiente = new TemperatureStatus();
	CommsProtocol * uart = new CommsProtocol();


	char sistemaTelaAcima[16];
	char sistemaTelaAbaixo[16];

	char sistemaDesligadoAcima[20] = "   Desligado   ";
	char sistemaDesligadoAbaixo[20] = "               ";

	int sinalControle{0};
	bool sistemaLigado{false};
	signal(SIGINT, tratarSinal);

	pid_configura_constantes( 30.0, 0.2, 400.0 );

	uart->enviarSinalDeReferencia( 28.00f );

	while(executar) {

		uart->lerComandosDoUsuario();

		switch( uart->get_codigoRetorno() ) {
			case 1:
				uart->enviarDisplayEstadoSistema( 0x01 );
				sistemaLigado = true;
				break;
			case 2: 
				uart->enviarDisplayEstadoSistema( 0x00 );
				// lcd->set_mensagemAcima16(sistemaDesligadoAcima);
				// lcd->set_mensagemAbaixo16(sistemaDesligadoAbaixo);
				// lcd->mostrarMensagem();
				sistemaLigado = false;
				break;
			case 3:
				uart->enviarDisplayControle( 0x00 );
				break;
			case 4:
				uart->enviarDisplayControle( 0x01 );
			default: break;	
		}

		if( sistemaLigado  ) {
			uart->solicitarTemperaturaInterna();
			uart->solicitarTemperaturaPotenciometro();

			pid_atualiza_referencia( uart->get_temperaturaReferencia() );
			sinalControle = (int)pid_controle( uart->get_temperaturaInterna() );

			uart->enviarSinalDeControle( sinalControle );

			// controleDaTemperatura.mudarTemperatura(sinalControle);

			sprintf( sistemaTelaAcima, "TR %.2f TE %.2f", uart->get_temperaturaReferencia(), (float)tempAmbiente->get_temperatura());
			sprintf( sistemaTelaAbaixo, "TI %.2f        ", uart->get_temperaturaInterna());

			printf("===> %s\n", sistemaTelaAcima);
			printf("===> %s\n", sistemaTelaAbaixo);

			// lcd->set_mensagemAcima16( sistemaTelaAcima);
			// lcd->set_mensagemAbaixo16(sistemaTelaAbaixo);
			// lcd->mostrarMensagem();
		}
	}


	char temperaturaAmbiente[16] = "Temp. Ambiente:";
	char tempReferencia[16] = "Temp. Pot.:";
	char tempInterna[16] = "Temp. Interna.:";


	uart->solicitarTemperaturaInterna();
	uart->solicitarTemperaturaPotenciometro();

	printf("A temperatura ambiente   eh: %.4lf\n", tempAmbiente->get_temperatura());
	printf("A temperatura referencia eh: %.4lf\n", uart->get_temperaturaReferencia());
	printf("A temperatura interna    eh: %.4lf\n", uart->get_temperaturaInterna());

	lcd->set_mensagemAcima16(temperaturaAmbiente);
	lcd->typeFloat( (float)(tempAmbiente->get_temperatura()) );

	lcd->mostrarMensagem();

	sleep(3); 

	lcd->set_mensagemAcima16(tempReferencia);
	lcd->typeFloat( uart->get_temperaturaReferencia() );

	lcd->mostrarMensagem();

	sleep(3); 

	lcd->set_mensagemAcima16(tempInterna);
	lcd->typeFloat( uart->get_temperaturaInterna());

	lcd->mostrarMensagem();

	sleep(3);

	std::sprintf( sistemaTelaAcima, "TR %.2f TE %.2f", uart->get_temperaturaReferencia(), (float)tempAmbiente->get_temperatura());
	std::sprintf( sistemaTelaAbaixo, "TI %.2f        ", uart->get_temperaturaInterna());

	printf("===> %s\n", sistemaTelaAcima);
	printf("===> %s\n", sistemaTelaAbaixo);

	lcd->set_mensagemAcima16(sistemaTelaAcima);
	lcd->set_mensagemAbaixo16(sistemaTelaAbaixo);
	lcd->mostrarMensagem();


	// teste->enviarDisplayEstadoSistema( 0x00 );
	// teste->enviarDisplayControle( 0x00 );

	// teste->lerComandosDoUsuario();

	// printf(" O comando foi  %d\n", teste->get_codigoRetorno());

	// teste->enviarSinalDeControle( 69 );
	// teste->enviarSinalDeReferencia( 50.0f );

	// printf("\n==================\nControle de temperatura interna teste \n");



	// printf("Adicionar temperatura\n");
	// umTeste.mudarTemperatura( 100 );
	// delay(30000);
	// printf("Reduzir temperatura\n");
	// umTeste.mudarTemperatura( -100 );
	// delay(30000);
	
	// sleep(2);

	// if ( teste->solicitarTemperaturaPotenciometro() ) {
	//   return EXIT_FAILURE;
	// }

	// sleep(2);

	// if ( teste->lerComandosDoUsuario() ) {
	//   return EXIT_FAILURE;
	// }

	delete(lcd);
	delete(tempAmbiente);
	delete(uart);

	return EXIT_SUCCESS;
}
