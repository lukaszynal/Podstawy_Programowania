// Program: PP_wskazniki v.01, 28.12.2021
// Autor: Szynal Lukasz, 150063, 2020/2021, Informatyka, D2, II semestr

/*
Napisz program wczytujący tablicę dziesięcioelementową „wypełnioną” dowolnymi liczbami całkowitymi (niech będą pobrane od użytkownika). Następnie, wykorzystując wskaźniki, wyświetl:
a) wartość pierwszego elementu tablicy,
b) wartość elementu piątego,
c) zawartość całej tablicy.
*/

#include <iostream>

// Wypełnianie elementów tablicy pobranymi od użytkownika.
void fillingArr(int arr[]) {
    for (int i = 0; i < 10; i++) {
        std::cout << "Podaj wartosc " << i + 1 << " elementu tablicy: ";
        std::cin >> arr[i];
    }
}

// Wyświetlanie wskazanych elementów tablicy za pomocą wskaźnika.
void printingResult(int *ptr) {
    std::cout << std::endl << "Pierwszy element tablicy: " << *ptr << std::endl;
    std::cout << "Piaty element tablicy:    " << *(ptr + 4) << std::endl << std::endl;

    std::cout << "Zawartosc calej tablicy:" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << *(ptr + i) << std::endl;
    }
}

int main()
{
    // Deklaracja zmiennych. Tablica 10-elementowa arr[10] oraz zmienna wskaźnikowa *ptr, której przypisujemy wartość komórki pamięci dla pierwszego elementu tablicy. 
    int *ptr, arr[10];
    ptr = &arr[0];
    
    fillingArr(arr);
    printingResult(ptr);  
}
