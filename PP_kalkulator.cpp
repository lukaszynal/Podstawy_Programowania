// Program: PP_projekt v.01, 29.12.2021
// Autor: Szynal Lukasz, 150063, 2020/2021, Informatyka, D2, II semestr

/*
Stwórz aplikację kalkulator.
Niech program pobiera od użytkownika rodzaj działania i dane na których zostaną wykonane obliczenia.
Proszę pamietać o obsłużeniu wyjątków.
Sporządź krótkie sprawozdanie które będzie opisywać jak należy korzystać z programu, przedstaw algorytm i krótkie podsumowanie.
*/

#include <iostream>
#include <vector>
#include <stack>
#include <string>

using namespace std;

class Calculator {
public:
    vector<string> parse(const string& input)
    {
        vector<string> vec;
        string current;
        bool lastWasDot = false, firstIsNegative = false;

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
            }

            else if (c)
            {
                if (!current.empty())
                {
                    vec.emplace_back(move(current));
                    current = "";
                }
                if (c != ' ')
                    vec.emplace_back(1, c);
            }
        }
        if (!current.empty())
            vec.push_back(move(current));
        return vec;
    }

    double perform(vector<string> notation)
    {
        if (notation.size() < 2)
            throw string("Niewystarczajaca liczba danych!");

        else if (!isNumber(notation[0]))
            throw string("Pierwszy argument musi być liczba!");
        
        else if (notation[1] == "!") {
            if (notation.size() == 2)
                return calculateOneArg(notation);
            else
                throw string("Niepoprawna ilosc lub wartosc argumentow dla tego dzialania!");
        }
        
        else if (notation[1] == "^" || notation[1] == "V" || notation[1] == "%") {
            if (notation.size() == 3 && isNumber(notation[2]))
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
            "|| Oblicza rowniez inne dzilania takie jak:                                    ||\n"
            "|| pierwiastek(^), potega(V), silnia(!) oraz procent(%)                        ||\n"
            "|| Jednak w tym przypadku musimy stosowac sie do kilku wytycznych:             ||\n"
            "|| Potegowanie: Podstawa potegi ^ Wykladnik potegi. Np: 3^2                    ||\n"
            "|| Pierwiastowanie: Stopien pierwiastka V Liczba Pierwiastkowana. Np: 2V9      ||\n"
            "|| Silnia: Liczba!. Np: 5!                                                     ||\n"
            "|| Procent: Procent % z liczby. Np. 10%100                                     ||\n"
            "++=============================================================================++" << endl << endl;
    }

private:
    double calculateOneArg(vector<string> notation)
    {
        string o = notation[1];
        double x = stod(notation[0]);
        char oper = o[0];

        switch (oper)
        {
        case '!':
            return strong(int(x));
        default:
            return 0.0;
        }
    }

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

    long long strong(double n)
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
                cout << "Wynik: " << result << endl;
                cout << "++=============================================================================++" << endl << endl;
            }
            catch (string& exc) {
                cout << exc << endl;
                cout << "++=============================================================================++" << endl << endl;
            }  
        }
        else {   
            cout << "Jesli chcesz zakończyć, nacisnij krzyzyk. Jeśli potrzebujesz pomocy wpisz: help" << endl;
            cout << "++=============================================================================++" << endl << endl;
        }
    }   
}
