extern double __ieee754_pow();

double pow(double __x, double __y) {
    return __ieee754_pow(__x, __y);
}
