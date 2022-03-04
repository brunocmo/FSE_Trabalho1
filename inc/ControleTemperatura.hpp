#ifndef CONTROLETEMPERATURA_HPP
#define CONTROLETEMPERATURA_HPP

#include <cstdio>
#include <cstdlib>
#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>

class ControleTemperatura {

    public:

        int pwmResistor;
        int pwmVentoinha;

        int valorPorcentagem;

        ControleTemperatura();
        ~ControleTemperatura();

};



#endif