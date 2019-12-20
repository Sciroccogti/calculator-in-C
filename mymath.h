#ifndef MYMATH_H
#define MYMATH_H

// a^n
int Pow(int a, int n)
{
    int i, ret = 1;
    for(i = 0; i < n; i++)
    {
        ret *= a;
    }
    return ret;
}

#endif