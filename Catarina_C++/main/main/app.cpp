#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "employee.h"
#include "storage.h"

using namespace std;

int main() {

    // vetor onde guardamos os colaboradores em memoria
    vector<Employee> team;

    // nome do ficheiro onde vamos guardar/carregar
    const string store = "catarina_storage.txt";

    // tenta carregar do ficheiro
    if (load_all(store, team))
        cout << "Base de dados carregada (" << team.size() << " colaboradores)\n";
    else
        cout << "Nenhuma base existente. A iniciar nova...\n";

    int opc = -1; // variavel para controlar opcao do menu

    // ciclo principal do menu
    while (opc != 0) {

        // menu principal
        cout << "\n=== Gestao RH - Catarina ===\n";
        cout << "O que pretende fazer?\n";
        cout << "[1] Inserir novo colaborador\n";
        cout << "[2] Mostrar todos os colaboradores\n";
        cout << "[3] Registar FERIAS\n";
        cout << "[4] Registar FALTA\n";
        cout << "[5] Remover marcacao num dia\n";
        cout << "[6] Consultar calendario mensal\n";
        cout << "[0] Sair e guardar\n";
        cout << "Opcao: ";

        // valida leitura de numero
        if (!(cin >> opc)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // 1) adicionar colaborador
        if (opc == 1) {
            Employee e;
            cout << "Nome do colaborador: ";
            getline(cin, e.fullName);

            // verifica se ja existe
            auto it = find_if(team.begin(), team.end(),
                [&](const Employee& x) { return x.fullName == e.fullName; });

            if (it != team.end()) {
                cout << "Ja existe esse nome. Substituir? (s/n): ";
                char r; cin >> r; cin.ignore();
                if (r == 's' || r == 'S') *it = e; // substitui
            }
            else team.push_back(e);
        }

        // 2) listar colaboradores
        else if (opc == 2) {
            cout << "\nColaboradores registados:\n";
            for (auto& p : team) cout << " - " << p.fullName << "\n";
        }

        // 3,4,5) marcar e desmarcar
        else if (opc >= 3 && opc <= 5) {

            // pedir nome
            cout << "Nome do colaborador: ";
            string name; getline(cin, name);

            // procurar
            auto it = find_if(team.begin(), team.end(),
                [&](const Employee& x) { return x.fullName == name; });

            if (it == team.end()) { cout << "Colaborador nao existe.\n"; continue; }

            // pedir data
            int d, m, y;
            cout << "Dia Mes Ano (ex: 12 5 2025): ";
            if (!(cin >> d >> m >> y)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // valida data
            if (!date_ok(d, m, y)) { cout << "Data invalida.\n"; continue; }

            // nao deixar marcar fim de semana
            if (weekend(d, m, y)) { cout << "Nao e permitido marcar ao fim de semana.\n"; continue; }

            // formata data YYYY-MM-DD
            string key = to_string(y) + "-" + (m < 10 ? "0" : "") + to_string(m)
                + "-" + (d < 10 ? "0" : "") + to_string(d);

            // 5) desmarcar
            if (opc == 5) {
                it->marks.erase(key);
                cout << "Marcacao removida.\n";
            }
            else {
                // marcar F ou X
                char t = (opc == 3) ? 'F' : 'X';
                it->marks[key] = t;
                cout << "Marcado '" << t << "' no dia " << key << ".\n";
            }
        }

        // 6) imprimir calendario
        else if (opc == 6) {
            cout << "Nome do colaborador: ";
            string name; getline(cin, name);
            auto it = find_if(team.begin(), team.end(),
                [&](const Employee& x) { return x.fullName == name; });

            if (it == team.end()) { cout << "Colaborador nao encontrado.\n"; continue; }

            // pede mes e ano
            int m, y;
            cout << "Indique mes e ano (ex: 5 2025): ";
            if (!(cin >> m >> y)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // chama funcao que mostra calendario
            show_month(*it, m, y);
        }

        // opcao invalida
        else if (opc != 0) {
            cout << "Opcao invalida.\n";
        }
    }

    // guardar dados ao sair
    save_all(store, team);
    cout << "Dados guardados em '" << store << "'\n";
    return 0;
}
