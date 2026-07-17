extern double __ieee754_fmod();


double fmod(double __x, double __y) {
    return __ieee754_fmod(__x, __y);
}
