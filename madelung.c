// Kompilier Befehl: gcc madelung.c -o madelung -lm
// Ausführbefehl: ./madelung

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<stdbool.h>

#define MADELUNG 1.747564594633 // Quelle: https://de.wikipedia.org/wiki/Madelung-Konstante
#define CHARGE 1.602176634e-19 // in Coulomb, Quelle: https://de.wikipedia.org/wiki/Elementarladung
#define VACUUM_PERMITTIVITY 8.8541878128e-12 // Farad/meter, Quelle: https://en.wikipedia.org/wiki/Vacuum_permittivity
#define LATTICE_CONSTANT 564e-12 // meter, Quelle: https://www.princeton.edu/~maelabs/mae324/glos324/nacl.htm#:~:text=NaCl%2C%20Sodium%20Chloride%2C%20Salt&text=The%20crystal%20lattice%20parameter%20is%200.563%20nm.


// definiere einen Funktionstypen, der für ein bestimmtes n eine Summe berechnet
typedef double (*sum_func)(int);


// gibt 1 wieder bei gerader Zahl und -1 bei ungerader Zahl
int odd(int num)
{
    return num % 2 == 0 ? 1 : -1;
}

// berechnet den Betrag einer Zahl des Typs double (Die Funktion abs() ist nur für integers definiert)
double absd(double x)
{
    return x < 0? -x: x;
}


//summiert nach evjen methode von 1 bis n (formel nach pdf dokument)
// Summe für Flachen Kristall über 2 Dimensionen
double evjen_method_2d(int n)
{
    double term1 = 0; // Erster Term, welcher über einen Index summiert wird
    double term2 = 0; // Zweiter Term, welcher über beide Indizes summiert wird
    double value;   // Laufvariable, in der Werte in der For-Schleife temporär gespeichert werden

    for (int i = 1; i <= n; i++)
    {
        value =  odd(i)/(double) i;
        
        //Evjen-Methode
        if (i == n) value /= 2; // Es handelt sich um eine Kante
        term1 += value;

        for (int j = 1; j <= n; j++)
        {
            value = odd(i+j)/(sqrt((double)i*i + j*j));
            
            // Evjen-Methode
            if (i == n && j == n) value /= 4; //Es handelt sich um eine Ecke
            else if (i == n || j == n) value /=2; //Es handelt sich um eine Kante

            term2 += value;

        }
    }

    return -4*(term1 + term2);
}


// summiert von 1 bis n (effektiver)
// Zur summation wurde die Formel aus dem PDF-Dokument verwendet (madelung = 8*term1 + 12*term2 +6*term3)
double evjen_method(int n)
{   
    double alpha = 0;  //Hier wird die Madelung-Konstante gespeichert

    double term1 = 0;   // Summation über 3 Indizes
    double term2 = 0;   // Summation über 2 Indizes
    double term3 = 0;   // Summation über 1 Index
    double val = 0;  // temporäre Variable zum speichern der momentanen Rechnung der Schleife

    for (int i = 1; i <=n; i++)
    {
        val = odd(i)/(double) i;

        //Evjen Methode
        if (i == n) term3 += val /2; // Oberfläche
        else term3 += val;
         
        for (int j = 1; j <= n; j++)
        {
            val = odd(i + j)/(sqrt(i*i + j*j));

            //Evjen Methode
            if (i == n && j == n) term2 += val/4;   //  Kante
            else if (i == n || j == n) term2 += val/2;  // Oberfläche
            else term2 += val;

            for (int k = 1; k <= n; k++)
            {
                val = odd(i+j+k)/(sqrt(i*i + j*j + k*k));
            
                //Evjen Methode
                if (i == n && j == n && k == n) term1 += val/8; // Ecke
                else if ((i == n && j == n) || (i == n && k == n) || (j == n && k == n)) term1 += val/4;    //Kante
                else if (i == n || j == n || k == abs(n)) term1 += val/2;   // Oberfläche
                else term1 += val;
            }
        }
    }

    alpha = 8*term1 + 12*term2 + 6*term3;


    return -alpha;
}

/*Berechnet den Wert, gegen die eine Summe konvergiert. 
Hierfür wird die summe für ein n sowie n+1 ausgewertet und die relative Differenz berechnet.
Dieser Prozess wird so lange geführt, bis die gewünschte Genauigkeit des Wertes erreicht ist. 
Falls eine Konvergenz nicht erreicht wird, wird die Schleife vorzeitig abgebrochen
Dies ist für beliebige Funktionen des typs double mit einem Argument des Typs int möglich*/
double calculate_convergence(sum_func f)
{
    int n = 1;
    double eps_rel = 1e-8; // gewünschte relative Genauigkeit des Wertes
    double current_eps_rel;// momentane relative Genauigkeit in der späteren while-Schleife
    

    // berechne für erstes n einen Wert für die Madelung Konstante
    double old_value = f(n);

    // hier wird der Wert der Konvergenz gespeichert
    double value;

    // Messe Zeit der While Schleife
    double start_time = clock();

    while (1)
    {
        // Erhöhe n und berechne die neue Konstante
        n += 1;
        value = f(n);

        // vergleiche beide Werte miteinander und wiederhole das Verfahren, wenn die Differenz zu hoch ist
        current_eps_rel = absd((value - old_value)/value);
    
        old_value = value;

        if (current_eps_rel < eps_rel || n > 300) break;
    }
    
    double time = (clock() - start_time)/CLOCKS_PER_SEC;
    printf("Konvergenz bei n = %d\n", n);

    return value;
}


// berechne die Bindungsenergie im Kristal in eV
double potential_energy(double madelung)
{
    return - madelung* 1/(4*M_PI*VACUUM_PERMITTIVITY) *CHARGE/LATTICE_CONSTANT;
}

//##########################################################################
//                 MAIN
//##########################################################################
int main()
{
    //######################################################
    //                  AUFGABE 2
    //##############################################
    printf("AUFGABE 2\n");
    double madelung = calculate_convergence(evjen_method);
    printf("Madelung Konstante: %.5f\n", madelung);
    printf("Referenzwert: %.5f\n", MADELUNG);
    //printf("Dauer: %.3f s\n", time);

    printf("Bindungsenergie: V = %.5f eV\n\n", potential_energy(madelung));

    //######################################################
    //                  AUFGABE 3
    //######################################################
    printf("AUFGABE 3\n");
    madelung = calculate_convergence(evjen_method_2d);
    printf("Madelung Konstante: %.5f\n", madelung);

    printf("Bindungsenergie: V = %.5f eV\n\n", potential_energy(madelung));


}