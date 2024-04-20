#ifndef WORLD_H_
#define WORLD_H_

#include"list.h"
#include"fixed_math.h"

#include<stdlib.h>

typedef struct {
	Fixed_FLT x;
	Fixed_FLT y;
} Vec2;

LIST_H(Vec2, PointList)

typedef struct {
	PointList *points;
} Polygon;

typedef struct {
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

typedef struct {
	Fixed_FLT mass;
} ObjectMaterial;

typedef struct {
	size_t len;
	size_t cap;
	Object *objects;
	Vec2 *positions;
	Vec2 *velocities;
	ObjectMaterial *materials;
} ObjectList;

typedef enum {
	WORLD_RESULT_SUCCESS,
	WORLD_RESULT_OUT_OF_MEMORY,
} World_Result;

typedef struct {
	ObjectList objects;
} World;

List_Result ObjectListInit(size_t cap, ObjectList *list);
void ObjectListFree(ObjectList list);
List_Result ObjectListAppend(ObjectList *list, Object object, Vec2 position, Vec2 velocity, ObjectMaterial material);
List_Result ObjectListUnorderedRemove(ObjectList *list, int idx);

World_Result WorldInit(World **world);
void WorldDeinit(World *world);
void WorldTick(World *world);

#endif
