// Program: PP_projekt v.01, 29.12.2021
// Autor: Szynal Lukasz, 150063, 2020/2021, Informatyka, D2, II semestr

/*
Stwórz aplikację kalkulator.
Niech program pobiera od użytkownika rodzaj działania i dane na których zostaną wykonane obliczenia.
Proszę pamietać o obsłużeniu wyjątków.
Sporządź krótkie sprawozdanie które będzie opisywać jak należy korzystać z programu, przedstaw algorytm i krótkie podsumowanie.
*/

#include <iostream>
#include <iomanip>
#include <math.h>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class Calculator {
public:
    //Przepisujemy liczby i operatory do wektora dbając o poprawność wprowadzonych danych.
    vector<string> parse(const string& input)
    {
        vector<string> vec;
        string current;
        int parenthesisOpen = 0, parenthesisClose = 0;
        bool firstIsNegative = false, lastWasDot = false, lastWasOperator = false, lastWasParenthesis = false;

        if (input[0] == '-')
            firstIsNegative = true;

        for (char c : input)
        {
            string s(1, c);

            if (isLetter(s)) {
                throw string("Wyrazenie nie moze zawierac liter!");
                break;
            }
            else if (!isdigit(c) && !isAcceptableChar(s)) {
                throw string("Nieobslugiwany operator!");
                break;
            }

            else if (isdigit(c) || (c == '.') || lastWasDot || firstIsNegative) {
                current += c;
                if (firstIsNegative)
                    firstIsNegative = false;

                if (c == '.')
                {
                    lastWasDot = true;
                }
                else {
                    lastWasDot = false;
                }
                lastWasOperator = false;
            }

            else if (c)
            {
                if ((lastWasOperator && c == '-') && !lastWasParenthesis)
                    current += c;
                else {
                    if (lastWasOperator && c == '(' && c == ')')
                        throw string("Operatory obok siebie sa zabronione!");
                    if (!current.empty())
                    {
                        vec.emplace_back(move(current));
                        current = "";
                    }
                    if (c != ' ')
                        vec.emplace_back(1, c);
                    lastWasOperator = true;
                    if (c == '(')
                        parenthesisOpen++;
                    if (c == ')') {
                        parenthesisClose++;
                        lastWasParenthesis = true;
                    } 
                    else
                        lastWasParenthesis = false;
                }
            }
        }
        if (parenthesisOpen != parenthesisClose)
            throw string("Niezgodnosc nawiasow!");
        if (!current.empty())
            vec.push_back(move(current));
        return vec;
    }

    //Sprawdzamy która funkcje wywowałać w zależności od otrzymanych danych.
    double perform(vector<string> notation)
    {
        if (notation.size() < 2)
            throw string("Niewystarczajaca liczba danych!");

        else if (!isNumber(notation[0]) && notation[0] != "(")
            throw string("Pierwszy argument musi byc liczba!");
        
        else if (notation[1] == "!") {
            if (notation.size() == 2) {
                if (isNatural(notation[0]))
                    return calculateOneArg(notation);
                else
                    throw string("Silnie obliczamy tylko dla liczb naturalych!");
            }
            else
                throw string("Niepoprawna ilosc lub wartosc argumentow dla tego dzialania!");
        }
        
        else if (notation[1] == "^" || notation[1] == "V" || notation[1] == "%") {
            if (notation.size() == 3 && isNumber(notation[2]))
                if(notation[1] == "V" && stod(notation[2]) <= 0)
                    throw string("Liczba pod pierwiastkiem musi być liczba dodatnia!");
                else
                return calculateTwoArg(notation);
            else
                throw string("Niepoprawna ilosc lub wartosc argumentow dla tego dzialania!");
        }
        else if (notation.size() > 2) {
            vector<string> output = convertToRPN(notation);
            return evalRPN(output);
        }
        else
            throw string("Niewystarczajaca liczba danych!");
    }
    
    void help() 
    {
        cout <<
            "++=============================================================================++\n"
            "|| Kalkulator oblicza zlozone dzialania arytmetyczne do ktorych naleza:        ||\n"
            "|| dodawanie(+), odejmowanie(-), mnozenie(*), dzielenie(/)                     ||\n"
            "|| Mozna rowniez uzywac nawiasow. Np: 2 * (10 / 2) - 2                         ||\n"
            "||                                                                             ||\n"
            "|| Oblicza rowniez inne dzialania takie jak:                                   ||\n"
            "|| potega(^), pierwiastek(V), silnia(!) oraz procent(%)                        ||\n"
            "|| Jednak w tym przypadku musimy stosowac sie do kilku wytycznych:             ||\n"
            "|| * Brak mozliwosci laczenia dzialan!                                         ||\n"
            "|| * Potegowanie: Podstawa potegi ^ Wykladnik potegi. Np: 3^2                  ||\n"
            "|| * Pierwiastowanie: Stopien pierwiastka V Liczba Pierwiastkowana. Np: 2V9    ||\n"
            "|| * Silnia: Liczba!. Np: 5!                                                   ||\n"
            "|| * Procent: Procent % z liczby. Np. 10%100                                   ||\n"
            "++=============================================================================++" << endl << endl;
    }

