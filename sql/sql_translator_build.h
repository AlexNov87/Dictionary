#pragma once
#include "../loader/loader.h"
#include "do_connection_str.h"
#include "../helpfoo/hfoo.h"

namespace psql
{
    /**
     * @brief Prepares dictionary by result from DB
     */
    Dictionary DictForward(const pqxx::result &res);
    Dictionary DictBackward(const pqxx::result &res);
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
    void DestroyCategory(const std::string &cat);

    /// @brief Returns Categories which are in SQL
    /// @return Categories which are in SQL
    std::map<int, std::wstring> AllCategories();

    /// @brief Deletes words from DB
    /// @param words Words which you want to delete
    /// @param cat Category parameters in which yo want to delete words
    void DeleteWordsFromDB(const std::unordered_set<std::wstring> &words, const std::pair<int, std::wstring> &cat);

    /// @brief Deletes couple of words from DB
    /// @param words Words which you want to delete
    /// @param cat Category parameters in which yo want to delete words
    void DeleteCoupleFromDB(const std::vector<std::wstring> &words, const std::pair<int, std::wstring> &cat);

    /// @brief Returns result (words and translates which are in category)
    /// @param cat_id Id category
    /// @return words and translates which are in category in pqxx::result format
    pqxx::result GetWordsFromTableById(int cat_id);

}