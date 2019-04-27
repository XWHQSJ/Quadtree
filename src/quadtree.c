#include "quadtree.h"
#include <stdio.h>

/* private prototypes */
static int
split_node_(quadtree_t *tree, quadtree_node_t *node);

static int
insert_(quadtree_t *tree, quadtree_node_t *root, quadtree_point_t *point, void *key);

static int
node_contains_(quadtree_node_t *outer, quadtree_point_t *it);

static quadtree_node_t *
get_quadrant_(quadtree_node_t *root, quadtree_point_t *point);


/* private implementations */
static int
node_contains_(quadtree_node_t *outer, quadtree_point_t *it) {
    return outer->bounds != NULL
           && outer->bounds->nw->x <= it->x
           && outer->bounds->nw->y >= it->y
           && outer->bounds->se->x >= it->x
           && outer->bounds->se->y <= it->y;
}


static int
node_contains_node_(quadtree_node_t *outer, quadtree_node_t *node) {
    return outer->bounds != NULL
           && outer->bounds->nw->x <= node->bounds->nw->x
           && outer->bounds->nw->y >= node->bounds->nw->y
           && outer->bounds->se->x >= node->bounds->se->x
           && outer->bounds->se->y <= node->bounds->se->y;
}


static int
node_equal_node_(quadtree_node_t *outer, quadtree_node_t *node) {
    return outer->bounds != NULL
           && outer->bounds->nw->x == node->bounds->nw->x
           && outer->bounds->nw->y == node->bounds->nw->y
           && outer->bounds->se->x == node->bounds->se->x
           && outer->bounds->se->y == node->bounds->se->y;
}


static void
elision_(void *key) {}

static void
reset_node_(quadtree_t *tree, quadtree_node_t *node) {
    if (tree->key_free != NULL) {
        quadtree_node_reset(node, tree->key_free);
    } else {
        quadtree_node_reset(node, elision_);
    }
}

static quadtree_node_t *
get_quadrant_(quadtree_node_t *root, quadtree_point_t *point) {
    if (node_contains_(root->nw, point)) return root->nw;
    if (node_contains_(root->ne, point)) return root->ne;
    if (node_contains_(root->sw, point)) return root->sw;
    if (node_contains_(root->se, point)) return root->se;
    return NULL;
}


static quadtree_node_t *
get_quadrant_node_(quadtree_node_t *root, quadtree_node_t *node) {
    if (node_contains_node_(root->nw, node)) return root->nw;
    if (node_contains_node_(root->ne, node)) return root->ne;
    if (node_contains_node_(root->sw, node)) return root->sw;
    if (node_contains_node_(root->se, node)) return root->se;
    return NULL;
}

static int
split_node_(quadtree_t *tree, quadtree_node_t *node) {
    quadtree_node_t *nw;
    quadtree_node_t *ne;
    quadtree_node_t *sw;
    quadtree_node_t *se;
    quadtree_point_t *old;
    void *key;

    double x = node->bounds->nw->x;
    double y = node->bounds->nw->y;
    double hw = node->bounds->width / 2;
    double hh = node->bounds->height / 2;

    //minx,   miny,       maxx,       maxy
    if (!(nw = quadtree_node_with_bounds(x, y - hh, x + hw, y))) return 0;
    if (!(ne = quadtree_node_with_bounds(x + hw, y - hh, x + hw * 2, y))) return 0;
    if (!(sw = quadtree_node_with_bounds(x, y - hh * 2, x + hw, y - hh))) return 0;
    if (!(se = quadtree_node_with_bounds(x + hw, y - hh * 2, x + hw * 2, y - hh))) return 0;

    node->nw = nw;
    node->ne = ne;
    node->sw = sw;
    node->se = se;

    old = node->point;
    key = node->key;
    node->point = NULL;
    node->key = NULL;

    return insert_(tree, node, old, key);
}


static quadtree_point_t *
find_(quadtree_node_t *node, double x, double y) {
    if (!node) {
        return NULL;
    }
    if (quadtree_node_isleaf(node)) {
        if (node->point->x == x && node->point->y == y)
            return node->point;
    } else if (quadtree_node_ispointer(node)) {
        quadtree_point_t test;
        test.x = x;
        test.y = y;
        return find_(get_quadrant_(node, &test), x, y);
    }

    return NULL;
}

