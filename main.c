//
// Created by Carillon on 2019/4/19.
//

#include <stdio.h>
#include "src/quadtree.h"

void ascent(quadtree_node_t *node) {
    printf("\n");
}

void descent(quadtree_node_t *node) {
    if (node->bounds != NULL)
        printf("{ nw.x:%f, nw.y:%f, se.x:%f, se.y:%f }: ", node->bounds->nw->x,
               node->bounds->nw->y, node->bounds->se->x, node->bounds->se->y);
}


int main(int argc, const char *argv[])
{
    int val = 10;
    quadtree_t *tree = quadtree_new(1, 1, 10, 10);
    quadtree_insert(tree, 8.0, 2.0, &val);
    quadtree_insert(tree, 9.0, 3.0, &val);
    quadtree_insert(tree, 2.0, 3.0, &val);
    quadtree_insert(tree, 3.0, 1.1, &val);

    printf("tree's length is %d\n", tree->length);
    // quadtree_point_t * point = quadtree_search(tree, 9.0, 3.0);
    // printf("search point is (%f, %f)\n", point->x, point->y);

    printf("------\n");
    quadtree_node_t *node = quadtree_search_querynode(tree->root, 2.0, 6.0);
    printf("node the (width, height) is (%f, %f)\n", node->bounds->width, node->bounds->height);
    printf("------\n");

    // quadtree_walk(tree->root, ascent, descent);
    quadtree_free(tree);

    return 0;
}