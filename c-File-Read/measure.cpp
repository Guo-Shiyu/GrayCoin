#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <ctime>

// ./run2 <filename> <block_size>
int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: ./run2 <filename> <block_size (count by kb)>\n");
        return 0;
    }

    char* filename = argv[1];
    int block_size = atoi(argv[2]) * 1024;

    int fd = open(filename, O_RDONLY);
    char* buf = new char[block_size];
    int block_counter = 0;

    int begin = clock();
    while(read(fd, buf, block_size) != 0)
        block_counter++;
    int end = clock();

    printf("block size = %d kb\nblock count = %d\ntime cost = %d ms\nspeed = %.3f mb/s", 
            atoi(argv[2]),      block_counter,      end - begin,    (float)((float)(block_counter * atoi(argv[2])) / 1024.0 / (end - begin) * 1000.0));
    
    return 0;
}