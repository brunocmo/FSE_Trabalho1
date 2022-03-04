#ifndef CONTROLETEMPERATURA_HPP
#define CONTROLETEMPERATURA_HPP

#include <cstdio>
#include <cstdlib>
#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>

class ControleTemperatura {

    public:
        int valorPorcentagem;

        ControleTemperatura();
        ~ControleTemperatura();

};



#endif