#pragma once
#include <pqxx/pqxx>
#include <iostream>
#include "../helpfoo/hfoo.h"


namespace psql_prepare{

using temp_tr = pqxx::transaction<pqxx::isolation_level::read_committed,pqxx::write_policy::read_write>;
/**
 * @file Here are parsing your sql.conf document,
 * creating line of your SQL-config parameters with and without
 * concrete name of your DB, also returns you configs as
 * unordered_map<string, string>&
 */

/*
------------------- CONNECTION ----------------------------
*/
/**
 * @brief ConnetionParameters
 */
const std::unordered_map<std::string, std::string> &GetConnectionParametrs();

/**
 * @brief Line to connect to sql without connection to concrete DB
 */
const std::string &GetParamLineToBuildConnetion();

/**
 * @brief Line to connect to sql with connection to concrete DB
 */
const std::string &GetParamLineToBuildConnetionWithDBName();

/*
---------------- CREATORS OF TABLES AND DATABASES -------------------
*/

/**
 * @brief Line - request to SQL with it's help we can check that main table is in table
 */
std::string GetRequestToCheckMainDBInTable();

/**
 * @brief Line - which will create Main Database
 */
std::string GetReqestToCreateDB();

/**
 * @brief Line - which will create Table of categories
 */
std::string GetReqestToCreateTableCategories();


/*
@brief Request to create table "Word - Word - Category"
*/
template <typename T>
std::string GetReqestToCreateTableTranslate(const T &value)
{
    std::ostringstream oss;
    oss << "CREATE TABLE IF NOT EXISTS " << value << " (source_id text NOT NULL ,"
                                                     " translate_id text NOT NULL,"
                                                     " category_id integer NOT NULL);";
    return oss.str();
};

/*
---------------------- INSERTERS TO TABLES ------------------------------
*/
/**
 * @brief Prepares request to insert to category one value 
 */
std::string PrepareInsertToCategoryOneValue();
/** 
 * @brief Prepares request to insert Word-Translate-ID into the needed table  
 */
template <typename T>
std::string  PrepareInsertToTranslateTable(const T &tablename)
{
    std::ostringstream oss;
    oss << "INSERT INTO " << tablename << "(source_id , translate_id , category_id )" << 
     R"(
      SELECT $1, $2, $3
      WHERE NOT EXISTS (
      SELECT 1 FROM )" << tablename 
      << R"( 
      WHERE source_id = $1 AND translate_id = $2  AND category_id = $3 )
      ON CONFLICT DO NOTHING;
      )";
      return oss.str();
};


/**
 * @brief Insert to categories category
 */
template <typename T>
std::string  GetRequestInsertToCategories(const T &category)
{
   std::ostringstream oss;
    oss << "INSERT INTO categories (category_name) "
        << "SELECT '" << category << "' "
        << "WHERE NOT EXISTS (SELECT 1 FROM categories WHERE category_name = '" << category << "') "
        << "ON CONFLICT DO NOTHING;";
      return oss.str();
};
std::string  GetRequestInsertToCategories(const std::wstring &category);



/**
 * @brief Delete from categories category
 */
template <typename T>
std::string  GetRequestDeleteFromCategories(const T &category)
{
    std::ostringstream oss;
    oss << "DELETE FROM categories WHERE category_name = '" << category << "';";
      return oss.str();
};

/*
-------------------- READ REQUESTS ----------------------------------------
*/

/** 
*  @brief Get request to select all from needed category
*/
template <typename T>
std::string GetRequestToSelectAll(const T &value)
{
    std::ostringstream oss;
    oss << "SELECT * FROM " << value << ';' ;
    return oss.str();
}

/** 
*  @brief Prepare request to get ID of category by its name
*/
std::string PrepareSelectIdCatecoriesByName();
/** 
*  @brief Prepare request to get name of category by its ID
*/
std::string PrepareSelectNameCatecoriesById();

/** 
*  @brief Prepare request to get ID category by its name
*  @param value name of category
*/
template <typename T>
std::string GetSelectIdCatecoriesByName(const T &value){
     std::ostringstream oss;
     oss << "SELECT (id) FROM categories WHERE category_name = '" 
     << value <<"';"; 
     return oss.str();
};

/**
 * @brief Prepares request to select all words by needed category
 * @param id Category which words are needed.
 */
std::string  GetRequestSelectFromTrBaseTablebyId(int id);

/// @brief Delete lang1 - lang2 from selected cateory request
/// @param couple lang1 word - lang2 word
/// @param cat descriptoin of category
/// @return Delete lang1 - lang2 from selected cateory request
std::string GetReruestToDeleteCouple(const std::vector<std::wstring>& couple, const std::pair<int, std::wstring> &cat);

};



