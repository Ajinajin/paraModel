#include "BezierOpt.h"
#include <cmath>

// 曲线系数
#define binomial(n, i) (factorial(n) / (factorial(i) * factorial(n - i)))

// Bernstein递推公式

double bernstein(int i, int n, double t)
{
    return binomial(n, i) * pow(t, i) * pow(1 - t, n - i);
}

// Factorial因子计算

double factorial(double n)
{
    double fact = 1.0;

    for (int i = 2; i <= n; i++)
        fact *= i;

    return fact;
}
