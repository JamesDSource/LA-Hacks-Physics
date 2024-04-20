#ifndef FIXED_MATH_H_
#define FIXED_MATH_H_

#include<stdint.h>

#define FLT_INT_BITS 16
#define FLT_FRAC_BITS 16
#define FLT_FRAC_MASK 0x0000FFFF
#define FLT_TOTAL_BITS FLT_INT_BITS + FLT_FRAC_BITS
#define FLT_MAX INT32_MAX
#define FLT_MIN INT32_MIN

typedef int32_t Fixed_FLT;

Fixed_FLT FixedFromInt(int x);
Fixed_FLT FixedFromFloat(float x);
float FloatFromFixed(Fixed_FLT x);

Fixed_FLT FixedAdd(Fixed_FLT a, Fixed_FLT b);
Fixed_FLT FixedSub(Fixed_FLT a, Fixed_FLT b);
Fixed_FLT FixedMult(Fixed_FLT a, Fixed_FLT b);
Fixed_FLT FixedDiv(Fixed_FLT a, Fixed_FLT b);

#ifdef FIXED_MATH_IMPL_
Fixed_FLT FixedFromInt(int x) {
	return (Fixed_FLT)(x << FLT_FRAC_BITS);
}

Fixed_FLT FixedFromFloat(float x) {
	int int_part = (int)(x);
	float frac_part = x - int_part;
	Fixed_FLT fixed = (Fixed_FLT)(int_part << FLT_FRAC_BITS);
	fixed += (int)((1 << FLT_FRAC_BITS)*frac_part);
	return fixed;
}

float FloatFromFixed(Fixed_FLT x) {
	return float(x) / float(65536);
}

Fixed_FLT FixedAdd(Fixed_FLT a, Fixed_FLT b) {
	if ((a > 0) && (b > FLT_MAX - a)) {
		return (a + FLT_MIN) + (b + FLT_MIN);
	} else if ((a < 0) && (b < FLT_MIN - a)) {
		return (a - FLT_MIN) + (b - FLT_MIN);
	} else {
		return a + b;
	}
}

Fixed_FLT FixedSub(Fixed_FLT a, Fixed_FLT b) {
	if ((a > 0) && (-b > FLT_MAX - a)) {
		return (a + FLT_MIN) + (-b + FLT_MIN);
	} else if ((a < 0) && (-b < FLT_MIN - a)) {
		return (a - FLT_MIN) + (-b - FLT_MIN);
	} else {
		return a - b;
	}
}

Fixed_FLT FixedMult(Fixed_FLT a, Fixed_FLT b) {
	Fixed_FLT int_part1 = a >> FLT_FRAC_BITS;
	Fixed_FLT frac_part1 = a & FLT_FRAC_MASK;

	Fixed_FLT int_part2 = b >> FLT_FRAC_BITS;
	Fixed_FLT frac_part2 = b & FLT_FRAC_MASK;

	Fixed_FLT value = (int_part1 * int_part2) << FLT_FRAC_BITS;
	value += (int_part1 * frac_part2);
	value += (frac_part1 * int_part2);
	value += ((frac_part1 * frac_part2) >> FLT_FRAC_BITS) & FLT_FRAC_MASK;
	return value;
}

Fixed_FLT FixedDiv(Fixed_FLT a, Fixed_FLT b) {
	uint32_t reciprocal = 1;
	reciprocal <<= 31;
	reciprocal = (uint32_t)(reciprocal / b);

	return (Fixed_FLT)(a * reciprocal) << 1;
}

#endif
#endif
