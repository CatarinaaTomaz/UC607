#pragma once
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <map>

// estrutura que representa um colaborador
struct Employee {
    std::string fullName;               // nome completo do colaborador

    // mapa onde guardamos marcacoes no formato:
    // key  = "YYYY-MM-DD"
    // value = 'F' (ferias) ou 'X' (falta)
    std::map<std::string, char> marks;
};

// verifica se o ano e bissexto (ano de 29 dias em fevereiro)
bool leapYear(int year);

// valida se uma data existe realmente (dia/mes/ano)
bool date_ok(int d, int m, int y);

// indica se aquela data calha num fim de semana
bool weekend(int d, int m, int y);

#endif
