#include <stdio.h>
#include <math.h>

// for test 
const char* FILE_PATH = "D:\\GitBranch\\GitHub\\GrayFortune\\c-Calculate-With-Designed-Formula\\data.csv";

// constant define 
static const int    maxn = 1000;
static const char* LINE_FORMAT = " %lf,%lf,%lf\n";

double previous_formula(const char* filepath, double alpha, double beta)
{
    FILE* fp = fopen(FILE_PATH, "r");

    // ignore the first line
    char deprecated[8];
    fscanf(fp, "%s\n", deprecated);

    // sum second part
    double x, u, y, result;
    while (!feof(fp))
    {
        fscanf(fp, LINE_FORMAT, &x, &u, &y);
        result += pow((y - alpha - beta * x), 2);
        // printf("x: %.4lf\tu: %.4lf\ty: %.4lf result:%.4lf\n", x, u, y, result);
    }

    // calcualte first part
    double first = (double)maxn * -log(2 * 3.14159265358);
    result = first - result / 2;
    // printf("%lf", result);
    fclose(fp);
    return result;
}

double alpha_order_1_deri(const char* filepath, double alpha, double beta)
{
    FILE* fp = fopen(FILE_PATH, "r");

    // ignore the first line
    char deprecated[8];
    fscanf(fp, "%s\n", deprecated);

    // sum part
    double x, u, y, result;
    while (!feof(fp))
    {
        fscanf(fp, LINE_FORMAT, &x, &u, &y);
        result += y - alpha - beta * x;
    }

    // printf("%lf", result);
    fclose(fp);
    return result;
}


double alpha_order_2_deri(const char* filepath, double alpha, double beta)
{
    return -maxn;
}

double beta_order_1_deri(const char* filepath, double alpha, double beta)
{
    FILE* fp = fopen(FILE_PATH, "r");

    // ignore the first line
    char deprecated[8];
    fscanf(fp, "%s\n", deprecated);

    // sum part
    double x, u, y, result;
    while (!feof(fp))
    {
        fscanf(fp, LINE_FORMAT, &x, &u, &y);
        result += x * (y - alpha - beta * x);
    }

    // printf("%lf", result);
    fclose(fp);
    return result;
}

double beta_order_2_deri(const char* filepath, double alpha, double beta)
{
    FILE* fp = fopen(FILE_PATH, "r");

    // ignore the first line
    char deprecated[8];
    fscanf(fp, "%s\n", deprecated);

    // sum part
    double x, u, y, result;
    while (!feof(fp))
    {
        fscanf(fp, LINE_FORMAT, &x, &u, &y);
        result += - (x * x);
    }

    // printf("%lf", result);
    fclose(fp);
    return result;
}

int main()
{
    const double alpha = 0.1, beta = 0.1;

    // previous result 
    double previous = previous_formula(FILE_PATH, alpha, beta);
    printf("%lf\n", previous);

    // 1-order derivative 
    double alpha_1 = alpha_order_1_deri(FILE_PATH, alpha, beta);
    double beta_1 = beta_order_1_deri(FILE_PATH, alpha, beta);
    printf("ahpha'1: %lf, beta'1: %lf\n", alpha_1, beta_1);

    // 2-order derivative
    double alpha_2 = alpha_order_2_deri(FILE_PATH, alpha, beta);
    double beta_2 = beta_order_2_deri(FILE_PATH, alpha, beta);
    printf("ahpha'2: %lf, beta'2: %lf\n", alpha_2, beta_2);
}

