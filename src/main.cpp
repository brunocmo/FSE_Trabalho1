#include <iostream>
#include "../inc/CommsProtocol.hpp"
#include "../inc/TemperatureStatus.hpp"

int main() {

  TemperatureStatus * statusTemperatura = new TemperatureStatus();
  CommsProtocol * teste = new CommsProtocol();


  if ( teste->solicitarTemperaturaInterna() ) {
    return EXIT_FAILURE;
  }

  printf("A temperatura ambiente eh: %.4lf\n", statusTemperatura->get_temperatura());
  printf("A temperatura interna  eh: %.4lf\n", teste->get_temperaturaInterna());

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
