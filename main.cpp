#include <windows.h>
#include <string>
#include <vector>
#include "resource.h"

//importare functie din DLL si declararea functiei importate
#define DllImport __declspec(dllimport)
extern "C" DllImport double CoeficientCorelatie(double* X, double* Y, int N);

//functia Dialog
INT_PTR CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //butoane,controale
    switch (msg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_CALCULEAZA) //butonul calculeaza
        {
            char buffer[256];
            GetDlgItemTextA(hwnd, IDC_EDIT_N, buffer, 255);  //citeste valoarea lui N
            int N = atoi(buffer);  //converteste din string in int (din caracter in numar)

            //verificam valoarea lui N, daca e negativa da eroare
            if (N <= 0) {
                MessageBoxA(hwnd, "Valoare invalida pentru N!", "Eroare", MB_OK | MB_ICONERROR);
                return TRUE;
            }

            //citire vector X
            GetDlgItemTextA(hwnd, IDC_EDIT_X, buffer, 255);
            std::string sx(buffer);

            //citire vector Y
            GetDlgItemTextA(hwnd, IDC_EDIT_Y, buffer, 255);
            std::string sy(buffer);

            std::vector<double> X, Y;
            double val;
            size_t pos;
            //prelucrare vector X
            while ((pos = sx.find(',')) != std::string::npos) {  //dupa fiecare valoare punem virgula
                val = atof(sx.substr(0, pos).c_str());  //convertim din caractere in numere reale
                X.push_back(val);
                sx.erase(0, pos + 1);  //elimina valorile procesate
            }
            X.push_back(atof(sx.c_str()));

            //prelucrare vector Y
            while ((pos = sy.find(',')) != std::string::npos) {
                val = atof(sy.substr(0, pos).c_str());
                Y.push_back(val);
                sy.erase(0, pos + 1);
            }
            Y.push_back(atof(sy.c_str()));

            //verifica daca numarul de valori ale lui X corespunde cu N, daca nu da eroare
            if (X.size() != N) {
                MessageBoxA(hwnd, "Vectorul X trebuie sa aiba N elemente!", "Eroare", MB_OK | MB_ICONERROR);
                return TRUE;
            }
            
            //verifica daca numarul de valori ale lui Y corespunde cu N, daca nu da eroare
            if (Y.size() != N) {
                MessageBoxA(hwnd, "Vectorul Y trebuie sa aiba N elemente!", "Eroare", MB_OK | MB_ICONERROR);
                return TRUE;
            }

            //apel functie din DLL
            double R = CoeficientCorelatie(X.data(), Y.data(), N);

            //afisare rezultat in caseta corespunzatoare
            char rez[64];
            sprintf_s(rez, "%.6f", R);  //formatare rezultat cu 6 zecimale dupa virgula
            SetDlgItemTextA(hwnd, IDC_RESULT, rez);
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return TRUE;
    }
    return FALSE;
}

//functia principala care creaza dialogul 
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    return (int)DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}
