#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// 2^14 = 16384
#define NUM 16384

static double *pPointX;
static double *pPointY;

void
generate_random_num() {
    pPointX = (double*)malloc(sizeof(double) * NUM);
    pPointY = (double*)malloc(sizeof(double) * NUM);
    
    for (int i = 0; i < NUM; ++i) {
        pPointX[i] = rand() / (double)(RAND_MAX/NUM);
        pPointY[i] = rand() / (double)(RAND_MAX/NUM);
    }
}

int main(int argc, char *argv[]) {

    generate_random_num();
    
    // test function < void generate_random_num() >
    printf("test function < void generate_random_num() >\n");
    printf("----------------------------------------------");
    for (int n = 0; n < 1000; ++n) {
        printf("pPointX[%d] %f pPointY[%d] %f\n", n, pPointX[n], n, pPointY[n]);
    }
    printf("----------------------------------------------");
    
    return 0;
}

