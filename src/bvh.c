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

// Helper function to compute the total area of the bounding boxes for a range of objects

// Recursive function to build the BVH tree
Node* BuildNode(ObjectList* objects, int startIndex, int endIndex, Fixed_FLT minArea) {
  Node* node = malloc(sizeof(Node));
  // Calculate initial bounding box based on objects
  AABB bounds;
  bounds.lower_bound = objects->positions[startIndex];
  bounds.upper_bound = objects->positions[startIndex];
  for (int i = startIndex + 1; i < endIndex; ++i) {
    bounds.lower_bound = Vec2Min(bounds.lower_bound, objects->positions[i]);
    bounds.upper_bound = Vec2Max(bounds.upper_bound, objects->positions[i]);
  }

  // Check if minimum area threshold is met
  Fixed_FLT currentArea = Area(bounds);
  if (currentArea < minArea) {
    bounds.upper_bound.x = FixedAdd(bounds.lower_bound.x, FixedSqrt(minArea));
    bounds.upper_bound.y = FixedAdd(bounds.lower_bound.y, FixedSqrt(minArea));
  }

  node->box = bounds;

  // Check if leaf node (single object)
  if (endIndex - startIndex == 1) {
    node->child1 = NULL;
    node->child2 = NULL;
    node->object_index = startIndex;
  } else {
    // Find split axis based on surface area heuristic (replace with your splitting strategy)
     // Assuming splitting on X-axis for simplicity
    Fixed_FLT minCost = FLT_MAX;
    int bestSplit = -1;

    for (int i = startIndex + 1; i < endIndex; ++i) {
      AABB leftBounds = bounds;
      leftBounds.upper_bound.x = objects->positions[i].x;
      AABB rightBounds = bounds;
      rightBounds.lower_bound.x = objects->positions[i].x;

      Fixed_FLT leftArea = Area(leftBounds);
      Fixed_FLT rightArea = Area(rightBounds);
      Fixed_FLT cost = leftArea + rightArea;

      if (cost < minCost) {
        minCost = cost;
        bestSplit = i;
      }
    }

    // Recursive calls to build child nodes
    node->child1 = BuildNode(objects, startIndex, bestSplit, minArea);
    node->child2 = BuildNode(objects, bestSplit, endIndex, minArea);
  }

  return node;
}

void TreeBuild(Tree **tree, ObjectList *objects, Fixed_FLT minArea) {
    *tree = malloc(sizeof(Tree));
    (*tree)->node_count = objects->len;

    // Build BVH tree
    (*tree)->root_node = BuildNode(objects, 0, objects->len, minArea);
}


void CleanBVHTree(Node *node) {
    if (node == NULL)
        return;

 
    CleanBVHTree(node->child1);
    CleanBVHTree(node->child2);
	free(node);
}
void TreeCleanup(Tree *tree){
	CleanBVHTree(tree->root_node);
	free(tree);
}