/* cribbed from the google closure library. */
static int
insert_(quadtree_t *tree, quadtree_node_t *root, quadtree_point_t *point, void *key) {
    if (quadtree_node_isempty(root)) {
        root->point = point;
        root->key = key;
        return 1; /* normal insertion flag */
    } else if (quadtree_node_isleaf(root)) {
        if (root->point->x == point->x && root->point->y == point->y) {
            reset_node_(tree, root);
            root->point = point;
            root->key = key;
            return 2; /* replace insertion flag */
        } else {
            if (!split_node_(tree, root)) {
                return 0; /* failed insertion flag */
            }
            return insert_(tree, root, point, key);
        }
    } else if (quadtree_node_ispointer(root)) {
        quadtree_node_t *quadrant = get_quadrant_(root, point);
        return quadrant == NULL ? 0 : insert_(tree, quadrant, point, key);
    }

    return 0;
}


/* public */
quadtree_t *
quadtree_new(double minx, double miny, double maxx, double maxy) {
    quadtree_t *tree;
    if (!(tree = malloc(sizeof(*tree))))
        return NULL;
    tree->root = quadtree_node_with_bounds(minx, miny, maxx, maxy);
    if (!(tree->root))
        return NULL;
    tree->key_free = NULL;
    tree->length = 0;
    return tree;
}

int
quadtree_insert(quadtree_t *tree, double x, double y, void *key) {
    quadtree_point_t *point;
    int insert_status;

    if (!(point = quadtree_point_new(x, y))) return 0;
    if (!node_contains_(tree->root, point)) {
        quadtree_point_free(point);
        return 0;
    }

    if (!(insert_status = insert_(tree, tree->root, point, key))) {
        quadtree_point_free(point);
        return 0;
    }
    if (insert_status == 1) tree->length++;
    return insert_status;
}

quadtree_point_t *
quadtree_search(quadtree_t *tree, double x, double y) {
    return find_(tree->root, x, y);
}

void
quadtree_free(quadtree_t *tree) {
    if (tree->key_free != NULL) {
        quadtree_node_free(tree->root, tree->key_free);
    } else {
        quadtree_node_free(tree->root, elision_);
    }
    free(tree);
}

void
quadtree_walk(quadtree_node_t *root, void (*descent)(quadtree_node_t *node),
              void (*ascent)(quadtree_node_t *node)) {
    (*descent)(root);
    if (root->nw != NULL) quadtree_walk(root->nw, descent, ascent);
    if (root->ne != NULL) quadtree_walk(root->ne, descent, ascent);
    if (root->sw != NULL) quadtree_walk(root->sw, descent, ascent);
    if (root->se != NULL) quadtree_walk(root->se, descent, ascent);
    (*ascent)(root);
}

// search query node who contain query point
// query point(x, y)
// return parent node of point
quadtree_node_t *
quadtree_search_querynode(quadtree_node_t *root, double x, double y) {
    quadtree_point_t *it_point;
    it_point = quadtree_point_new(x, y);


    if (node_contains_(root, it_point)) {
        if (node_contains_(root->nw, it_point)) {
            if(quadtree_node_isleaf(root->nw)) {
                return root->nw;
            } else if ((root->nw->nw == NULL) &&
                       (root->nw->ne == NULL) &&
                       (root->nw->sw == NULL) &&
                       (root->nw->se == NULL)) {
                return root->nw;
            } else {
                return quadtree_search_querynode(root->nw, x, y);
            }
        }

        if (node_contains_(root->ne, it_point)) {
            if (quadtree_node_isleaf(root->ne)) {
                return root->ne;
            } else if ((root->ne->nw == NULL) &&
                       (root->ne->ne == NULL) &&
                       (root->ne->sw == NULL) &&
                       (root->ne->se == NULL)) {
                return root->ne;
            } else {
                return quadtree_search_querynode(root->ne, x, y);
            }
        }

        if (node_contains_(root->sw, it_point)) {
            if (quadtree_node_isleaf(root->sw)) {
                return root->sw;
            } else if ((root->se->nw == NULL) &&
                       (root->se->ne == NULL) &&
                       (root->se->sw == NULL) &&
                       (root->se->se == NULL)) {
                return root->sw;
            } else {
                return quadtree_search_querynode(root->sw, x, y);
            }
        }

        if (node_contains_(root->se, it_point)) {
            if (quadtree_node_isleaf(root->se)) {
                return root->se;
            } else if ((root->sw->nw == NULL) &&
                       (root->sw->ne == NULL) &&
                       (root->sw->sw == NULL) &&
                       (root->sw->se == NULL)) {
                return root->se;
            } else {
                return quadtree_search_querynode(root->se, x, y);
            }
        }
    } else {
        return NULL;
    }

    return NULL;
}

