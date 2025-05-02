#include "do_connection_str.h"
#include "create_base.h"
using namespace std;

pqxx::connection ConnectToDB(const std::string &argument)
{
    pqxx::connection conn{argument};
    return conn;
}

void CreateDataBaseIfNotExists()
{
    const string &connectline = psql_prepare::GetParamLineToBuildConnetion();
    pqxx::connection conn = ConnectToDB(connectline);
    pqxx::read_transaction work{conn};
    const string &req_to_check_main_db = psql_prepare::GetRequestToCheckMainDBInTable();
    auto res = work.exec(req_to_check_main_db);
    work.commit();
    if (res.size() != 0)
    {
        conn.close();
        return;
    }

    pqxx::nontransaction create_db{conn};
    const string &create_req = psql_prepare::GetReqestToCreateDB();
    create_db.exec(create_req);
    create_db.commit();
    conn.close();
}

void CreateTables(pqxx::connection &conn)
{
    pqxx::transaction create_tables{conn};
    create_tables.exec(psql_prepare::GetReqestToCreateTableCategories());
    create_tables.exec(psql_prepare::GetReqestToCreateTableTranslate(SQL::tr_base1));
    create_tables.commit();
}

void CreateDataBase()
{
    CreateDataBaseIfNotExists();
    const string &conn_to_database = psql_prepare::GetParamLineToBuildConnetionWithDBName();
    pqxx::connection conn = ConnectToDB(conn_to_database);
    CreateTables(conn);
    conn.close();
};