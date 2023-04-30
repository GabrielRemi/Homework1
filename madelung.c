#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define MADELUNG 1.747564594633

// gibt 1 wieder bei gerader Zahl und -1 bei ungerader Zahl
int odd(int num)
{
    return num % 2 == 0 ? 1 : -1;
}

// berechnet den Betrag einer Zahl des Typs double
double absd(double x)
{
    return x < 0? -x: x;
}

double algo1(int n)
{
     double alpha = 0;

    //double start_time = clock();

    double val = 0;  // temporäre Variable zum speichern der momentanen Rechnung

    for (int i = -n; i <=n; i++)
    { 
        for (int j = -n; j <= n; j++)
        {
            for (int k = -n; k <= n; k++)
            {
                if (i == 0 && j == 0 && k == 0) continue;

                val = odd(i+j+k)/(sqrt(i*i + j*j + k*k));

                //Evjen Methode
                if (abs(i) == n && abs(j) == n && abs(k) == n) val/=8;
                else if ((abs(i) == n && abs(j) == n) || (abs(i) == n && abs(k) == n) || (abs(j) == n && abs(k) == n)) val/=4;
                else if (abs(i) == n || abs(j) == n || abs(k) == n) val/=2;
                
                alpha += val;
            }
        }
    }


    return -alpha;
}

// summiert von 1 bis n (effektiver)
double evjen_method(int n)
{   
    double alpha = 0;

    //double start_time = clock();

    double term1 = 0;
    double term2 = 0;
    double term3 = 0;
    double val = 0;  // temporäre Variable zum speichern der momentanen Rechnung

    for (int i = 1; i <=n; i++)
    {
        val = odd(i)/(double) i;

        //Evjen Methode
        if (i == n) term3 += val /2; 
        else term3 += val;
         
        for (int j = 1; j <= n; j++)
        {
            val = odd(i + j)/(sqrt(i*i + j*j));

            //Evjen Methode
            if (i == n && j == n) term2 += val/4;
            else if (i == n || j == n) term2 += val/2;
            else term2 += val;

            for (int k = 1; k <= n; k++)
            {
                val = odd(i+j+k)/(sqrt(i*i + j*j + k*k));

                //Evjen Methode
                if (i == n && j == n && k == n) term1 += val/8;
                else if ((i == n && j == n) || (i == n && k == n) || (j == n && k == n)) term1 += val/4;
                else if (i == n || j == n || k == abs(n)) term1 += val/2;
                else term1 += val;
            }
        }
    }

    alpha = 8*term1 + 12*term2 + 6*term3;


    //double time = (clock()-start_time)/CLOCKS_PER_SEC;
    //printf("Dauer: %.5f s\n", time);

    return -alpha;
}

//##############################################
//                 MAIN
//##############################################
int main()
{
    int n = 1;
    double eps_rel = 1e-8; // gewünschte relative Genauigkeit des Wertes
    double eps = 1e-6; // gewünschte Genauigkeit des Wertes
    double current_eps_rel;
    double current_eps;

    // berechne für erstes n einen Wert für die Madelung Konstante
    double old_madelung = evjen_method(n);

    
    double madelung;

    // Messe Zeit der While Schleife
    double start_time = clock();

    while (1)
    {
        // Erhöhe n und berechne die neue Konstante
        n += 1;
        madelung = evjen_method(n);
        // vergleiche beide Werte miteinander und wiederhole das Verfahren, wenn die Differenz zu hoch ist
        current_eps_rel = absd(madelung - old_madelung)/madelung;
        current_eps = absd(madelung - old_madelung);

        old_madelung = madelung;

        if (current_eps_rel < eps_rel || n > 300) break;
    }
    
    double time = (clock() - start_time)/CLOCKS_PER_SEC;

    printf("Iteration: n = %d\n", n);
    printf("Madelung Konstante: %.5f\n", madelung);
    printf("Referenzwert: %.5f\n", MADELUNG);
    printf("Dauer: %.3f s", time);
}