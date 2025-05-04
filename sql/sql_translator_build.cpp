#include "sql_translator_build.h"
#include "create_base.h"

using namespace std;

namespace psql
{
    /**
     * @brief Loads category to DB
     * @param category Categorized Dictionary category
     * @param conn Connection to create a tranzaction
     */
    void LoadSource(const pair<wstring, Dictionary> &category,
                    pqxx::connection &conn)
    {
        string category_string(hf::WstrToStr(category.first));
        int id_cat = IdCategory(conn, category_string);
        pqxx::transaction trz{conn};

        for (auto &&word_source : category.second)
        {
            const auto &primary = word_source.first;
            string primary_string(hf::WstrToStr(primary));
            const auto &translates = word_source.second;

            for (auto &&translate : translates)
            {
                string translate_string(hf::WstrToStr(translate));
                trz.exec_prepared("to_lang1", primary_string, translate_string, id_cat);
            }
        }
        trz.commit();
    }

    int IdCategory(pqxx::connection &conn, const std::string &name)
    {
        pqxx::read_transaction trz{conn};
        auto res = trz.exec(psql_prepare::GetSelectIdCatecoriesByName(name));
        trz.commit();
        if (res.size() != 1)
        {
            wcerr << L"No Results In foo 'Id Category'";
            return -1;
        }
        int id;
        for (auto [idx] : res.iter<int>())
        {
            id = idx;
        }
        return id;
    }

    void DestroyCategory(const std::string &cat)
    {
        pqxx::connection conn{psql_prepare::GetParamLineToBuildConnetionWithDBName()};
        int cat_id = IdCategory(conn, cat);
        pqxx::transaction tr{conn};
        tr.exec("DELETE FROM categories * WHERE id = " + to_string(cat_id) + " ;");
        tr.exec("DELETE FROM trbase1 * WHERE category_id = " + to_string(cat_id) + " ;");
        tr.commit();
    };

    void UploadTranslatesSource(const pair<wstring, Dictionary> &category)
    {
        try
        {
            pqxx::connection conn{psql_prepare::GetParamLineToBuildConnetionWithDBName()};
            conn.prepare("to_lang1", psql_prepare::PrepareInsertToTranslateTable(SQL::tr_base1));
            pqxx::transaction trz{conn};
            trz.exec(psql_prepare::GetRequestInsertToCategories(category.first));
            trz.commit();
            LoadSource(category, conn);
        }
        catch (const std::exception &ex)
        {
            hf::WPrintSynchro(err_stream::stream, hf::ToWstr(ex.what()));
            abort();
        }
    }

    void DeleteWordsFromDB(const std::unordered_set<std::wstring> &words, const std::pair<int, std::wstring> &cat)
    {

        try
        {
            const std::string &connectline = psql_prepare::GetParamLineToBuildConnetionWithDBName();
            pqxx::connection conn = ConnectToDB(connectline);
            pqxx::transaction trz{conn};
            conn.prepare("delete_words", "DELETE FROM trbase1 WHERE (source_id = $1 OR translate_id = $1) AND category_id = $2");
            for (auto &&word : words)
            {
                trz.exec_prepared("delete_words", hf::WstrToStr(word), cat.first);
            }
            trz.commit();
        }
        catch (const std::exception &ex)
        {
            hf::WPrintSynchro(err_stream::stream, "DELETE REQUEST\n", hf::ToWstr(ex.what()));
            abort();
        }
    };

    map<int, wstring> AllCategories()
    {
        pqxx::connection conn{psql_prepare::GetParamLineToBuildConnetionWithDBName()};
        pqxx::read_transaction trz{conn};
        auto categories = trz.exec(psql_prepare::GetRequestToSelectAll("categories"));
        map<int, wstring> result;
        for (auto [id, cat] : categories.iter<int, string>())
        {
            result[id] = hf::ToWstr(cat);
        }
        return result;
    }

    Dictionary DictForward(const pqxx::result &res)
    {
        Dictionary dct;

        for (auto [source, translate] : res.iter<string, string>())
        {
            dct[hf::ToWstr(source)].insert(hf::ToWstr(translate));
        }
        return dct;
    }

    Dictionary DictBackward(const pqxx::result &res)
    {
        Dictionary dct;
        for (auto [source, translate] : res.iter<string, string>())
        {
            dct[hf::ToWstr(translate)].insert(hf::ToWstr(source));
        }
        return dct;
    }

    pqxx::result GetWordsFromTableById(int cat_id)
    {
        // Connect to DB
        pqxx::connection conn{psql_prepare::GetParamLineToBuildConnetionWithDBName()};
        pqxx::read_transaction trz{conn};

        // Selecting all words by category
        auto res = trz.exec(psql_prepare::GetRequestSelectFromTrBaseTablebyId(cat_id));
        trz.commit();
        return res;
    }

    void DeleteCoupleFromDB(const std::vector<std::wstring> &words, const std::pair<int, std::wstring> &cat)
    {
        try{
        pqxx::connection conn{psql_prepare::GetParamLineToBuildConnetionWithDBName()};
        pqxx::transaction trz{conn};
       
       
        trz.exec(psql_prepare::GetReruestToDeleteCouple(words, cat));
         trz.commit();
       }
       catch(const std::exception& ex){
           hf::WPrintSynchro(err_stream::stream, hf::ToWstr(ex.what()));
       };
        
    };

}