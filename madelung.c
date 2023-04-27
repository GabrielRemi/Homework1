#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

// gibt 1 wieder bei gerader Zahl und -1 bei ungerader Zahl
int odd(int num)
{
    return num % 2 == 0 ? 1 : -1;
}

// einfachster Algorithmus (sehr ineffizient)
double algo1(int n)
{
    double alpha = 0;

    double start_time = clock();

    for (int i = -n; i <= n; i++)
    {
        for (int j = -n; j <= n; j++)
        {
            for (int k = -n; k <= n; k++)
            {
                if(i == 0 && j == 0 && k == 0)
                    continue;
                
                double val = odd(i+j+k)/(sqrt(i*i + j*j + k*k));
                
                // Evjen Methode
                if (i == abs(n) && j == abs(n) && k == abs(n)) val /= 8;
                else if ((i == abs(n) && j == abs(n)) || (i == abs(n) && k == abs(n)) || (j == abs(n) && k == abs(n))) val /= 4;
                else if (i == abs(n) || j == abs(n) || k == abs(n)) val /= 2;
                alpha += val;
        
            }
        }
    }

    double time_diff = ((double )clock() - start_time)/CLOCKS_PER_SEC;
    printf("Dauer %.5f s\n", time_diff);

    return -alpha;
}

int main()
{
    
    printf("%f", algo1(100));
}