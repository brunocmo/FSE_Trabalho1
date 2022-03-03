#include <iostream>
#include "../inc/CommsProtocol.hpp"
#include "../inc/TemperatureStatus.hpp"
#include "../inc/ShowInfoLCD.h"

int main() {

	TemperatureStatus * statusTemperatura = new TemperatureStatus();
	CommsProtocol * teste = new CommsProtocol();

	char tempAmbiente[16] = "Temp. Ambiente:";
	char tempReferencia[16] = "Temp. Pot.:";
	char tempInterna[16] = "Temp. Interna.:";


	teste->solicitarTemperaturaInterna();
	teste->solicitarTemperaturaPotenciometro();

	printf("A temperatura ambiente   eh: %.4lf\n", statusTemperatura->get_temperatura());
	printf("A temperatura referencia eh: %.4lf\n", teste->get_temperaturaReferencia());
	printf("A temperatura interna    eh: %.4lf\n", teste->get_temperaturaInterna());

	mostrarMensagem( tempAmbiente, typeFloat( (float)statusTemperatura->get_temperatura() ));
	mostrarMensagem( tempReferencia, typeFloat( teste->get_temperaturaReferencia() ));
	mostrarMensagem( tempInterna, typeFloat( teste->get_temperaturaInterna() ));

	// sleep(2);

	// if ( teste->solicitarTemperaturaPotenciometro() ) {
	//   return EXIT_FAILURE;
	// }

	// sleep(2);

	// if ( teste->lerComandosDoUsuario() ) {
	//   return EXIT_FAILURE;
	// }

	delete(statusTemperatura);
	delete(teste);

	return EXIT_SUCCESS;
}
