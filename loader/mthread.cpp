#include "mthread.h"
#include "../sql/sql_translator_build.h"

namespace loader{
using namespace std;
using namespace std::filesystem;
namespace ba = boost::algorithm;
 
/// @brief Adds category by path. Filename well be name of category.
/// @param pth path to file
/// @param mtx mutex to synchonics add to categories
/// @param forward CategoriedDictionary L1-L2
/// @param backward CategoriedDictionary L2-L1
void addCategory(const path &pth, mutex &mtx,
                 CategoriedDictionary &forward,
                 CategoriedDictionary &backward)
{
    string category_string = hf::GetFilename(pth);
    wstring category(hf::ToWstr(category_string));

    Dictionary fw;
    Dictionary bw;
    wifstream ifs(pth);
    while (ifs)
    {
        wstring tmp;
        getline(ifs, tmp);
        ba::trim(tmp);
       
       for(int i = 0; i < tmp.size(); ++i){
           tmp[i] = tolower(tmp[i]);
       }

        auto wordsvc = hf::SplitBySymbolSet(tmp, '|');
        if (wordsvc.size() != 2)
        {
            continue;
        }
        auto it = wordsvc.begin();
        auto lng1 = hf::SplitBySymbolSet(*it, ',');
        advance(it, 1);
        auto lng2 = hf::SplitBySymbolSet(*it, ',');

        for (auto &&l1 : lng1)
        {
            for (auto &&l2 : lng2)
            {
                fw[l2].insert(l1);
                bw[l1].insert(l2);
            }
        }
    }
    lock_guard<mutex> lg(mtx);
     forward[category] = std::move(fw);
     backward[category] = std::move(bw);
};

/// @brief Builds Categoried dictionary from file
/// @param pathes Pathes which are in folder
/// @param indexes Indexes which you chose to add.
/// @return Pair of CategoriedDictionary
pair<CategoriedDictionary, CategoriedDictionary>
BuildDictionaryMthread(const vector<filesystem::path> &pathes, const unordered_set<int> &indexes)
{
    mutex mtx;
    CategoriedDictionary forward;
    CategoriedDictionary backward;

    vector<jthread> threads;
    for (int index : indexes)
    {
        threads.emplace_back(
            ref(addCategory),
            ref(pathes[index]),
            ref(mtx),
            ref(forward),
            ref(backward));
    }
    threads.clear();
    return {move(forward), move(backward)};
};

/// @brief Loads CategoriedDictionary to database
/// @param source dictionary, which must be loaded.
void LoadDictionaryToDB(const CategoriedDictionary &source)
{
    unordered_set<string_view> categories;
    vector<jthread> threads;
    for (const auto &pair : source)
    {
        threads.push_back(jthread(ref(psql::UploadTranslatesSource), pair));
    }
    threads.clear();
}


/// @brief Adds category to CategoriedDictionary when you are building it 
/// @param cat_id Id of category 
/// @param cat_name Name of category
/// @param mtx Mutex which will save your dictionaries of race-condition
/// @param forward Storage L1-L2 
/// @param backward Storage L2-L1
void AddCategorySql(
  int cat_id, const wstring& cat_name,  
  mutex &mtx, CategoriedDictionary &forward, 
  CategoriedDictionary &backward)
{
    //Connect to DB
    pqxx::connection conn{psql_prepare::GetParamLineToBuildConnetionWithDBName()};
    pqxx::read_transaction trz{conn};
    
    //Selecting all words by category
    auto res = trz.exec(psql_prepare::GetRequestSelectFromTrBaseTablebyId(cat_id));
    trz.commit();
    
    //Constructing dictionary by category
    future<Dictionary> fwd_async =  async(psql::DictForward, ref(res));
    future<Dictionary> bwd_async =  async(psql::DictBackward, ref(res));
        
    auto  fwd = fwd_async.get();
    auto  bwd = bwd_async.get();

    lock_guard<mutex> lg(mtx);
       forward[cat_name]  = move(fwd);
       backward[cat_name] = move(bwd);
}


/// @brief Builds ServerDictionary from SQL
/// @return Pair of dictionaries
pair<CategoriedDictionary, CategoriedDictionary> BuildDictionaryMthreadSQL()
{
    auto all_categories = psql::AllCategories();
    auto needed_categories = GetCategoryIndexes(all_categories);

    CategoriedDictionary forward;
    CategoriedDictionary backward;
    mutex mtx;
    vector<jthread> threads;

    for (int ct : needed_categories)
    {
     threads.emplace_back(AddCategorySql, ct, 
     ref(all_categories.at(ct)), ref(mtx), ref(forward), ref(backward)); 
    }
    threads.clear();
    return { move(forward), move(backward)};
}
}
