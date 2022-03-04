#include "../inc/ControleTemperatura.hpp"

ControleTemperatura::ControleTemperatura() {

    if ( wiringPiSetup() == -1) exit(1);

    printf("Criação do PWM 100 \n");

    printf("Resultado: %d", softPwmCreate( 4, 0, 100);

    sleep(10);

    printf("Mudou para 50 \n");
    softPwmWrite( 4, 50);

    sleep(10);

    printf("PWM desligando");
    softPwmStop( 4 );


}

ControleTemperatura::~ControleTemperatura() {}