private:
    //Funkcja obslugująca działanie z jedną liczbą i jednym operatorem.
    unsigned long long calculateOneArg(vector<string> notation)
    {
        string o = notation[1];
        int x = stoi(notation[0]);
        char oper = o[0];

        switch (oper)
        {
        case '!':
            return strong(x) ;
        default:
            return 0;
        }
    }
    // Funkcja obslugująca działanie z dwiema liczbami i jednym operatorem.
    double calculateTwoArg(vector<string> notation)
    {
        string o = notation[1];
        double x = stod(notation[0]);
        char oper = o[0];
        double y = stod(notation[2]);

        switch (oper)
        {
        case '^':
            return pow(x, y);
        case 'V':
            return pow(y, 1 / x);
        case '%':
            return (x / 100 * y);
        default:
            return 0.0;
        }
    }

    // Funkcja rozwiązująca wyrażenie zapisane w odwrotnej notacji polskiej.
    double evalRPN(vector<string>& tokens) {
        if (tokens.empty()) {
            return 0;
        }
        stack<string> s;
        for (const auto& tok : tokens) {
            if (!isOperator(tok)) {
                s.emplace(tok);
            }
            else {
                double y = stod(s.top());
                s.pop();
                double x = stod(s.top());
                s.pop();
                if (tok[0] == '+') {
                    x += y;
                }
                else if (tok[0] == '-') {
                    x -= y;
                }
                else if (tok[0] == '*') {
                    x *= y;
                }
                else if (tok[0] == '/') {
                    if (y == 0)
                        throw string("Dzielenie przez 0 jest zabronione!");
                    else x /= y;
                }
                else throw string("W przypadku ciagu wyrazen obslugiwane sa tylko operatory: '+', '-', '*', '/'");
                s.emplace(to_string(x));
            }
        }
        return stod(s.top());
    }

    // Konwersja wyrażenia infixowego znajdującego się w wektorze na zapis w postacji postfixowej.
    vector<string> convertToRPN(vector<string>& expression) {
        vector<string> output;
        infixToPostfix(expression, output);
        return output;
    }

    void infixToPostfix(const vector<string>& infix, vector<string>& postfix) {
        stack<string> s;
        for (string tok : infix) {
            if (atoi(tok.c_str())) {
                postfix.emplace_back(tok);
            }
            else if (tok == "(") {
                s.emplace(tok);
            }
            else if (tok == ")") {
                while (!s.empty()) {
                    tok = s.top();
                    s.pop();
                    if (tok == "(") {
                        break;
                    }
                    postfix.emplace_back(tok);
                }
            }
            else {
                while (!s.empty() && precedence(tok) <= precedence(s.top())) {
                    postfix.emplace_back(s.top());
                    s.pop();
                }
                s.emplace(tok);
            }
        }
        while (!s.empty()) {
            postfix.emplace_back(s.top());
            s.pop();
        }
    }

    // Kolejność wykonywania działań.
    int precedence(string x) {
        if (x == "(") {
            return 0;
        }
        else if (x == "+" || x == "-") {
            return 1;
        }
        else if (x == "*" || x == "/") {
            return 2;
        }
        return 3;
    }

    // Rekurencyjna funkcja obliczająca silnię.
    unsigned long long strong(unsigned long long n)
    {
        if (n < 2)
            return 1;

        return n * strong(n - 1);
    }

    bool isNumber(const string& number) {
        bool condition = false;
        for (const auto& tok : number) {
            if (isdigit(tok) || tok == '.' || tok == '-') {
                condition = true;
            }  
            else {
                condition = false;
                break;
            }
        }
        return condition;
    }

    bool isNatural(const string& number) {
        double x = stod(number);
        if ((x - int(x) == 0) && (x >= 0)) return true;
        else return false;
    }

    bool isLetter(const string& letter) {
        return letter.length() == 1 && string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUWXYZ").find(letter) != string::npos;
    }

    bool isOperator(const string& op) {
        return op.length() == 1 && string("+-*/^V!%").find(op) != string::npos;
    }

    bool isAcceptableChar(const string& op) {
        return op.length() == 1 && string("+-*/^V!%(). ").find(op) != string::npos;
    }
};

int main() {
    string input;
    double result = 0.0;
    Calculator c;
    cout <<
        "++================================++\n"
        "|| CALCULATOR CONSOLE APPLICATION ||\n"
        "||                                ||" << endl;
    c.help();

    while (true) {
        cout << "Wprowadz operacje do wykonania." << endl;
        getline(cin, input);
        
        if (input == "help")
            c.help();
        else if (input != "") {     
            try {
                vector<string> notation = c.parse(input);
                result = c.perform(notation);
                cout << "Wynik: " << fixed << result << endl;
                cout << "++=============================================================================++" << endl << endl;
            }
            catch (string& exc) {
                cout << exc << endl;
                cout << "++=============================================================================++" << endl << endl;
            }  
        }
        else {   
            cout << "Jesli chcesz zakonczyc, zamknij okno. Jesli potrzebujesz pomocy wpisz: 'help'" << endl;
            cout << "++=============================================================================++" << endl << endl;
        }
    }   
}
