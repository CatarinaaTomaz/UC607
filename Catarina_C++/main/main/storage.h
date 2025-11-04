#pragma once
#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <string>
#include "employee.h"

// grava todos os colaboradores num ficheiro TXT
// cada colaborador fica separado com "***"
void save_all(const std::string& path, const std::vector<Employee>& list);

// carrega todos os colaboradores do ficheiro TXT
// devolve true se conseguiu ler, false se o ficheiro nao existir
bool load_all(const std::string& path, std::vector<Employee>& list);

// cifra o nome do colaborador
// cada letra desloca usando a formula (shiftBase + index)
std::string progressiveCipher(const std::string& s, int shiftBase);

// mostra o calendario mensal com marcacoes
// imprime dias, ferias, faltas e * nos fins de semana
void show_month(const Employee& e, int month, int year);

#endif
