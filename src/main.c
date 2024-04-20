#include<stdio.h>
#include<stdbool.h>
#include"world.h"
#include<assert.h>
#include "raylib.h"

void drawPoly(Vector2* points, size_t size, Vector2 velocity){                      
    for (int i = 0; i<size; i++){                                                   
        points[i].x = points[i].x + velocity.x;                                       
        points[i].y = points[i].y + velocity.y;                                       
    }        
	for (int i = 0; i<size-1; i++){                                                                     
        DrawLine(points[i].x, points[i].y, points[i+1].x, points[i+1].y,BLACK);       
    }    

    DrawLine(points[0].x, points[0].y, points[size-1].x, points[size-1].y,RED);    
}                                                                                     


void drawCircle(Vector2 center, Fixed_FLT radius, Vector2 velocity){                
    center.x = center.x + velocity.x;                                                 
    center.y = center.y + velocity.y;                                                 
    DrawCircleV(center, radius, DARKBLUE);                                            
}

Vector2 object1[] = {                                                                
     { 100.0f, 150.0f },                                                              
     { 200.0f, 200.0f },                                                              
     { 250.0f, 300.0f },                                                              
     { 400.0f, 160.0f },                                                             
    // { 200.0f, 260.0f },                                                            
 };
Vector2 speed = {0.1,0.1};                                                                  
int size = sizeof(object1)/sizeof(Vector2);  

int main(void) {
	InitWindow(800, 450, "Physics Demo");
	Vec2 v = (Vec2){
		.x = FixedFromInt(1),
		.y = FixedFromInt(2),
	};
	Fixed_FLT length = FixedSqrt(FixedAdd(FixedMult(v.x, v.x), FixedMult(v.y, v.y)));
	printf("%f\n%f\n%f\n", FloatFromFixed(FixedDiv(FixedFromInt(2), FixedFromInt(3))), FloatFromFixed(FixedFromFloat(2)), FloatFromFixed(length));
	World *world;
	World_Result world_res = WorldInit(&world);
	assert(world_res == WORLD_RESULT_SUCCESS);
	while (!WindowShouldClose())                                                      
   { 
       BeginDrawing();                                                               
       ClearBackground(RAYWHITE);                                                                                              
       DrawText(TextFormat("size %i", size), GetScreenWidth()/2 - 100, 50, 20, BLACK);
       drawPoly(object1, size, speed);                                                    
       drawCircle(object1[0], 10, speed);                                                   
       EndDrawing();
    }
	CloseWindow();
	return 0;
}
