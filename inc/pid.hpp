#ifndef PID_HPP
#define PID_HPP

#include <stdio.h>

void pid_configura_constantes(double Kp_, double Ki_, double Kd_);
void pid_atualiza_referencia(float referencia_);
double pid_controle(double saida_medida);

#endif /* PID_H_ */
