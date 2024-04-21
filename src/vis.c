#include"vis.h"
#include"raylib.h"

static void drawPoly(Vec2 position, PointList* points) {
	for (Vec2 *point = &(points->items); point < &(points->items) + points->len; ++point) {
		Fixed_FLT x = FixedAdd(position.x, point->x);
		Fixed_FLT y = FixedAdd(position.y, point->y);
		Fixed_FLT x1 = FixedAdd(position.x, point[1].x);
		Fixed_FLT y1 = FixedAdd(position.y, point[1].y);
		DrawLine((int)FloatFromFixed(x), (int)FloatFromFixed(y), (int)FloatFromFixed(x1), (int)FloatFromFixed(y1), BLACK);
	}

	Fixed_FLT x = FixedAdd(position.x, points->items.x);
	Fixed_FLT y = FixedAdd(position.y, points->items.y);
	Fixed_FLT x1 = FixedAdd(position.x, (&(points->items) + (points->len - 1))->x);
	Fixed_FLT y1 = FixedAdd(position.y, (&(points->items) + (points->len - 1))->y);
	DrawLine((int)FloatFromFixed(x), (int)FloatFromFixed(y), (int)FloatFromFixed(x1), (int)FloatFromFixed(y1), RED);
}

static void drawCircle(Vec2 center, Fixed_FLT radius){
	DrawCircle((int)FloatFromFixed(center.x),(int)FloatFromFixed(center.y), (int)FloatFromFixed(radius), DARKBLUE);
}

void VisualizeWorld(World* world) {
	for (size_t i = 0; i < world->objects.len; ++i) {
		switch (world->objects.objects[i].type) {
		case SHAPE_CIRCLE:
			drawCircle(world->objects.positions[i], world->objects.objects[i].shape.circle.radius);
		case SHAPE_POLYGON:
			drawPoly(world->objects.positions[i], world->objects.objects[i].shape.polygon.points);
		}
	}
}
