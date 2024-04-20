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

typedef struct {
	Fixed_FLT x;
	Fixed_FLT y;
} Vec2;

Fixed_FLT FixedFromInt(int x);
Fixed_FLT FixedFromFloat(float x);
float FloatFromFixed(Fixed_FLT x);

Fixed_FLT FixedAdd(Fixed_FLT a, Fixed_FLT b);
Fixed_FLT FixedSub(Fixed_FLT a, Fixed_FLT b);
Fixed_FLT FixedMult(Fixed_FLT a, Fixed_FLT b);
Fixed_FLT FixedDiv(Fixed_FLT a, Fixed_FLT b);

Fixed_FLT Vec2Add(Vec2 a, Vec2 b);
Fixed_FLT Vec2Sub(Vec2 a, Vec2 b);
Fixed_FLT Vec2Mult(Vec2 a, Vec2 b);
Fixed_FLT Vec2Div(Vec2 a, Vec2 b);
Fixed_FLT Vec2MultScaler(Vec2 a, Fixed_FLT b);
Fixed_FLT Vec2DivScaler(Vec2 a, Fixed_FLT b);
Fixed_FLT Vec2Dot(Vec2 a, Vec2 b);
Fixed_FLT Vec2Cross(Vec2 a, Vec2 b);

#endif

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

Vec2 Vec2Add(Vec2 a, Vec2 b) {
	return (Vec2) {
		.x = FixedAdd(a.x, b.x),
		.y = FixedAdd(a.y, b.y),
	};
}

Vec2 Vec2Sub(Vec2 a, Vec2 b) {
	return (Vec2) {
		.x = FixedAdd(a.x, -b.x),
		.y = FixedAdd(a.y, -b.y),
	};
}

Vec2 Vec2Mult(Vec2 a, Vec2 b) {
	return (Vec2) {
		.x = FixedMult(a.x, b.x),
		.y = FixedMult(a.y, b.y),
	};
}

Vec2 Vec2Div(Vec2 a, Vec2 b) {
	return (Vec2) {
		.x = FixedDiv(a.x, b.x),
		.y = FixedDiv(a.y, b.y),
	};
}

Fixed_FLT Vec2MultScaler(Vec2 a, Fixed_FLT b) {
	return (Vec2) {
		.x = FixedMult(a.x, b),
		.y = FixedMult(a.y, b),
	};
}

Fixed_FLT Vec2DivScaler(Vec2 a, Fixed_FLT b) {
	return (Vec2) {
		.x = FixedDiv(a.x, b),
		.y = FixedDiv(a.y, b),
	};
}

Fixed_FLT Vec2Dot(Vec2 a, Vec2 b) {
	return FixedAdd(FixedMult(a.x, b.x), FixedMult(a.y, b.y));
}

vector_cross2 :: proc "contextless" (a, b: $T/[2]$E) -> E where IS_NUMERIC(E) {
	return a[0]*b[1] - b[0]*a[1];
}

Fixed_FLT Vec2Cross(Vec2 a, Vec2 b) {
	return FixedSub(FixedMult(a.x, b.y), FixedMult(b.x, a.y));
}

#endif
