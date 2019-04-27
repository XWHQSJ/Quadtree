#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define QUADTREE_VERSION "0.0.1"

#include <stdlib.h>
#include <math.h>

typedef struct quadtree_point {
    double x;
    double y;
} quadtree_point_t;

typedef struct quadtree_bounds {
    quadtree_point_t *nw;
    quadtree_point_t *se;
    double width;
    double height;
} quadtree_bounds_t;


// add count for each node
typedef struct quadtree_node {
    struct quadtree_node *ne;
    struct quadtree_node *nw;
    struct quadtree_node *se;
    struct quadtree_node *sw;
    quadtree_bounds_t *bounds;
    quadtree_point_t *point;
    // int count;
    void *key;
} quadtree_node_t;

typedef struct quadtree {
    quadtree_node_t *root;

    void (*key_free)(void *key);

    unsigned int length;
} quadtree_t;

// POINT
quadtree_point_t *
quadtree_point_new(double x, double y);

void
quadtree_point_free(quadtree_point_t *point);


// BOUNDS
quadtree_bounds_t *
quadtree_bounds_new();

void
quadtree_bounds_extend(quadtree_bounds_t *bounds, double x, double y);

void
quadtree_bounds_free(quadtree_bounds_t *bounds);


// NODE
quadtree_node_t *
quadtree_node_new(double d, double d1);

void
quadtree_node_free(quadtree_node_t *node, void (*value_free)(void *));

int
quadtree_node_ispointer(quadtree_node_t *node);

int
quadtree_node_isempty(quadtree_node_t *node);

int
quadtree_node_isleaf(quadtree_node_t *node);

void
quadtree_node_reset(quadtree_node_t *node, void (*key_free)(void *));

quadtree_node_t *
quadtree_node_with_bounds(double minx, double miny, double maxx, double maxy);


// TREE
quadtree_t *
quadtree_new(double minx, double miny, double maxx, double maxy);

void
quadtree_free(quadtree_t *tree);

quadtree_point_t *
quadtree_search(quadtree_t *tree, double x, double y);

int
quadtree_insert(quadtree_t *tree, double x, double y, void *key);

void
quadtree_walk(quadtree_node_t *root,
              void (*descent)(quadtree_node_t *node),
              void (*ascent)(quadtree_node_t *node));

quadtree_node_t *
quadtree_search_querynode(quadtree_node_t *root, double x, double y);

quadtree_node_t *
quadtree_search_parentnode(quadtree_node_t *root, quadtree_node_t *node);

quadtree_point_t **
quadtree_search_points(quadtree_node_t *rootnode);

quadtree_point_t *
quadtree_search_nearest_point(quadtree_t *tree, quadtree_node_t *querynode);

double
compute_point_distance(quadtree_point_t *point, quadtree_point_t *query_point);

double
compare_point_distance(double distance_nw, double distance_ne, double distance_sw, double distance_se);


#define K_1 20
#define KEY 2

quadtree_point_t **pPoints;

static int count_point = 0;

#ifdef __cplusplus
}
#endif

#endif
