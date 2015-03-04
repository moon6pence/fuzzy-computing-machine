#include "Timer.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

using namespace std;

const int N = 10;
const int WIDTH = 1024, HEIGHT = 1024;

void process(float *A, float *B, float *C, int WIDTH, int HEIGHT)
{
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
        {
            float a = A[y * HEIGHT + x];
            float b = B[y * HEIGHT + x];

            C[y * HEIGHT + x] = sqrt(a * a + b * b);
        }
}

int main()
{
    puts("Hello World!");

    //int fd = open("input.dat", O_RDONLY);
    int fd = open("network/input.dat", O_RDONLY);
    if (fd == -1)
    {
        puts("Cannot read input.dat");
        return -1;
    }

    float *A = new float[WIDTH * HEIGHT];
    float *B = new float[WIDTH * HEIGHT];
    float *C = new float[WIDTH * HEIGHT];

    // generate b
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            B[y * HEIGHT + x] = (float)rand();

    Timer t0("all");

    for (int index = 0; index < N; index++)
    {
        Timer t1("read");

        // read a
        read(fd, A, WIDTH * HEIGHT * sizeof(float));

        t1.print();

        Timer t2("process");

        // process
        process(A, B, C, WIDTH, HEIGHT);

        for (int i = 0; i < 10; i++)
            process(B, C, C, WIDTH, HEIGHT);

        // for (int y = 0; y < HEIGHT; y++)
        //     for (int x = 0; x < WIDTH; x++)
        //         C[y * WIDTH + x] = A[y * WIDTH + x] + B[y * WIDTH + x];

        t2.print();
    }

    t0.print();

    delete[] A;
    delete[] B;
    delete[] C;

    close(fd);

    return 0;
}