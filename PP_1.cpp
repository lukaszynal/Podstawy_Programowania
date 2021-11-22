//Program: PP_1 v.01, 16.03.2021
//Autor: Szynal Lukasz, 150063, 2020/2021, Informatyka, D2, I semestr

#include <iostream>
using namespace std;

float cena_instalacji, cena_gaz, cena_benzyna, przebieg, spalanie_benzyna;
float spalanie_gaz, koszt_benzyna, koszt_gaz, roznica_ceny_1km, oszczednosc;
int wynik; // Wynik zaokraglony do pelnego miesiaca

void Pobierz_Dane() // Pobieranie danych od uzytkownika
{
    cout << "Podaj cene instalacji gazowej: "; cin >> cena_instalacji;
    cout << "Podaj cene litra gazu: "; cin >> cena_gaz;
    cout << "Podaj cene litra benzyny: "; cin >> cena_benzyna;
    cout << "Podaj szacowany miesieczny przebieg: "; cin >> przebieg;
    cout << "Podaj spalanie benzyny (na 100 km): "; cin >> spalanie_benzyna;
}

float Oblicz() // Obliczanie wyniku na podstawie pobranych danych
{
    koszt_benzyna = cena_benzyna * spalanie_benzyna;
    spalanie_gaz = spalanie_benzyna + (spalanie_benzyna * 0.1) ;
    koszt_gaz = spalanie_gaz * cena_gaz;
    roznica_ceny_1km = (koszt_benzyna - koszt_gaz) / 100;
    oszczednosc = przebieg * roznica_ceny_1km;
    wynik = cena_instalacji / oszczednosc;
    return wynik;
}

// blok glowny programu

int main()
{
    cout << ("Kalkulator oplacalnosci instalacji gazowej") << endl;
    cout << ("Autor: Lukasz Szynal, 150063, 2020/2021, Informatyka, D2, I semestr") << endl;
    cout << ("====================================================================") << endl;
    cout << endl;
    Pobierz_Dane();
    cout << endl;
    cout << "Instalacja zwroci sie po " << Oblicz() << " miesiacach." << endl;
    return 0;
}
