#include "do_connection_str.h"


namespace psql_prepare{

using namespace std;
namespace ba = boost::algorithm;
const unordered_set<string> SQL_VALUES
{SQL::PORT, SQL::SQL::TABLENAME, SQL::HOST, SQL::PASSWORD, SQL::USERNAME};
constexpr int SQL_PARAMETERS = 5;

/** @brief Check validity parameter
 *  @param line parameter
 *  @returns true if SQL_VALUE contains line, else false
 */
bool IsValidParameter(const std::string &line)
{
    return SQL_VALUES.contains(line);
}

/**
 * @brief Checks that the line str wi can convert to int;
 * @param str Line, which we will try conver to int
 * @returns Converted digit
 */
int CheckConvertiableToInt(const std::string &str)
{
    size_t converted;
    int digit = stoi(str, &converted);
    if (converted != str.size())
    {
       wcerr << L"CAN NOT CONVERT TO INT STRING " << hf::ToWstr(str);
       abort();
    }
    return digit;
}

/**
 * @brief Checks that the port is coccect;
 * @param prt line, which contains ip-member
 */
void CheckPort(const std::string &prt)
{
    int port = CheckConvertiableToInt(prt);
    if (port < 0 || port > 65535)
    {
        wcerr << L"PORT CAN NOT BE " << hf::ToWstr(prt);
        abort();
    }
}

/**
 * @brief Checks that the ip member is coccect;
 * @param ip_m line, which contains ip-member
 */
void CheckIPMember(const std::string &ip_m)
{
    int ip_member = CheckConvertiableToInt(ip_m);
    if (ip_member < 0 || ip_member > 255)
    {
        wcerr << L"IP MEMBER CAN NOT BE " << hf::ToWstr(ip_m); 
        abort();
    }
};
/**
 * @brief Checks the ip adress line for coccect symbols;
 * @param ip line, which contains ip-adress
 */
void CheckIpLine(const std::string &ip)
{
    for (char c : ip)
    {
        if (!isdigit(c) && c != '.')
        {
            wcerr << L"IP LINE IS INCORRECT";
            abort();
        }
    }
}

/**
 * @brief Checks the ip adress, where we want to connect;
 * @param host line, which contains ip-adress
 */
void CheckHost(const string &host)
{
    if (host == "localhost")
    {
        return;
    }
    CheckIpLine(host);
    auto ip = hf::SplitBySymbolVec(host, '.');
    if (ip.size() != 4)
    {
        wcerr << L"IP CAN NOT CONTAIN " << ip.size() << L" MEMBERS";
        abort();
    };
    for (auto &&member : ip)
    {
        CheckIPMember(member);
    }
}

/**
 * @brief Checks that line not contains ' ';
 */
void CheckThatNopGaps(const std::string &str)
{
    for (char c : str)
    {
        switch (c)
        {
        case ' ':
            wcerr << L"THE GAP ' ' DETECTED";
            abort();
            break;
        }
    }
}

/**
 * @brief Advanced Checker that configurator is correct;
 * @param conf Table key-Value
 */
void DetailCheckConfgigurator(const unordered_map<string, string> &conf)
{
    for (auto &&param : SQL_VALUES)
    {
        if (!conf.contains(param))
        {
            wcerr << L"PARAMETER '" <<  hf::ToWstr(param) << L"' WAS NOT FOUND";
        }
    }

    for (auto &&[param, value] : GetConnectionParametrs())
    {
        CheckThatNopGaps(value);
    }
    CheckHost(conf.at(SQL::HOST));
    CheckPort(conf.at(SQL::PORT));
}

/**
 * @brief Parameters which we will use to connect to sql
 * @param lines vector of lines, which parsed in stream
 * @returns Configurator of DB-connection parameters.
 */
unordered_map<string, string> ExecuteParameters(const vector<string> &lines)
{
    unordered_map<string, string> res;
    int line = 1;
    for (auto &&ln : lines)
    {
        auto vec = hf::SplitBySymbolVec(ln, '=');
        if (vec.size() != 2)
        {
            wcerr << "THERE ARE TOO MANY OR TOO LOW ARGUMENTS IN CONF FILE Line: " <<
            line;
            abort();
        }
        if (!IsValidParameter(vec[0]))
        {
            wcerr << L"THERE PARAMETER '" << hf::ToWstr(vec[0]) << L"' IS INVALID";
            abort();
        }
        res[vec[0]] = vec[1];
        if (res.size() > SQL_PARAMETERS)
        {
            wcerr << L"TOO MANY SQL PARAMETERS" ; 
            abort();
        }
        ++line;
    }
    return res;
}



/*
API
VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
*/
/*
-----------------------CONNECTION-----------------------
*/
const std::unordered_map<std::string, std::string> &GetConnectionParametrs()
{
    static unordered_map<string, string> connection_parametrs;
    if (!connection_parametrs.empty())
    {
        return connection_parametrs;
    }
    vector<string> read_parameters = hf::ReadFromFile("sql.conf");
    connection_parametrs = ExecuteParameters(read_parameters);
    DetailCheckConfgigurator(connection_parametrs);
    return connection_parametrs;
};


const string &GetParamLineToBuildConnetion()
{
    static string res;
    if (!res.empty())
    {
        return res;
    }
    ostringstream oss;
    oss << "host=" << GetConnectionParametrs().at(SQL::HOST) 
    << " port=" << GetConnectionParametrs().at(SQL::PORT) 
    << " user=" << GetConnectionParametrs().at(SQL::USERNAME) 
    << " password=" << GetConnectionParametrs().at(SQL::PASSWORD);
    res = move(oss.str());
    return res;
};

const string &GetParamLineToBuildConnetionWithDBName()
{
    static string res;
    if (!res.empty())
    {
        return res;
    }
    ostringstream oss;
    oss << GetParamLineToBuildConnetion() << " dbname=" << GetConnectionParametrs().at(SQL::TABLENAME); 
    res = move(oss.str());
    return res;
};

/*
-------------------CREATORS DB AND TABLES ---------------------------
*/

std::string GetRequestToCheckMainDBInTable()
{
    ostringstream oss;
    oss <<  "SELECT datname FROM pg_database WHERE datname = '" 
    << GetConnectionParametrs().at(SQL::TABLENAME) << "';";
    return oss.str();   
};

std::string GetReqestToCreateDB()
{
    ostringstream oss;
    oss << "CREATE DATABASE " << GetConnectionParametrs().at(SQL::TABLENAME) << ';';
    return oss.str();
};

std::string GetReqestToCreateTableCategories()
{
    return  "CREATE TABLE IF NOT EXISTS categories "
    "(id SERIAL PRIMARY KEY , category_name varchar(50) UNIQUE NOT NULL);";

};

/*
--------------------PREPARES-----------------------
*/

//--> readers
std::string PrepareSelectIdCatecoriesByName(){
     return "SELECT id FROM categories WHERE category_name = $1";
};

std::string PrepareSelectNameCatecoriesById(){
    return "SELECT (category_name) FROM categories WHERE id = $1";
};

//--> insertions
std::string PrepareInsertToCategoryOneValue(){
    return "INSERT INTO categories (category_name) VALUES ($1) ON CONFLICT DO NOTHING";
};

std::string  GetRequestSelectFromTrBaseTablebyId(int id)
{
    std::ostringstream oss;
    oss << "SELECT source_id  , translate_id FROM trbase1 WHERE"
    " category_id = " << id << ";";
      return oss.str();
};

std::string  GetRequestInsertToCategories(const std::wstring &category){
    return GetRequestInsertToCategories(hf::WstrToStr(category));
}

}


