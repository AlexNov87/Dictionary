#include "loader.h"

namespace loader{
/** @brief Name of folder where are located dictionary categoriies */
const std::string folder_to_play = "categories";
const std::string folder_to_sql_load = "categories_sql";
using namespace std;
using namespace filesystem;
namespace ba = boost::algorithm;

/** @brief Checks line for possibility to make
 *  collection of (int) from line
 *  @param line line which would be checked
 *  @returns If line contains only digits or gaps , it is correct line
 */
bool CorrectLine(const wstring &line)
{
    for (char c : line)
    {
        if (!isdigit(c) && c != ' ')
        {
            return false;
        }
    }
    return true;
}

/** @brief Splits line of digits and gaps.
 *  @param line Line, which we may be will parse.
 *  @returns Collection of unique numbers or std::nullopt
 * */
optional<unordered_set<int>> SplitLineNumbers(const wstring &line)
{
    if (line.empty())
    {
        return nullopt;
    }
    if (!CorrectLine(line))
    {
        return nullopt;
    }
    auto splitted = hf::SplitBySymbolSet(line, ' ');
    unordered_set<int> positions;
    for (auto &&str : splitted)
    {
        positions.insert(stoi(str));
    }
    return positions;
}


unordered_set<int> GetCategoryIndexes(const std::map<int, wstring> &cat_numerator)
{
    for (auto &&i : cat_numerator)
    {
        wcout << i.first << " " << i.second << endl;
    }

    wstring line;
    optional<unordered_set<int>> collection_numbers;

    auto it = cat_numerator.begin();
    advance(it, cat_numerator.size() - 1);
    int max_category_number = (*it).first;

    while (!collection_numbers)
    {
        wcout << L"ВВЕДИТЕ ЧЕРЕЗ ПРОБЕЛ НОМЕРА НУЖНЫХ ВАМ КАТЕГОРИЙ ИЛИ -ALL ЧТОБЫ ДОБАВИТЬ ВСЕ" << endl;
        getline(wcin, line);
        ba::trim(line);
        if(line == L"-ALL"){
          unordered_set<int> all;
            for(auto && pair : cat_numerator ) {
                all.insert(pair.first);
            }
          return all;
        }
        collection_numbers = SplitLineNumbers(line);
    };

    unordered_set<int> &matched = *collection_numbers;
    for (int cat : matched)
    {
        if (!cat_numerator.count(cat))
        {
            matched.erase(cat);
        }
    }
    return *collection_numbers;
}

/** @brief
 *  @returns Pairs of dictionaries from files
 */
pair<unordered_map<wstring, Dictionary>, unordered_map<wstring, Dictionary>> GetLangPairsFromTexts(std::string folder)
{
    path way = filesystem::current_path() / folder;
    int count = 0;
    vector<path> all_pathes;
    std::map<int, wstring> cat_numerator;
    
    for (auto &&w : directory_iterator(way))
    {
        // If file has .txt extension, it is correct
        if (w.path().extension() != ".txt")
        {
            continue;
        }
        all_pathes.push_back(w.path());
        string file_name = hf::GetFilename(w.path());
        cat_numerator[count++] = hf::ToWstr(file_name);
    }

    if (cat_numerator.empty())
    {
        hf::WPrintSynchro(err_stream::stream, L"THE FOLDER ", 
        hf::ToWstr(way.string()) , L" WHERE FROM YOU WANT LOAD FILES IS EMPTY");
        system("pause");
        return {};
    }
    // Recieving valid needed indexes of ways collection
    auto nums = GetCategoryIndexes(cat_numerator);
    // Building Dictionary
    return BuildDictionaryMthread(all_pathes, nums);
};

void LanguageStorage::Print() const
{
    wcout << *dictfwd << endl
          << *dictbwd << endl;
};

void LanguageStorageFile::LoadDictionary()
{
    auto dics = GetLangPairsFromTexts(folder_to_play);
    this->dictfwd = std::make_shared<CategoriedDictionary>(move(dics.first));
    this->dictbwd = std::make_shared<CategoriedDictionary>(move(dics.second));
};

void LanguageStorageSql::LoadDictionary()
{
    auto dics = BuildDictionaryMthreadSQL();
    this->dictfwd = std::make_shared<CategoriedDictionary>(move(dics.first));
    this->dictbwd = std::make_shared<CategoriedDictionary>(move(dics.second));
};

//LanguageStorageSaveToSQL
void LanguageStorageSaveToSQL::LoadDictionary()
{
    auto dics = GetLangPairsFromTexts(folder_to_sql_load);
    this->dictfwd = std::make_shared<CategoriedDictionary>(move(dics.first));
    this->dictbwd = std::make_shared<CategoriedDictionary>(move(dics.second));
};


}