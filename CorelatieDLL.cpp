#include <windows.h>
#include <math.h>

//marcare functii exportate din DLL
#define DllExport __declspec(dllexport)

//declarare functie exportata
extern "C" DllExport double CoeficientCorelatie(double* X, double* Y, int N);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved)
{
    //initializari
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:  //atasare la un proces
    case DLL_THREAD_ATTACH:   //fir creat in proces atasat
    case DLL_THREAD_DETACH:   //terminare fir
    case DLL_PROCESS_DETACH:  //descarcare DLL
        break;
    }
    return TRUE;
}

//functia exportata de calculare a coeficientului
extern "C" DllExport double CoeficientCorelatie(double* X, double* Y, int N)
{
    if (N <= 0) 
        return 0.0;

    double MX = 0, MY = 0; //medii aritmetice pt vectori
    //calculare medie pentru fiecare vector
    for (int i = 0; i < N; i++) {
        MX = MX + X[i];
        MY = MY + Y[i];
    }
    MX = MX / N;
    MY = MY / N;

    //calculare numarator si numitor
    double num = 0, denX = 0, denY = 0;
    for (int i = 0; i < N; i++) {
        num = num + (X[i] - MX) * (Y[i] - MY); //numarator
        denX = denX + pow(X[i] - MX, 2);  //partea cu X din numitor, cu ridicare la puterea 2
        denY = denY + pow(Y[i] - MY, 2);  //partea cu Y din numitor, cu ridicare la puterea 2
    }

    if (denX == 0 || denY == 0)
        return 0.0;

    //returnare rezultat
    return num / sqrt(denX * denY);
}
