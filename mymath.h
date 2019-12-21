#ifndef MYMATH_H
#define MYMATH_H

// a^n
double Pow(int a, int n)
{
    int i;
    double ret = 1;
    if (n >= 0)
    {
        for (i = 0; i < n; i++)
        {
            ret *= a;
        }
    }
    else
    {
        for (i = 0; i < -n; i++)
        {
            ret /= a;
        }
    }

    return ret;
}

// log_n(a)
double Log(double a)
{
    int N = 20; //我们取了前15+1项来估算
    int k, nk;
    double x, xx, y;
    x = (a - 1) / (a + 1);
    xx = x * x;
    nk = 2 * N + 1;
    y = 1.0 / nk;
    for (k = N; k > 0; k--)
    {
        nk = nk - 2;
        y = 1.0 / nk + xx * y;
    }
    return 2.0 * x * y;
}

#endif