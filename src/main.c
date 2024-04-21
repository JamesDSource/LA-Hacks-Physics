#include<stdio.h>
#include<stdbool.h>
#include<assert.h>
#include"raylib.h"
#include"world.h"

const int width  = 800;
const int height = 800;

void drawPoly(Vec2* points, size_t size, Vec2* velocity){                      
    for (int i = 0; i<size; i++){                              
		points[i].x = points[i].x + velocity->x;                                       
		points[i].y = points[i].y + velocity->y;  
		if(points[i].x>FixedFromInt(width)){
			points[i].x = FixedFromInt(width);
			velocity->x = 0;
			velocity->y = 0;
		}else if(points[i].x<0){
			points[i].x = FixedFromInt(0);
			velocity->x = 0;
			velocity->y = 0;
		}
		if(points[i].y>FixedFromInt(height)){
			points[i].y = FixedFromInt(height);
			velocity->x = 0;
			velocity->y = 0;                     
		}   else if(points[i].y<0){
			points[i].y = FixedFromInt(0);
			velocity->x = 0;
			velocity->y = 0;
		}                    
    }       
	for (int i = 0; i<size-1; i++){                                                                     
        DrawLine((int)FloatFromFixed(points[i].x), (int)FloatFromFixed(points[i].y), (int)FloatFromFixed(points[i+1].x), (int)FloatFromFixed(points[i+1].y),BLACK);       
    }    
    DrawLine((int)FloatFromFixed(points[0].x), (int)FloatFromFixed(points[0].y),(int)FloatFromFixed(points[size-1].x), (int)FloatFromFixed(points[size-1].y),RED);    
}                                                                                     


void drawCircle(Vec2 center, Fixed_FLT radius, Vec2* velocity){                
    center= Vec2Add(center,*velocity);

	if(center.x>FixedFromInt(width-radius)){
		center.x = FixedFromInt(width-radius);
		velocity->x = 0;
		velocity->y = 0;
	}else if(center.x<FixedFromInt(radius)){
		center.x = FixedFromInt(radius);
		velocity->x = 0;
		velocity->y = 0;

	}
	if(center.y>FixedFromInt(height-radius)){
		center.y = FixedFromInt(height-radius);
		velocity->x = 0;
		velocity->y = 0;
	} else if(center.y<FixedFromInt(radius)){
		center.x = FixedFromInt(radius);
		velocity->x = 0;
		velocity->y = 0;

	}                              
    DrawCircle((int)FloatFromFixed(center.x),(int)FloatFromFixed(center.y), radius, DARKBLUE);                                            
}

int main(void) {   
	#if 0 
	root->objectIndex = 0;
	root->box
    root->objectIndex
    root->parentIndex
    root->child1
    root->child2
    root->isLeaf
	#endif
	InitWindow(width, height, "Physics Demo");

	Vec2 velocity = (Vec2){
		.x = FixedFromFloat(-0.1f),
		.y = FixedFromFloat(-0.1f),
	};

	Vec2 object1[] = {                                                                
    //(Vec2){ .x = FixedFromInt(100), .y = FixedFromInt(150) },  
    //(Vec2){ .x = FixedFromInt(200), .y = FixedFromInt(150) },                                                              
    (Vec2){ .x = FixedFromInt(300), .y = FixedFromInt(450) }, 
    (Vec2){ .x = FixedFromInt(400), .y = FixedFromInt(460) },                                                                                    
    (Vec2){ .x = FixedFromInt(600), .y = FixedFromInt(550) },                                                                                    

 	};


	Vec2 object2[] = {                                                                
    //(Vec2){ .x = FixedFromInt(100), .y = FixedFromInt(150) },  
    //(Vec2){ .x = FixedFromInt(200), .y = FixedFromInt(150) },                                                              
    (Vec2){ .x = FixedFromInt(350), .y = FixedFromInt(550) },                                                                                    

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
       drawCircle(object2[0], 10, &velocity);                                                   


       EndDrawing();
    }
	CloseWindow();
	return 0;
}
