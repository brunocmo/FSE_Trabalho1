#include <iostream>
#include "../inc/CommsProtocol.hpp"
#include "../inc/TemperatureStatus.hpp"
#include "../inc/ShowInfoLCD.hpp"
#include "../inc/ControleTemperatura.hpp"
#include "../inc/pid.hpp"

int main( int argc, char *argv[] ) {

	ShowInfoLCD * lcd = new ShowInfoLCD();
	TemperatureStatus * tempAmbiente = new TemperatureStatus();
	CommsProtocol * uart = new CommsProtocol();
	ControleTemperatura controleDaTemperatura;

	int sinalControle{0};
	bool sistemaLigado{false};

	pid_configura_constantes( 20.0, 0.1, 100.0 );

	while(1) {

		uart->lerComandosDoUsuario();

		switch( uart->get_codigoRetorno() ) {
			case 1:
				uart->enviarDisplayEstadoSistema( 0x01 );
				sistemaLigado = true;
				break;
			case 2: 
				uart->enviarDisplayEstadoSistema( 0x00 );
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

			controleDaTemperatura.mudarTemperatura(sinalControle);

		}
		delay(1000);
	}


	// char tempAmbiente[16] = "Temp. Ambiente:";
	// char tempReferencia[16] = "Temp. Pot.:";
	// char tempInterna[16] = "Temp. Interna.:";


	// teste->solicitarTemperaturaInterna();
	// teste->solicitarTemperaturaPotenciometro();

	// printf("A temperatura ambiente   eh: %.4lf\n", statusTemperatura->get_temperatura());
	// printf("A temperatura referencia eh: %.4lf\n", teste->get_temperaturaReferencia());
	// printf("A temperatura interna    eh: %.4lf\n", teste->get_temperaturaInterna());

	// lcd->set_mensagemAcima16(tempAmbiente);
	// lcd->typeFloat( (float)(statusTemperatura->get_temperatura()) );

	// lcd->mostrarMensagem();

	// sleep(3); 

	// lcd->set_mensagemAcima16(tempReferencia);
	// lcd->typeFloat( teste->get_temperaturaReferencia() );

	// lcd->mostrarMensagem();

	// sleep(3); 

	// lcd->set_mensagemAcima16(tempInterna);
	// lcd->typeFloat( teste->get_temperaturaInterna());

	// lcd->mostrarMensagem();

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
