#include"vis.h"
#include"raylib.h"
#include"world.h"

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
	DrawCircle((int)FloatFromFixed(center.x),(int)FloatFromFixed(center.y), FloatFromFixed(radius), DARKBLUE);
}

void VisualizeWorld(World* world) {
    static int init_done =0;
    if (init_done ==0){
      init_done=1;
      Vec2 object1[] = {
      //(Vec2){ .x = FixedFromInt(100), .y = FixedFromInt(150) },
      //(Vec2){ .x = FixedFromInt(200), .y = FixedFromInt(150) },
      (Vec2){ .x = FixedFromInt(300), .y = FixedFromInt(450) },
      (Vec2){ .x = FixedFromInt(400), .y = FixedFromInt(460) },
      (Vec2){ .x = FixedFromInt(600), .y = FixedFromInt(550) },
    };
    Vec2 velocity = (Vec2){
		.x = FixedFromFloat(0),
		.y = FixedFromFloat(0),
    };
    Circle circle = {FixedFromInt(10)};
    Shape shape = {.circle = circle};
    Vec2 object2[] = {
      //(Vec2){ .x = FixedFromInt(100), .y = FixedFromInt(150) },
      (Vec2){ .x = FixedFromInt(200), .y = FixedFromInt(150) },
      (Vec2){ .x = FixedFromInt(40), .y = FixedFromInt(46) },
      (Vec2){ .x = FixedFromInt(60), .y = FixedFromInt(55) },
    };

    Polygon polygon;
    PointListInit(2, &polygon.points);
    PointListAppend(&polygon.points,object2[0]);
   
    
    Shape shape2 = {.polygon = polygon};
    ObjectListAppend(&(world->objects), (Object){SHAPE_CIRCLE, shape}, object1[0], velocity, (ObjectMaterial){0});
    ObjectListAppend(&(world->objects), (Object){SHAPE_POLYGON, shape2}, object1[0], velocity, (ObjectMaterial){0});
    }
			drawCircle((Vec2){world->objects.positions->x, world->objects.positions->y},world->objects.objects->shape.circle.radius);
			drawPoly(world->objects.positions[0], world->objects.objects->shape.polygon.points);
//	for (size_t i = 0; i < world->objects.len; ++i) {
//		switch (world->objects.objects[i].type) {
//		case SHAPE_CIRCLE:
//			drawCircle(world->objects.positions[i], world->objects.objects[i].shape.circle.radius);
//		case SHAPE_POLYGON:
}
