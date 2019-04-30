//
// Created by Carillon on 2019/4/29.
//

#include "ore/ore.h"
#include "ore/errors.h"
#include "ore/flags.h"

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>


#define ERR_CHECK(x) if((_err = x) != ERROR_NONE) { return _err; }
// 16384 = 2 ^ 14
#define NUM 16384

static int _err;
static double *pPointX;
static double *pPointY;

// generate the random num for points location (x, y)
void
generate_random_num() {
    pPointX = (double*)malloc(sizeof(double) * NUM);
    pPointY = (double*)malloc(sizeof(double) * NUM);

    for (int i = 0; i < NUM; ++i) {
        pPointX[i] = rand() % NUM;
    }

    for (int j = 0; j < NUM; ++j) {
        pPointY[j] = rand() % NUM;
    }
}



int main() {

    for (int i = 0; i < 10; ++i) {
        printf("x is %f ", pPointX[i]);
        printf("\n");
        printf("y is %f ", pPointY[i]);
    }



    return 0;
}