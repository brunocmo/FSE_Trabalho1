#include "../inc/Forno.hpp"

bool executar = true;

void tratarSinal(int s){
	printf("\nFechando o programa... \n");
	sleep(1);
	executar = false;
}

Forno::Forno() {

    iteradorReflow = 0;

    std::strcpy( telaModoUart, "Modo: UART     " );
    std::strcpy( telaModoTerminal, "Modo: Terminal ");

    std::strcpy( sistemaTelaAcima, "  Modo: UART   " );
    std::strcpy( sistemaTelaAbaixo, "               " );

    std::strcpy( sistemaDesligadoAcima, "   Desligado   " );
    std::strcpy( sistemaDesligadoAbaixo, "      OFF      " );

    comecaCronometro = std::chrono::system_clock::now();
    cronometroReflow = std::chrono::system_clock::now();
    voltaCronometro = std::chrono::system_clock::now();
    comeca_tempo = std::chrono::system_clock::to_time_t(comecaCronometro);
    tempoPassado = voltaCronometro - comecaCronometro;
    
    temperaturaReferencia = 28.00f;

    sinalControle = 0;
    sistemaLigado = false;
}

Forno::~Forno() {
	delete(controleDaTemperatura);
    delete(lcd);
	delete(tempAmbiente);
	delete(uart);
}

bool Forno::executarSistema() {

    signal(SIGINT, tratarSinal);

	pid_configura_constantes( get_pid_Kp(), get_pid_Ki(), get_pid_Kd() );

	lcd->set_mensagemAcima16( sistemaDesligadoAcima );
	lcd->set_mensagemAbaixo16( sistemaDesligadoAbaixo );
	lcd->mostrarMensagem();

	uart->enviarDisplayControle( MODO_UART );
	
	lcd->set_mensagemAcima16(sistemaTelaAcima);
	uart->enviarSinalDeReferencia( temperaturaReferencia );

	referenciaReflow.carregarValores();

	while(executar) {

		uart->lerComandosDoUsuario();

		switch( uart->get_codigoRetorno() ) {
			case 1:
				uart->enviarDisplayEstadoSistema( LIGAR );
				sistemaLigado = true;
				break;
			case 2: 
				uart->enviarDisplayEstadoSistema( DESLIGAR );
				lcd->set_mensagemAcima16(sistemaDesligadoAcima);
				lcd->set_mensagemAbaixo16(sistemaDesligadoAbaixo);
				lcd->mostrarMensagem();
                controleDaTemperatura->mudarTemperatura(0);
                uart->enviarSinalDeControle( 0 );
				sistemaLigado = false;

				break;
			case 3:
				uart->enviarDisplayControle( MODO_UART );
				std::strcpy( sistemaTelaAcima, telaModoUart );
				lcd->set_mensagemAcima16(sistemaTelaAcima);
				modoUART = true;
				break;
			case 4:
				uart->enviarDisplayControle( MODO_TERMINAL );
				std::strcpy( sistemaTelaAcima, telaModoTerminal );
				lcd->set_mensagemAcima16(sistemaTelaAcima);
				iteradorReflow = 0;
                curvaTemperaturaAnterior = referenciaReflow.tempo.at(iteradorReflow);
				modoUART = false;
				cronometroReflow = std::chrono::system_clock::now();
			default: break;	
		}

		if( sistemaLigado ) {

			uart->solicitarTemperaturaInterna();

			if (modoUART) 
				uart->solicitarTemperaturaPotenciometro();
			else {
				
				voltaCronometro = std::chrono::system_clock::now();
				tempoPassado = voltaCronometro - cronometroReflow;

                if( ((int)referenciaReflow.tempo.size()) > (iteradorReflow+1)) {
                    if( ((int)tempoPassado.count()) >= referenciaReflow.tempo.at(iteradorReflow+1) ) {
                        // std::cout << "Mudando temperatura!" << '\n';
                        iteradorReflow++;

                    }

                    temperaturaReferencia = interpolarReferencia( 
                    referenciaReflow.tempo.at(iteradorReflow),
                    referenciaReflow.tempo.at(iteradorReflow+1),
                    (int)tempoPassado.count(),
                    referenciaReflow.temperatura.at(iteradorReflow),
                    referenciaReflow.temperatura.at(iteradorReflow+1)
                    );

                    printf("Valor temperatura referencia: %.2f\n", temperaturaReferencia);
                }		
                

                uart->enviarSinalDeReferencia(temperaturaReferencia);
                uart->set_temperaturaReferencia(temperaturaReferencia);

			}

			pid_atualiza_referencia( uart->get_temperaturaReferencia() );
			sinalControle = (int)pid_controle( uart->get_temperaturaInterna() );

			uart->enviarSinalDeControle( sinalControle );

			controleDaTemperatura->mudarTemperatura(sinalControle);

			std::sprintf( sistemaTelaAbaixo, "%.1f %.1f %.1f", 
				uart->get_temperaturaInterna(),
				uart->get_temperaturaReferencia(), 
				(float)tempAmbiente->get_temperatura()
			);

			lcd->set_mensagemAbaixo16(sistemaTelaAbaixo);
			lcd->mostrarMensagem();
		
			comecaCronometro = std::chrono::system_clock::now();
			comeca_tempo = std::chrono::system_clock::to_time_t(comecaCronometro);

			std::time(&comeca_tempo);
			timeinfo = std::localtime(&comeca_tempo);

    		std::strftime( tempoString, 18, "%d/%m/%y %T", timeinfo );

			std::printf("%s -> Temp.Interna: %.2f deg C Temp.Externa: %.2f deg C Temp.Rerencia: %.2f deg C Resistor: %3d%% Ventoinha: %3d%% \n", 
				tempoString,
				uart->get_temperaturaInterna(),
				tempAmbiente->get_temperaturaEmFloat(),
				uart->get_temperaturaReferencia(),
				controleDaTemperatura->get_valorResistor(),
				controleDaTemperatura->get_valorVentoinha()
			);

			registro.set_dataHora(tempoString);
			registro.set_tempInterna(uart->get_temperaturaInterna());
			registro.set_tempExterna(tempAmbiente->get_temperaturaEmFloat());
			registro.set_tempReferencia(uart->get_temperaturaReferencia());
			registro.set_valorPotenciometro(sinalControle);
			registro.registrarInformacoes();

		}
	}

	uart->enviarSinalDeControle( 0 );

    return EXIT_SUCCESS;

}

