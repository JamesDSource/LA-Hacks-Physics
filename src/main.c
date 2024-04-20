#include<stdio.h>
#include<stdbool.h>
#include"world.h"
#include<assert.h>
#include "raylib.h"

const int width  = 800;
const int height = 450;

void drawPoly(Vec2* points, size_t size, Vec2* velocity){                      
    for (int i = 0; i<size; i++){                              
		points[i].x = points[i].x + velocity->x;                                       
		points[i].y = points[i].y + velocity->y;  
		if(points[i].x>width){
			points[i].x = width;
			velocity->x = 0;
			velocity->y = 0;
		}	
		if(points[i].y>height){
			points[i].y = height;
			velocity->x = 0;
			velocity->y = 0;                     
		}                       
    }       
	for (int i = 0; i<size-1; i++){                                                                     
        DrawLine(points[i].x, points[i].y, points[i+1].x, points[i+1].y,BLACK);       
    }    
    DrawLine(points[0].x, points[0].y, points[size-1].x, points[size-1].y,RED);    
}                                                                                     


void drawCircle(Vec2 center, Fixed_FLT radius, Vec2* velocity){                
    center.x = center.x + velocity->x;                                                 
    center.y = center.y + velocity->y;        
	if(center.x>width-radius){
		center.x = width-radius;
		velocity->x = 0;
		velocity->y = 0;
	}
	if(center.y>height-radius){
		center.y = height-radius;
		velocity->x = 0;
		velocity->y = 0;
	}                                 
    DrawCircle((int)FloatFromFixed(center.x),(int)FloatFromFixed(center.y), radius, DARKBLUE);                                            
}



int main(void) {   
	InitWindow(width, height, "Physics Demo");

	Vec2 velocity = (Vec2){
		.x = FixedFromInt(0),
		.y = FixedFromInt(0),
	};

	Vec2 object1[] = {                                                                
    (Vec2){ .x = FixedFromInt(100), .y = FixedFromInt(150) },  
    (Vec2){ .x = FixedFromInt(200), .y = FixedFromInt(150) },                                                              
    (Vec2){ .x = FixedFromInt(300), .y = FixedFromInt(450) },                                                                                    
 	};

 	int size = sizeof(object1)/sizeof(Vec2);
	World *world;
	World_Result world_res = WorldInit(&world);
	Circle circle = {10};
	Shape shape = {.circle = circle};
	ObjectListAppend(&(world->objects), (Object){SHAPE_CIRCLE, shape}, object1[0], velocity, (ObjectMaterial){0});
	assert(world_res == WORLD_RESULT_SUCCESS);
	while (!WindowShouldClose())                                                      
   	{ 
       BeginDrawing();                                                               
       ClearBackground(RAYWHITE);                                                                                              
       DrawText(TextFormat("size %i", size), GetScreenWidth()/2 - 100, 50, 20, BLACK);
       drawPoly(object1, size, &velocity);                                                    
       drawCircle(object1[2], 10, &velocity); 
       drawCircle(object1[1], 10, &velocity);                                                   

       EndDrawing();
    }
	CloseWindow();
	return 0;
}
