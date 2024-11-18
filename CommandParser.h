#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include "CustVector.h"

using namespace std;

// Объявляем функцию для парсинга команды на токены
CustVector<string> parse_command(const string& command);

#endif