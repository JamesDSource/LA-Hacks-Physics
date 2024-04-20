#ifndef WORLD_H_
#define WORLD_H_

#include"list.h"

// Add the fixed math implementation. Do only once
#define _FIXED_MATH_IMPL_
#include"fixed_math.h"

#include<stdlib.h>

typedef struct {
	Fixed_FLT x;
	Fixed_FLT y;
} Vec2;

LIST(Vec2, PointList)

typedef struct {
	PointList *points;
} Polygon;

typedef struct {
	Vec2 center;
	int radius;
} Circle;

typedef enum {
	SHAPE_POLYGON,
	SHAPE_CIRCLE,
} Shape_Type;

typedef union {
	Polygon polygon;
	Circle circle;
} Shape;

typedef struct {
	Shape_Type type;
	Shape shape;
} Object;

LIST(Object, ObjectList)

typedef enum {
	WORLD_RESULT_SUCCESS,
	WORLD_RESULT_OUT_OF_MEMORY,
} World_Result;

typedef struct {
	ObjectList *objects;
} World;

World_Result WorldInit(World **world);
void WorldDeinit(World *world);
void WorldTick(World *world);

#endif