// search parent node of query node
// the root is tree main parent node
// the node is query node
// return parent node of query node
quadtree_node_t *
quadtree_search_parentnode(quadtree_node_t *root, quadtree_node_t *node) {

    if (!node_contains_node_(root, node)) {
        return NULL;
    } else {
        if (node_equal_node_(root->nw, node)) return root;
        if (node_equal_node_(root->ne, node)) return root;
        if (node_equal_node_(root->sw, node)) return root;
        if (node_equal_node_(root->se, node)) return root;

        if (node_contains_node_(root->nw, node)) {
            return quadtree_search_parentnode(root->nw, node);
        }

        if (node_contains_node_(root->ne, node)) {
            return quadtree_search_parentnode(root->ne, node);
        }

        if (node_contains_node_(root->sw, node)) {
            return quadtree_search_parentnode(root->sw, node);
        }

        if (node_contains_node_(root->se, node)) {
            return quadtree_search_parentnode(root->se, node);
        }
    }

    return NULL;
}

// search points of parent node
// the node is parent node
// return the points in the parent node
quadtree_point_t **
quadtree_search_points(quadtree_node_t *rootnode) {

    pPoints = malloc(K_1 * sizeof(**pPoints));

    if (quadtree_node_isleaf(rootnode)) {

        pPoints[count_point] = rootnode->point;
        count_point++;
    }

    if (rootnode->nw != NULL) {
        return quadtree_search_points(rootnode->nw);
    }

    if (rootnode->ne != NULL) {
        return quadtree_search_points(rootnode->ne);
    }

    if (rootnode->sw != NULL) {
        return quadtree_search_points(rootnode->sw);
    }

    if (rootnode->se != NULL) {
        return quadtree_search_points(rootnode->se);
    }

    if (KEY <= count_point) {
        return pPoints;
    }

    return pPoints;
}

// search the nearest point of the query point
// the root is tree main parent node
// return the nearest point
quadtree_point_t *
quadtree_search_nearest_point(quadtree_t *tree, quadtree_node_t *querynode) {

    quadtree_node_t *node_parent_q = quadtree_search_parentnode(tree->root, querynode);

    double distance = 0;
    double distance_nw = 0;
    double distance_ne = 0;
    double distance_sw = 0;
    double distance_se = 0;


    if (quadtree_node_isleaf(querynode)) {
        return querynode->point;
    }

    if (quadtree_node_ispointer(node_parent_q)) {
        if (quadtree_node_isleaf(node_parent_q->nw)) {
            distance_nw = compute_point_distance(node_parent_q->nw->point, querynode->point);

            pPoints[0] = node_parent_q->nw->point;
        }

        if (quadtree_node_isleaf(node_parent_q->ne)) {
            distance_ne = compute_point_distance(node_parent_q->ne->point, querynode->point);

            pPoints[1] = node_parent_q->ne->point;
        }

        if (quadtree_node_isleaf(node_parent_q->sw)) {
            distance_sw = compute_point_distance(node_parent_q->sw->point, querynode->point);

            pPoints[2] = node_parent_q->sw->point;
        }

        if (quadtree_node_isleaf(node_parent_q->se)) {
            distance_se = compute_point_distance(node_parent_q->se->point, querynode->point);

            pPoints[3] = node_parent_q->se->point;
        }

        distance = compare_point_distance(distance_nw, distance_ne, distance_sw, distance_se);

        if (distance == distance_nw) {
            return pPoints[0];
        } else if (distance == distance_ne) {
            return pPoints[1];
        } else if (distance == distance_sw) {
            return pPoints[2];
        } else {
            return pPoints[3];
        }

    } else {
        querynode = quadtree_search_parentnode(tree->root, node_parent_q);
        quadtree_search_nearest_point(tree, querynode);
    }

    return NULL;
}


double
compute_point_distance(quadtree_point_t *point, quadtree_point_t *query_point) {
    double deltaY = query_point->y - point->y;
    double deltaX = query_point->x - point->x;
    double distance = 0;

    if (deltaY < 0) {
        deltaY = -deltaY;
    }

    if (deltaX < 0) {
        deltaX = -deltaX;
    }

    distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

    return distance;
}

double
compare_point_distance(double distance_nw, double distance_ne, double distance_sw, double distance_se) {
    double distance = distance_nw;

    double d[4] = {distance_nw, distance_ne, distance_sw, distance_se};

    for (int i = 0; i < 4; ++i) {
        if (distance >= d[i]) {
            distance = d[i];
        }
    }

    return distance;
}

