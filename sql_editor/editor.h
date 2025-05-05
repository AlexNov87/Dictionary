#pragma once
#include "../loader/loader.h"
#include "../sql/sql_translator_build.h"
#include "../loader/mthread.h"

namespace sql_editor
{
    class LoadToSQLClient
    {
    private:
        /// @brief checks that category is valid
        /// @param tmp id of category in string representation
        /// @param categories list of categories id-name
        /// @return if category is matched returns its id 
        std::optional<int> MatchCategory(const std::wstring &tmp, const std::map<int, std::wstring> &categories);
        /// @brief constructs list of categories you need. Only matched 
        /// @param categories list of all categories
        /// @return list of id of matched categories
        std::unordered_set<int> MatchedCategories(const std::map<int, std::wstring> &categories);
        
        void PrintCategoriesWhichAreInDB(const std::map<int, std::wstring> categories);
        bool ShowMenu();
        
        /// @brief Select one category (maybe to modify)
        /// @return parameters of category
        std::optional<std::pair<int,std::wstring>> SelectCategory();

    public:
        void Menu();

    private:
        /// @brief Delete list of words in category
        void DeleteWordsFromCategory();
        /// @brief Deletes a couple of lang1-lang2 in category you choose 
        void DeleteCoupleFromCategory();
        /// @brief Destroy category
        void DestroyCategories();
        loader::LanguageStorageSaveToSQL storage;
    };

}