#pragma once
#include <pqxx/pqxx>
void CreateDataBase();
pqxx::connection ConnectToDB(const std::string &argument);
//psql -h 127.0.0.1 -p 5432 -U postgres
