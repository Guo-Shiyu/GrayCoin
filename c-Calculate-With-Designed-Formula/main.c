#include <stdio.h>
#include <math.h>

const char*  FILE_PATH = "D:\\qq\\2821006329\\filerecv\\data.csv";
const char*  LINE_FORMAT = " %lf,%lf,%lf\n";

const double constant = -log(2 * 3.14159265358);
const double alpha = 0.1;
const double beta = 0.1;
const int    maxn = 1000;

double calculate_second(double x, double u, double y)
{
    return pow((y - alpha - beta * x), 2);
}

int main()
{
    FILE *fp = fopen(FILE_PATH, "r");

    // ignore the first line
    char deprecated[8]; 
    fscanf(fp, "%s\n", deprecated);

    // sum second part
    double x, u, y, result;
    while(!feof(fp))
    {
        fscanf(fp, LINE_FORMAT, &x, &u, &y);
        result += calculate_second(x, u, y);
        // printf("x: %.4lf\tu: %.4lf\ty: %.4lf result:%.4lf\n", x, u, y, result);
    }

    // calcualte first part
    double first = (double)maxn * constant;
    result = first - result / 2;
    printf("%lf", result);
}