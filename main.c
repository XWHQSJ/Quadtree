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


int main(int argc, const char *argv[]) {
    int val = 10;
    quadtree_t *tree = quadtree_new(1, 1, 10, 10);
    quadtree_insert(tree, 8.0, 2.0, &val);
    quadtree_insert(tree, 9.0, 3.0, &val);
    quadtree_insert(tree, 3.0, 7.0, &val);
    quadtree_insert(tree, 4.0, 6.0, &val);
    quadtree_insert(tree, 5.0, 8.0, &val);
    quadtree_insert(tree, 4.0, 8.0, &val);
    quadtree_insert(tree, 4.0, 9.0, &val);
    quadtree_insert(tree, 2.0, 9.0, &val);
    quadtree_insert(tree, 2.5, 8.5, &val);
    quadtree_insert(tree, 2.0, 3.0, &val);
    quadtree_insert(tree, 3.0, 1.5, &val);
    quadtree_insert(tree, 9.0, 9.0, &val);
    quadtree_insert(tree, 8.0, 8.0, &val);
    quadtree_insert(tree, 7.0, 6.0, &val);
    quadtree_insert(tree, 6.0, 4.0, &val);
    quadtree_insert(tree, 5.0, 5.0, &val);

    printf("tree's length is %d\n\n", tree->length);
    // quadtree_point_t * point = quadtree_search(tree, 9.0, 3.0);
    // printf("search point is (%f, %f)\n", point->x, point->y);


    // printf("node the point (x, y) is (%f, %f)\n", tree->root->sw->sw->ne->sw->point->x, tree->root->sw->sw->ne->sw->point->y);

    // test1 -- function < quadtree_node_t *quadtree_search_querynode(quadtree_node_t *root, double x, double y) >
    // search the node who contains query point
    printf("test1 -- function < quadtree_node_t *quadtree_search_querynode(quadtree_node_t *root, double x, double y) >\n");
    printf("------\n");
    quadtree_node_t *node_q1 = quadtree_search_querynode(tree->root, 6.0, 6.0);
    printf("node the bound nw (x, y) is (%f, %f)\n and node the bound se (x, y) is (%f, %f)\n", \
            node_q1->bounds->nw->x, node_q1->bounds->nw->y, node_q1->bounds->se->x, node_q1->bounds->se->y);
    printf("node the (width, height) is (%f, %f)\n", node_q1->bounds->width, node_q1->bounds->height);

    printf("\nnode contain point (x, y) is (%f, %f)\n", node_q1->point->x, node_q1->point->y);

    printf("------\n");

    printf("------\n");
    quadtree_point_t *querypoint2 = quadtree_point_new(8.5, 3.0);
    quadtree_node_t *node_q2 = quadtree_search_querynode(tree->root, 8.5, 3.0);
    printf("node the bound nw (x, y) is (%f, %f)\n and node the bound se (x, y) is (%f, %f)\n", \
            node_q2->bounds->nw->x, node_q2->bounds->nw->y, node_q2->bounds->se->x, node_q2->bounds->se->y);
    printf("node the (width, height) is (%f, %f)\n", node_q2->bounds->width, node_q2->bounds->height);

    printf("------\n\n");

    printf("------\n");
    quadtree_node_t *node_q3 = quadtree_search_querynode(tree->root, 4.0, 4.0);
    printf("node the bound nw (x, y) is (%f, %f)\n and node the bound se (x, y) is (%f, %f)\n", \
            node_q3->bounds->nw->x, node_q3->bounds->nw->y, node_q3->bounds->se->x, node_q3->bounds->se->y);
    printf("node the (width, height) is (%f, %f)\n", node_q3->bounds->width, node_q3->bounds->height);

    printf("\nnode contain point (x, y) is (%f, %f)\n", node_q3->point->x, node_q3->point->y);

    printf("------\n\n");


    // test2 -- function < quadtree_node_t *quadtree_search_parentnode(quadtree_node_t *root, quadtree_node_t *node) >
    // search the parent node who contains the node who contains query point
    printf("test2 -- function < quadtree_node_t *quadtree_search_parentnode(quadtree_node_t *root, quadtree_node_t *node) >\n");
    printf("------\n");
    quadtree_node_t *node_parent_q1 = quadtree_search_parentnode(tree->root, node_q1);
    printf("node the bound nw (x, y) is (%f, %f)\n and node the bound se (x, y) is (%f, %f)\n", \
            node_parent_q1->bounds->nw->x, node_parent_q1->bounds->nw->y, node_parent_q1->bounds->se->x,
           node_parent_q1->bounds->se->y);
    printf("node the (width, height) is (%f, %f)\n", node_parent_q1->bounds->width, node_parent_q1->bounds->height);
    printf("------\n");

    printf("------\n");
    quadtree_node_t *node_parent_q2 = quadtree_search_parentnode(tree->root, node_q2);
    printf("node the bound nw (x, y) is (%f, %f)\n and node the bound se (x, y) is (%f, %f)\n", \
            node_parent_q2->bounds->nw->x, node_parent_q2->bounds->nw->y, node_parent_q2->bounds->se->x,
           node_parent_q2->bounds->se->y);
    printf("node the (width, height) is (%f, %f)\n", node_parent_q2->bounds->width, node_parent_q2->bounds->height);
    printf("------\n\n");


    // test3 -- function < quadtree_point_t **quadtree_search_points(quadtree_node_t *rootnode) >
    // search the points in the parent node
    printf("test3 -- function < quadtree_point_t **quadtree_search_points(quadtree_node_t *rootnode) >\n");
    printf("------\n");
    quadtree_point_t **searchpoints;
    searchpoints = quadtree_search_points(node_parent_q2);
    free(pPoints);
    count_point = 0;
    for (int i = 0; i < KEY; i++) {
        printf("node %d the (x, y) is (%f, %f)\n", i, searchpoints[i]->x, searchpoints[i]->y);
    }
    printf("------\n\n");


    // test4 -- function < double compute_point_distance(quadtree_point_t *point, quadtree_point_t *query_point) >
    // compute the distance between querypoint and nodepoint
    printf("test4 -- function < double compute_point_distance(quadtree_point_t *point, quadtree_point_t *query_point) >\n");
    printf("------\n");
    printf("node_parent_q2 bound nw (x, y) is (%f, %f)\n node_parent_q2 bount se (x, y) is (%f, %f)\n", \
            node_parent_q2->bounds->nw->x, node_parent_q2->bounds->nw->y, node_parent_q2->bounds->se->x, node_parent_q2->bounds->se->y);
    printf("node_parent_q2->ne->point (x, y) is (%f, %f)\n", node_parent_q2->ne->point->x, node_parent_q2->ne->point->y);
    double distanceX = compute_point_distance(node_q1->point, node_parent_q2->ne->point);
    printf("the distanceX between node_q1->point and node_parent_q2->ne->point is %f\n", distanceX);

    double distanceY = compute_point_distance(node_q1->point, node_q3->point);
    printf("the distanceY between node_q1->point and node_q3->point is %f\n", distanceY);
    printf("------\n\n");


    // test5 -- function < double compare_point_distance(double distance_nw, double distance_ne, double distance_sw, double distance_ne) >
    // compare the distances to find the smallest distance and the nearest point
    printf("test5 -- function < double compare_point_distance(double distance_nw, double distance_ne, double distance_sw, double distance_ne) >\n");
    printf("------\n");
    double di[4] = {distanceX, distanceY, 100, 100};
    double distanceZ = compare_point_distance(tree, di);
    printf("the distanceZ is %f\n", distanceZ);
    printf("------\n\n");


    // test6 -- function < quadtree_point_t *quadtree_search_nearest_point(quadtree_t *tree, quadtree_node_t *querynode) >
    // search the nearest point in the querynode or the parentnode
    printf("test6 -- function < quadtree_point_t *quadtree_search_nearest_point(quadtree_t *tree, quadtree_node_t *querynode) >\n");
    printf("------\n");
    printf("node_q2->point (x, y) is (%f, %f)\n", querypoint2->x, querypoint2->y);
    quadtree_point_t *point_q = quadtree_search_nearest_point(tree, node_q2, querypoint2);
    printf("the nearest node (x, y) is (%f, %f)\n", point_q->x, point_q->y);
    printf("------\n\n");


    // test7 -- function < quadtree_node_t *get_new_quadrant(quadtree_point_t *point, quadtree_point_t *querypoint) >
    // get the new quadrant who contains the query and the nearest point
    printf("test7 -- function < quadtree_node_t *get_new_quadrant(quadtree_point_t *point, quadtree_point_t *querypoint) >\n");
    printf("------\n");
    quadtree_node_t *new_quadrant = get_new_quadrant(point_q, querypoint2);
    free(pPoints);
    count_point = 0;
    printf("the new_quadrant bound nw (x, y) is (%f, %f)\n the new_quadrant bound se (x, y) is (%f, %f)\n", \
            new_quadrant->bounds->nw->x, new_quadrant->bounds->nw->y, new_quadrant->bounds->se->x, new_quadrant->bounds->se->y);
    printf("------\n\n");





    // quadtree_walk(tree->root, ascent, descent);
    quadtree_free(tree);

    return 0;
}