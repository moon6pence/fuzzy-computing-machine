#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

const int N = 100;
const int WIDTH = 1024, HEIGHT = 1024;

int main()
{
    puts("Hello World!");

    int fd = open("input.dat", O_RDONLY);
    if (fd == -1)
    {
        puts("Cannot read input.dat");
        return -1;
    }

    float *A = new float[WIDTH * HEIGHT];
    float *B = new float[WIDTH * HEIGHT];
    float *C = new float[WIDTH * HEIGHT];

    for (int index = 0; index < N; index++)
    {
        // read a
        read(fd, A, WIDTH * HEIGHT * sizeof(float));

        // generate b
        for (int y = 0; y < HEIGHT; y++)
            for (int x = 0; x < WIDTH; x++)
                B[y * HEIGHT + x] = (float)rand();

        // process
        for (int y = 0; y < HEIGHT; y++)
            for (int x = 0; x < WIDTH; x++)
            {
                float a = A[y * HEIGHT + x];
                float b = B[y * HEIGHT + x];

                C[y * HEIGHT + x] = sqrt(a * a + b * b);
            }
    }

    delete[] A;
    delete[] B;
    delete[] C;

    close(fd);

    return 0;
}