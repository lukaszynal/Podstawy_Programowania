// Program: PP_zarzadzanie_pamiecia v.01, 28.12.2021
// Autor: Szynal Lukasz, 150063, 2020/2021, Informatyka, D2, II semestr

/*
Napisz program który będzie pobierał od uzytkownika ciagi znaków.
Każdy ciąg będzie wpisany do tablicy, podanie 0 przez użytkownika ma zakończyć wprowadzanie ciągów.
Podanie przez użytkownika nowego elementu do wpisania ma wiązać się z utworzeniem nowej tablicy do której będą przepisane wartości ze starej tablicy i dopisany nowy elelement. 
Stara tablica ma zostać usunięta po tej operacji.
*/

#include <iostream>

using namespace std;

int main()
{
    // Licznik przechowujący ilość wpisanych ciągów znaków.
    int count = 0;
    string ciagZnakow;
    // Zmienne wskaźnikowe tablic.
    string *array, *auxiliaryArray;

    // Tworzymy nową tablicę dynamiczną.
    array = new string[count + 1];

    // Pętla, z której możemy wyjść tylko poprzez wpisanie "0" jako ciag znaków.
    while (true)
    {     
        cout << "Wpisz tekst: ";
        cin >> ciagZnakow;

        if (ciagZnakow == "0") break;
        else {
            // Zapisujemy wprowadzony ciag znaków do tablicy.
            array[count] = ciagZnakow;
            // Inkrementujemy licznik ilości wpisanych ciągów.
            count++;

            // Tworzymy pomocniczą tablicę i przepisujemy do niej wszystkie wartości ze starej tablicy. Starą tablicę usuwamy po tym zabiegu.
            auxiliaryArray = new string[count];
            for (int i = 0; i < count; i++)
                auxiliaryArray[i] = array[i];
            delete[] array;
            
            // Tworzymy tablicę powiększoną o jeden element. Usuwamy tablicę pomocnicza.
            array = new string[count + 1];
            for (int i = 0; i < count; i++)
                array[i] = auxiliaryArray[i];
            delete[] auxiliaryArray;
        }
    }

    // Wyświetlamy zawartość całej tablicy a następnie usuwamy ją z pamięci.
    cout << endl << "Zawartosc calej tablicy:" << endl;
    for (int i = 0; i < count; i++)
        cout << array[i] << endl;

    delete[] array;
}
