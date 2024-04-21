#include"bvh.h"
#include <math.h>

AABB AABBUnion(AABB a, AABB b) {
	AABB c;
	c.lower_bound = Vec2Min(a.lower_bound, b.lower_bound);
	c.upper_bound = Vec2Max(a.upper_bound, b.upper_bound);
	return c;
}

Fixed_FLT Area(AABB a) {
	Fixed_FLT width = FixedSub(a.upper_bound.x, a.lower_bound.x);
	Fixed_FLT height = FixedSub(a.upper_bound.y, a.lower_bound.y);
	return FixedMult(width, height);
}

void TreeBuild(Tree **tree, ObjectList *objects) {
	*tree = malloc(sizeof(Tree));
	(*tree)->node_count = objects->len;

	int steps = (int)(log2((double)(objects->len))) + 1;

	Node *root = malloc(sizeof(Node));
}

void TreeRebuild(Tree *tree, ObjectList *objects);
void TreeCleanup(Tree *tree);
