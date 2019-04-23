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
    quadtree_insert(tree, 4.0, 6.0, &val);
    quadtree_insert(tree, 5.0, 8.0, &val);
    quadtree_insert(tree, 2.0, 3.0, &val);
    quadtree_insert(tree, 3.0, 1.1, &val);

    printf("tree's length is %d\n\n", tree->length);
    // quadtree_point_t * point = quadtree_search(tree, 9.0, 3.0);
    // printf("search point is (%f, %f)\n", point->x, point->y);

    // test1 -- function < quadtree_node_t *quadtree_search_querynode(quadtree_node_t *root, double x, double y) >
    // search the node who contains query point
    printf("test1 -- function < quadtree_node_t *quadtree_search_querynode(quadtree_node_t *root, double x, double y) >\n");
    printf("------\n");
    quadtree_node_t *node_q1 = quadtree_search_querynode(tree->root, 6.0, 6.0);
    printf("node the bound nw (x, y) is (%f, %f)\n and node the bound se (x, y) is (%f, %f)\n",\
            node_q1->bounds->nw->x, node_q1->bounds->nw->y, node_q1->bounds->se->x, node_q1->bounds->se->y);
    printf("node the (width, height) is (%f, %f)\n", node_q1->bounds->width, node_q1->bounds->height);
    printf("------\n");

    printf("------\n");
    quadtree_node_t *node_q2 = quadtree_search_querynode(tree->root, 3.0, 8.0);
    printf("node the bound nw (x, y) is (%f, %f)\n and node the bound se (x, y) is (%f, %f)\n",\
            node_q2->bounds->nw->x, node_q2->bounds->nw->y, node_q2->bounds->se->x, node_q2->bounds->se->y);
    printf("node the (width, height) is (%f, %f)\n", node_q2->bounds->width, node_q2->bounds->height);
    printf("------\n\n");


    // test2 -- function < quadtree_node_t *quadtree_search_parentnode(quadtree_node_t *root, quadtree_node_t *node) >
    // search the parent node who contains the node who contains query point
    printf("test2 -- function < quadtree_node_t *quadtree_search_parentnode(quadtree_node_t *root, quadtree_node_t *node) >\n");
    printf("------\n");
    quadtree_node_t *node_parent_q1 = quadtree_search_parentnode(tree->root, node_q1);
    printf("node the bound nw (x, y) is (%f, %f)\n and node the bound se (x, y) is (%f, %f)\n",\
            node_parent_q1->bounds->nw->x, node_parent_q1->bounds->nw->y, node_parent_q1->bounds->se->x, node_parent_q1->bounds->se->y);
    printf("node the (width, height) is (%f, %f)\n", node_parent_q1->bounds->width, node_parent_q1->bounds->height);
    printf("------\n");

    printf("------\n");
    quadtree_node_t *node_parent_q2 = quadtree_search_parentnode(tree->root, node_q2);
    printf("node the bound nw (x, y) is (%f, %f)\n and node the bound se (x, y) is (%f, %f)\n",\
            node_parent_q2->bounds->nw->x, node_parent_q2->bounds->nw->y, node_parent_q2->bounds->se->x, node_parent_q2->bounds->se->y);
    printf("node the (width, height) is (%f, %f)\n", node_parent_q2->bounds->width, node_parent_q2->bounds->height);
    printf("------\n\n");


    // quadtree_walk(tree->root, ascent, descent);
    quadtree_free(tree);

    return 0;
}