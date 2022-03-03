#include <iostream>
#include "../inc/CommsProtocol.hpp"

int main() {

  CommsProtocol * teste = new CommsProtocol();

  if ( teste->solicitarTemperaturaInterna() ) {
    return EXIT_FAILURE;
  }

  if ( teste->solicitarTemperaturaPotenciometro() ) {
    return EXIT_FAILURE;
  }

  if ( teste->lerComandosDoUsuario() ) {
    return EXIT_FAILURE;
  }

  delete(teste);

  return EXIT_SUCCESS;
}
