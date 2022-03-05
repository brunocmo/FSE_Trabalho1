#include <iostream>
#include "../inc/Forno.hpp"

int main(int argc, char * argv[]) {

	if( argc < 2 ) {
		std::cout << "Erro na execução, falta o input do hostname" << '\n';
		return -1;
    }

	int selecaoMenu{0};
	float temperaturaReferenciaMenu{0.0f};
	char inputSimOuNao{'n'};

	Forno * raspberryEmbarcados = new Forno();

	std::cout << "======== Sistema de Forno para soldagem de placas ========" << '\n';
	std::cout << "Selecione uma opcao: " << '\n';
	std::cout << "1. Modo Terminal " << '\n';
	std::cout << "2. Modo via dashboard " << '\n';
	std::cout << "0. Sair" << '\n';

	std::cin >> selecaoMenu;

	system("clear");

	switch (selecaoMenu)
	{
		case 1:
			std::cout << " Digite a temperatura referencia desejada (TR): ";			
			std::cin >>  temperaturaReferenciaMenu;

			system("clear");

			std::cout << "Deseja inserir os parametros do PID manualmente?(s/n) ";
			std::cin >> inputSimOuNao;

			system("clear");

			if( inputSimOuNao == 's') {
				raspberryEmbarcados->configurarParametrosPID();
			}else {
				raspberryEmbarcados->configurarParametrosViaHostname(argv[1]);
			}

			raspberryEmbarcados->executarSistema(temperaturaReferenciaMenu);
			break;
		
		case 2:
			std::cout << "Deseja inserir os parametros do PID manualmente?(s/n) ";
			std::cin >> inputSimOuNao;

			if( inputSimOuNao == 's') {
				raspberryEmbarcados->configurarParametrosPID();
			}else {
				raspberryEmbarcados->configurarParametrosViaHostname(argv[1]);
			}

			raspberryEmbarcados->executarSistema();
			break;

		default:
			break;
	}


	delete(raspberryEmbarcados);

	std::cout << "Obrigado por usar o sistema! " << '\n';

	return EXIT_SUCCESS;
}