bool Forno::executarSistema( float temperaturaReferenciaMenu ) {

    signal(SIGINT, tratarSinal);

	pid_configura_constantes( get_pid_Kp(), get_pid_Ki(), get_pid_Kd() );

	lcd->set_mensagemAcima16( sistemaDesligadoAcima );
	lcd->set_mensagemAbaixo16( sistemaDesligadoAbaixo );
	lcd->mostrarMensagem();

	uart->enviarDisplayControle( MODO_TERMINAL );
	
	lcd->set_mensagemAcima16(sistemaTelaAcima);
	uart->enviarSinalDeReferencia( temperaturaReferenciaMenu );
    pid_atualiza_referencia( temperaturaReferenciaMenu );

	referenciaReflow.carregarValores();

	while(executar) {

		uart->lerComandosDoUsuario();

		switch( uart->get_codigoRetorno() ) {
			case 1:
				uart->enviarDisplayEstadoSistema( LIGAR );
				sistemaLigado = true;
				break;
			case 2: 
				uart->enviarDisplayEstadoSistema( DESLIGAR );
				lcd->set_mensagemAcima16(sistemaDesligadoAcima);
				lcd->set_mensagemAbaixo16(sistemaDesligadoAbaixo);
				lcd->mostrarMensagem();
                controleDaTemperatura->mudarTemperatura(0);
                uart->enviarSinalDeControle( 0 );
				sistemaLigado = false;
				break;
			default: break;	
		}

		if( sistemaLigado ) {

			uart->solicitarTemperaturaInterna();

			sinalControle = (int)pid_controle( uart->get_temperaturaInterna() );

			uart->enviarSinalDeControle( sinalControle );

			controleDaTemperatura->mudarTemperatura(sinalControle);

			std::sprintf( sistemaTelaAbaixo, "%.1f %.1f %.1f", 
				uart->get_temperaturaInterna(),
				temperaturaReferenciaMenu, 
				(float)tempAmbiente->get_temperatura()
			);

			lcd->set_mensagemAbaixo16(sistemaTelaAbaixo);
			lcd->mostrarMensagem();
		
			comecaCronometro = std::chrono::system_clock::now();
			comeca_tempo = std::chrono::system_clock::to_time_t(comecaCronometro);

			std::time(&comeca_tempo);
			timeinfo = std::localtime(&comeca_tempo);

    		std::strftime( tempoString, 18, "%d/%m/%y %T", timeinfo );

			std::printf("%s -> Temp.Interna: %.2f deg C Temp.Externa: %.2f deg C Temp.Rerencia: %.2f deg C Resistor: %3d%% Ventoinha: %3d%% \n", 
				tempoString,
				uart->get_temperaturaInterna(),
				tempAmbiente->get_temperaturaEmFloat(),
				temperaturaReferenciaMenu,
				controleDaTemperatura->get_valorResistor(),
				controleDaTemperatura->get_valorVentoinha()
			);

			registro.set_dataHora(tempoString);
			registro.set_tempInterna(uart->get_temperaturaInterna());
			registro.set_tempExterna(tempAmbiente->get_temperaturaEmFloat());
			registro.set_tempReferencia(temperaturaReferenciaMenu);
			registro.set_valorPotenciometro(sinalControle);
			registro.registrarInformacoes();

		}
	}

	uart->enviarSinalDeControle( 0 );

    return EXIT_SUCCESS;

}




