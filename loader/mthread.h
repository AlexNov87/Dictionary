#pragma once
#include "../helpfoo/definitions.h"
#include <mutex>
#include <future>
#include <thread>
#include <filesystem>
#include <map>
#include <vector>

namespace loader{
/**
 * @brief Builds dictionaries from txt-files 
 * @param pathes All pathes which found in categories
 * @param indexes Indexes of pathes, which we'll use to create dictionaries
 */
std::pair<CategoriedDictionary, CategoriedDictionary> BuildDictionaryMthread
(const std::vector<std::filesystem::path>& pathes, const std::unordered_set<int>& indexes);

/**
 * @brief Builds dictionaries from needed categories from SQL-base
 */
std::pair<CategoriedDictionary, CategoriedDictionary> BuildDictionaryMthreadSQL();

/**
 * @brief Builds dictionaries from needed categories from SQL-base
 */
std::unordered_set<int> GetCategoryIndexes(const std::map<int, std::wstring> &cat_numerator);


/// @brief Loads Dictionary to DB
/// @param sourse CategoriedDictionary which must be loaded to DB
void LoadDictionaryToDB(const CategoriedDictionary& sourse);


}
