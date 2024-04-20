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
Fixed_FLT FixedAbs(Fixed_FLT x);
Fixed_FLT FixedSqrt(Fixed_FLT x);

Vec2 Vec2Add(Vec2 a, Vec2 b);
Vec2 Vec2Sub(Vec2 a, Vec2 b);
Vec2 Vec2Mult(Vec2 a, Vec2 b);
Vec2 Vec2Div(Vec2 a, Vec2 b);
Vec2 Vec2MultScaler(Vec2 a, Fixed_FLT b);
Vec2 Vec2DivScaler(Vec2 a, Fixed_FLT b);
Vec2 Vec2Abs(Vec2 x);
Fixed_FLT Vec2Dot(Vec2 a, Vec2 b);
Fixed_FLT Vec2Cross(Vec2 a, Vec2 b);
Fixed_FLT Vec2Length(Vec2 x);
Vec2 Vec2Normalize(Vec2 x);
#endif