void Forno::configurarParametrosPID() {
    double valorTemporario{0};
    
    std::cout << "Insirar os valores dos parametros do PID na ordem pedida!" << '\n';
    std::cout << "Inserir o valor do Kp: ";
    std::cin >> valorTemporario;
    set_pid_Kp(valorTemporario);

    std::cout << "Inserir o valor do Ki: ";
    std::cin >> valorTemporario;
    set_pid_Ki(valorTemporario);

    std::cout << "Inserir o valor do Kd: ";
    std::cin >> valorTemporario;
    set_pid_Kd(valorTemporario);
}

void Forno::configurarParametrosViaHostname( char * hostname ) {
    if ( !(std::strcmp(hostname, "rasp42")) ) {
        std::cout << "Placa Rasp42 (12V) detectado, inserindo configurações padrões do PID" << '\n';
        set_pid_Kp( 30.0 );
        set_pid_Ki( 0.2 );
        set_pid_Kd( 400.0 );
    } else {
        std::cout << "Placa Rasp43 (5V) detectado, inserindo configurações padrões do PID" << '\n';
        set_pid_Kp( 20.0 );
        set_pid_Ki( 0.1 );
        set_pid_Kd( 100.0 );
    }

}

float Forno::interpolarReferencia( int x_anterior, int x_posterior, int x_atual, float t_anterior, float t_posterior){
    float resultado{0};

    resultado = t_anterior + (((x_atual-x_anterior)/(x_posterior-x_anterior))*(t_posterior-t_anterior));
    return resultado;
}

double Forno::get_pid_Kp() {
    return this->pid_Kp;
}

void Forno::set_pid_Kp( double pid_Kp ) {
    this->pid_Kp = pid_Kp;
}

double Forno::get_pid_Ki() {
    return this->pid_Ki;
}

void Forno::set_pid_Ki( double pid_Ki ) {
    this->pid_Ki = pid_Ki;
}

double Forno::get_pid_Kd() {
    return this->pid_Kd;
}

void Forno::set_pid_Kd( double pid_Kd ) {
    this->pid_Kd = pid_Kd;
}