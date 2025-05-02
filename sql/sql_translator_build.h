#pragma once
#include "../loader/loader.h"
#include "do_connection_str.h"

namespace psql {
/**
 * @brief Prepares dictionary by result from DB
 */
Dictionary DictForward(const pqxx::result& res);
Dictionary DictBackward(const pqxx::result& res);

/** 
 * @brief Creates category if not this category not exists and uploads
 * all words by this category
 * @param category name of category and its words  
*/
void UploadTranslatesSource(const std::pair<std::wstring, Dictionary> &category);
/**
 * @brief Returns Id Category from DB by its name
 * @param conn connection
 * @param name name of category 
 */
int IdCategory(pqxx::connection &conn, const std::string &name);
/**
 * @brief Returns from DB all categories wiht its id's 
 */
std::map<int, std::wstring> AllCategories();
/**
 * @brief Deletes category and all words in Trtable which have this category
 * @param cat category name which we need to destroy  
 */
void DestroyCategory(const std::string& cat);

}