#include <iostream>
#include "../inc/CommsProtocol.hpp"
#include "../inc/TemperatureStatus.hpp"
#include "../inc/ShowInfoLCD.hpp"

int main() {

	ShowInfoLCD * lcd = new ShowInfoLCD();
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

	lcd->set_mensagemAcima16(tempAmbiente);
	lcd->typeFloat( (float)(statusTemperatura->get_temperatura()) );

	lcd->mostrarMensagem();

	sleep(3); 

	lcd->set_mensagemAcima16(tempReferencia);
	lcd->typeFloat( teste->get_temperaturaReferencia() );

	lcd->mostrarMensagem();

	sleep(3); 

	lcd->set_mensagemAcima16(tempInterna);
	lcd->typeFloat( teste->get_temperaturaInterna());

	lcd->mostrarMensagem();

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
