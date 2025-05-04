#pragma once
#include "../loader/loader.h"
#include "../sql/sql_translator_build.h"
#include "../loader/mthread.h"

namespace sql_editor
{
    class LoadToSQLClient
    {
    private:
        std::optional<int> MatchCategory(const std::wstring &tmp, const std::map<int, std::wstring> &categories);
        std::unordered_set<int> MatchedCategories(const std::map<int, std::wstring> &categories);
        void PrintCategoriesWhichAreInDB(const std::map<int, std::wstring> categories);
        bool ShowMenu();
        std::optional<std::pair<int,std::wstring>> SelectCategory();

    public:
        void Menu();

    private:
        void DeleteWordsFromCategory();
        void DeleteCoupleFromCategory();
        void DestroyCategories();

        loader::LanguageStorageSaveToSQL storage;
    };

}