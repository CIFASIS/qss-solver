#include <math.h>
#include <stdlib.h>
#include <stdio.h>

double
ipvsolver (double Vpv, int Np, int Ns, double Tpv, double lambdaph, double emax, int itmax, double Ipvguess)
{
    double q = 1.6e-19; // [C]
    double Ac = 1.6;
    double K = 1.3805e-23; // [Nm/K]
    double K1 = 0.0017; // [ A/oC]
    double Ior = 2.0793e-6; // [A]
    double Tref = 303; // [K]
    double Eg = 1.1; // [V]
    double Isc = 3.27; // [A]
    double Rspv = 1.0;

    double Irs; //Corriente de saturación inversa
    double Iph; //Corriente por radiación

    Irs = Ior * pow ((Tpv / Tref), 3) * exp (q * Eg * (1 / Tref - 1 / Tpv) / K / Ac);
    Iph = (Isc + K1 * (Tpv - Tref)) * lambdaph / 100;

    double Ipv = 0;
    double Ipv1;
    int it = 0;
    double error = 10;
    double exponente;
    double f0;
    double df0;

    while (error >= emax && it < itmax)
    {
        exponente = q * (Vpv / Ns + Ipv * Rspv / Np) / (K * Ac * Tpv);
        f0 = Ipv - Np * Iph + Np * Irs * (exp (exponente) - 1);
        df0 = 1 + Np * Irs * exp (exponente) * q * Rspv / (Np * K * Ac * Tpv);
        Ipv1 = Ipv - f0 / df0;
        it = it + 1;
        error = fabs ((Ipv1 - Ipv) / Ipv1);
//    double errorabs=Ipv-Ipv(1);
        Ipv = Ipv1;
    }
    return Ipv;
}

