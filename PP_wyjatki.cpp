// Program: PP_zadanie3 v.01, 29.12.2021
// Autor: Szynal Lukasz, 150063, 2020/2021, Informatyka, D2, II semestr

/*
Napisz program który będzie pobierał od użytkownika 2 liczby naturalne a i b a następnie prezentował wynik działania  a / b.
W tym liczby a i b mają być całkowite a my poruszamy się w zakresie liczb naturalnych więc wynik nie może być ułamkiem.
Przeanalizuj wszystkie możliwe sytacje i stwórz odpowiednie wyjątki w kodzie.
*/

#include <iostream>
using namespace std;

// Procedura sprawdzajaca czy liczba podana przez uzytkownika nalezy do zbioru liczb naturalnych
bool isNatural(float x) {
	if ((x - int(x) == 0) && (x >= 0)) return true;
	else return false;
}

// Procedura sprawdzająca warunki dzielenia. W przypadku spełnienia wszystkich założeń zwraca wynik dzielenia a / b.
// Nie ma koniecznosci sprawdzania wszystkich mozliwych wyjątków, gdyż przy wprowadzaniu liczb upewniliśmy się, że wszystkie argumenty należa do liczb naturalnych.
float division(float a, float b) {
	if (b == 0)
		throw string("Dzielenie przez 0 jest zabronione!");
	if (a < b)
		throw string("Aby otrzymac liczbe naturalna dzielnik musi mniejszy od dzielnej!");
	if (int(a) % int(b) != 0)
		throw string("Iloraz tych liczb jest ulamkiem!");
	return a / b;
}

int main(void) {
	float a, b;
	bool isNaturalA = false, isNaturalB = false;
	
	cout << "Podaj dzielna oraz dzielnik nalezace do zbioru liczb naturalnych: " << endl;

	// Żądamy podania liczby naturalnej przez użytkownika aż do skutku.
	while (!isNaturalA) {
		cout << "Dzielna: ";
		cin >> a;
		isNaturalA = isNatural(a);
		if (!isNaturalA)
			cout << "Podana liczba nie nalezy do zbioru liczb naturalnych. Podaj inna liczbe:" << endl;
	}
	while (!isNaturalB) {
		cout << "Dzielnik: ";
		cin >> b;
		isNaturalB = isNatural(b);
		if (!isNaturalB)
			cout << "Podana liczba nie nalezy do zbioru liczb naturalnych. Podaj inna liczbe:" << endl;
	}
	
	// W przypadku spełnienia wszystkich warunków wyświetlany jest wynik dzielenia a / b.
	try {
		cout << "Wynik: " << a << " / " << b << " = " << division(a, b);
	}
	// Jeśli, któryś z warunków nie został spełniony to na ekranie zostanie wyświetlony komunikat informujący o przyczynie niepowodzenia.
	catch (string& exc) {
		cout << exc << endl;
	}
	return 0;
}
