#ifndef BVH_H_
#define BVH_H_

#include "fixed_math.h"
#include "world.h"

typedef struct {
  Vec2 lower_bound;
  Vec2 upper_bound;
}AABB;

typedef struct {
    AABB box;
    int object_index;
    int parent_index;
    int child1;
    int child2;
} Node;

typedef struct {
    Node *nodes;
    int node_count;
    int root_index;
} Tree;

void TreeBuild(Tree **tree, ObjectList *objects);
void TreeRebuild(Tree *tree, ObjectList *objects);
void TreeCleanup(Tree *tree);

AABB AABBUnion(AABB a, AABB b);
Fixed_FLT Area(AABB a);

#endif
