#include "../inc/ControleTemperatura.hpp"

ControleTemperatura::ControleTemperatura() {

    pwmResistor = 4;
    pwmVentoinha = 5;

    if ( wiringPiSetup() == -1) exit(1);

    printf("Ligando Resistor e Ventoinha\n");
    pinMode(pwmResistor, OUTPUT);
    pinMode(pwmVentoinha, OUTPUT);
    softPwmCreate(pwmResistor, 1, 100);
    softPwmCreate(pwmVentoinha, 1, 100);
    softPwmWrite(pwmResistor, 0);
    softPwmWrite(pwmVentoinha, 0);

}

ControleTemperatura::~ControleTemperatura() {
    printf("Desligando Resistor e Ventoinha\n");
    softPwmWrite(pwmResistor, 0);
    softPwmWrite(pwmVentoinha, 0);
    softPwmStop( pwmResistor );
    softPwmStop( pwmVentoinha );
}

void ControleTemperatura::mudarTemperatura( int valorPorcentagem ) {
    if( valorPorcentagem > -101 && valorPorcentagem < 101) {
        if(valorPorcentagem >= 0) {
            softPwmWrite(pwmResistor, valorPorcentagem);
            softPwmWrite(pwmVentoinha, 0);
            this->valorVentoinha = 0;
            this->valorResistor = valorPorcentagem;
        }else {
            if( valorPorcentagem < 0 && valorPorcentagem >= -40 ) {
                softPwmWrite(pwmResistor, 0);
                softPwmWrite(pwmVentoinha, 40);
                this->valorVentoinha = 40;
                this->valorResistor = 0;
            } else {
                softPwmWrite(pwmResistor, 0);
                softPwmWrite(pwmVentoinha, valorPorcentagem*-1 );
                this->valorVentoinha = valorPorcentagem*-1;
                this->valorResistor = 0;
            }
        }
    }
}

int ControleTemperatura::get_valorPorcentagem() {
    return this->valorPorcentagem;
}

int ControleTemperatura::get_valorResistor() {
    return this->valorResistor;
}
int ControleTemperatura::get_valorVentoinha() {
    return this->valorVentoinha;
}
