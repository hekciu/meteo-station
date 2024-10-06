#ifndef POWER_H
#define POWER_H

int power(int base, int exp){
    if (exp == 0) {
        return 1;
    }

    int result = base;
    for (int i = 1; i < exp; i++) {
        result *= base;
    }

    return result;
}

#endif
