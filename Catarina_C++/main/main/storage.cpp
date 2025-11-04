#include "storage.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>

using namespace std;

// Esta funcao faz uma cifra mais forte que a de Cesar:
// cada letra descola com base + o indice da letra na string.
// Para ler, usamos o mesmo mas com shiftBase negativo.
string progressiveCipher(const string& s, int shiftBase) {
    string out = s;
    for (size_t i = 0; i < out.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(out[i]);
        if (isalpha(ch)) { // so altera letras
            bool up = isupper(ch);
            char base = up ? 'A' : 'a';
            int desloc = shiftBase + static_cast<int>(i); // aumenta com o index
            // coloca no alfabeto outra vez (mod 26)
            out[i] = static_cast<char>(base + ((ch - base + desloc + 26 * 100) % 26));
        }
    }
    return out;
}

// grava todos os colaboradores e marcacoes num ficheiro texto
void save_all(const string& path, const vector<Employee>& list) {
    ofstream ofs(path);
    if (!ofs) { cerr << "Erro: file write.\n"; return; }

    for (const auto& e : list) {
        // grava nome cifrado
        ofs << progressiveCipher(e.fullName, 1) << "\n";

        // grava marcacoes no formato YYYY-MM-DD|X
        for (const auto& p : e.marks)
            ofs << p.first << "|" << p.second << "\n";

        // *** separa colaboradores
        ofs << "***\n";
    }
}

// le o ficheiro texto para dentro do programa
bool load_all(const string& path, vector<Employee>& list) {
    ifstream ifs(path);
    if (!ifs) return false;

    string line;
    Employee cur;
    bool hasName = false;

    while (getline(ifs, line)) {

        // *** separador: indica fim de um colaborador
        if (line == "***") {
            if (hasName) list.push_back(cur);
            cur = Employee{};
            hasName = false;
        }

        // linha sem '|' = nome cifrado
        else if (line.find('|') == string::npos) {
            cur.fullName = progressiveCipher(line, -1); // descifra nome
            hasName = true;
        }

        // linha com '|' = marcacao de um dia
        else {
            size_t p = line.find('|');
            string d = line.substr(0, p);
            char t = line[p + 1];
            cur.marks[d] = t;
        }
    }

    if (hasName) list.push_back(cur);
    return true;
}

// mostra o calendario mensal de um colaborador
void show_month(const Employee& e, int month, int year) {
    cout << "\n[" << e.fullName << "] " << month << "/" << year << "\n";
    cout << "Su Mo Tu We Th Fr Sa\n";

    // calcular em que dia da semana o mes comeca
    tm first = { 0,0,0,1,month - 1,year - 1900 };
    mktime(&first);
    int start = first.tm_wday;

    // dias em cada mes
    int mdays[] = { 31, (leapYear(year) ? 29 : 28),31,30,31,30,31,31,30,31,30,31 };
    int days = mdays[month - 1];
    int col = 0;

    // imprimir espacos ate ao primeiro dia
    for (int i = 0; i < start; i++) { cout << "   "; col++; }

    // imprimir dias do mes com marcacoes
    for (int d = 1; d <= days; ++d) {

        // chave YYYY-MM-DD
        string key = to_string(year) + "-" + (month < 10 ? "0" : "") + to_string(month)
            + "-" + (d < 10 ? "0" : "") + to_string(d);

        char mark = ' ';
        auto it = e.marks.find(key);
        if (it != e.marks.end()) mark = it->second; // se marcado, mostra

        tm t = { 0,0,0,d,month - 1,year - 1900 }; mktime(&t);
        bool wk = (t.tm_wday == 0 || t.tm_wday == 6); // fim de semana?

        // imprime com * ou com simbolo se ferias/falta
        if (wk) {
            if (mark == ' ') cout << setw(2) << d << "* ";
            else             cout << setw(2) << d << mark << " ";
        }
        else {
            if (mark == ' ') cout << setw(2) << d << "  ";
            else             cout << setw(2) << d << mark << " ";
        }

        col++;
        if (col % 7 == 0) cout << "\n";
    }

    cout << "\nNote: * weekend. F=ferias, X=falta\n";
}
