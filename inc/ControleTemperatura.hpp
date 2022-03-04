#ifndef CONTROLETEMPERATURA_HPP
#define CONTROLETEMPERATURA_HPP

#include <cstdio>
#include <cstdlib>
#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>

class ControleTemperatura {
    private:
        int pwmResistor;
        int pwmVentoinha;

        int valorPorcentagem;

    public:
        ControleTemperatura();
        ~ControleTemperatura();

        void mudarTemperatura( int valorPorcentagem );

        int get_valorPorcentagem();
};
#endif