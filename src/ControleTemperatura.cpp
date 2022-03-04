#include "../inc/ControleTemperatura.hpp"

ControleTemperatura::ControleTemperatura() {

    pwmResistor = 4;

    if ( wiringPiSetup() == -1) exit(1);

    printf("Criação do PWM 100 \n");
    pinMode(pwmResistor, OUTPUT);
    softPwmCreate(pwmResistor, 1, 100);
    softPwmWrite(pwmResistor, 100);
    delay(10000);

    printf("PWM desligando\n");
    softPwmStop( 4 );


}

ControleTemperatura::~ControleTemperatura() {}