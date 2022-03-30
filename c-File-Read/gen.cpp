#include <cstdio>
#include <cstdlib>

// ./gen <file_name> <file_size (count by mb)>
int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: <file_name> <file_size (count by mb)>");
        return 0;
    }

    FILE* file = fopen(argv[1], "w+");
    int size = atoi(argv[2]) * 1024 * 1024;

    char* buffer = new char[size];

    int count = atoi(argv[2]);
    for (int i = 0; i < count; i++) {
        fwrite(buffer, sizeof(char), size, file);
    }

    printf("Generate a file %s with size %d mb\n", argv[1], count);
}