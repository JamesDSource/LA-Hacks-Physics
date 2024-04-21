#include"fixed_math.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

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
	return (float)(x) / (float)(65536);
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
	return (Fixed_FLT)((long)(a) *(1 << 16)/b);
}

Fixed_FLT FixedAbs(Fixed_FLT x) {
	return x < 0 ? -x : x;
}

Fixed_FLT FixedSqrt(Fixed_FLT x) {
	printf("%f\n", FloatFromFixed(x));
	float f = FloatFromFixed(x);
	printf("%f\n", f);
	f = sqrtf(f);
	printf("%f\n", f);
	return FixedFromFloat(f);
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

Vec2 Vec2MultScaler(Vec2 a, Fixed_FLT b) {
	return (Vec2) {
		.x = FixedMult(a.x, b),
		.y = FixedMult(a.y, b),
	};
}

Vec2 Vec2DivScaler(Vec2 a, Fixed_FLT b) {
	return (Vec2) {
		.x = FixedDiv(a.x, b),
		.y = FixedDiv(a.y, b),
	};
}

Vec2 Vec2Abs(Vec2 x) {
	return (Vec2){
		.x = FixedAbs(x.x),
		.y = FixedAbs(x.y),
	};
}

Fixed_FLT Vec2Dot(Vec2 a, Vec2 b) {
	return FixedAdd(FixedMult(a.x, b.x), FixedMult(a.y, b.y));
}

Fixed_FLT Vec2Cross(Vec2 a, Vec2 b) {
	return FixedSub(FixedMult(a.x, b.y), FixedMult(b.x, a.y));
}

Fixed_FLT Vec2Length(Vec2 x) {
	return FixedSqrt(FixedAdd(FixedMult(x.x, x.x), FixedMult(x.y, x.y)));
}

Vec2 Vec2Normalize(Vec2 x) {
	printf("x %f y %f \n", FloatFromFixed(x.x), FloatFromFixed(x.y));
	Fixed_FLT length = Vec2Length(x);
	printf("Length %f\n", FloatFromFixed(length));
	return (Vec2){
		.x = FixedDiv(x.x, length),
		.y = FixedDiv(x.y, length),
	};
}

Vec2 Vec2Normalize0(Vec2 x) {
	Fixed_FLT length = Vec2Length(x);
	return length == 0 ? (Vec2){0} : (Vec2){
		.x = FixedDiv(x.x, length),
		.y = FixedDiv(x.y, length),
	};
}

Vec2 Vec2Min(Vec2 a, Vec2 b) {
	return (Vec2) {
		a.x < b.x ? a.x : b.x,
		a.y < b.y ? a.y : b.y
	};
}

Vec2 Vec2Max(Vec2 a, Vec2 b) {
	return (Vec2) {
		a.x > b.x ? a.x : b.x,
		a.y > b.y ? a.y : b.y
	};
}
