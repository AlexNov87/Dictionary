#include "editor.h"

namespace sql_editor{


    std::optional<int> LoadToSQLClient::MatchCategory(const std::wstring &tmp, const std::map<int, std::wstring> &categories)
        {
            size_t converted;
            int cat = -1;
            try
            {
                //if can't convert
                cat = std::stoi(tmp, &converted);
            }
            catch (const std::exception &ex)
            {
                std::wcout << tmp << " CAN NOT BE CATEGORY ID\n";
                system("pause");
                return std::nullopt;
            }

            if (converted != tmp.size())
            {
                //if can't convert to full size 
                std::wcout << tmp << " CAN NOT BE CATEGORY ID\n";
                system("pause");
                return std::nullopt;
            }
            else if (!categories.contains(cat))
            {
                //if category doesn't have value which is in string line
                std::wcout << "CATEGORY WITH ID " << tmp << " CAN NOT BE FOUND\n";
                system("pause");
                return std::nullopt;
            }
            return cat;
        }

        std::unordered_set<int> LoadToSQLClient::MatchedCategories(const std::map<int, std::wstring> &categories)
        {
            std::wstring tmp;
            //input lines of categories
            std::getline(std::wcin, tmp);
            //put categories lime into the stream 
            std::wstringstream wss(std::move(tmp));
            tmp.clear();
            //categories which must be in const std::map<int, std::wstring>
            std::unordered_set<int> ctgs;
            while (wss >> tmp)
            {
                //match category
                std::optional<int> cat = MatchCategory(tmp, categories);
                if (cat)
                {
                    ctgs.insert(cat.value());
                }
            }
            return ctgs;
        };

        void LoadToSQLClient::PrintCategoriesWhichAreInDB(const std::map<int, std::wstring> categories)
        {
            int cnt = 1;
            for (auto &&pair : categories)
            {
                std::wcout << pair.first << " -> " << pair.second << " | ";
                ++cnt;
                if (cnt % 5 == 0)
                {
                    std::wcout << '\n';
                }
            }
            std::wcout << '\n';
        }

        bool LoadToSQLClient::ShowMenu()
        {
            std::wcout << L"ENTER:\n"
                          "1.DELETE CATEGORIES\n"
                          "2.LOAD TO DATABASE FROM FILES WHICH ARE IN categories_sql\n"
                          "3.DELETE LIST OF WORDS FROM DB IN CATEGORY\n"
                          "4.DELETE ONE COUPLE OF WORDS WHERE FROM CATEGORY\n";
            return false;
        }

        std::optional<std::pair<int,std::wstring>> LoadToSQLClient::SelectCategory()
        {
            //get all categories, which are in base
            auto categories = psql::AllCategories();
            PrintCategoriesWhichAreInDB(categories);
            
            std::wcout << L"ENTER THE CATEGORY ID WHICH YOU WANT TO EDIT\n";
            //get matched gategories
            auto matched_cat = MatchedCategories(categories);
            //selected category must be only one
            if (matched_cat.size() != 1)
            {
                std::wcout << L"INCORRECT CATEGORY INPUT\n";
                return std::nullopt;
            }
            //begin is a first element  
            int cat_id = *matched_cat.begin();
            //category name
            std::wstring cat_name = categories.at(*matched_cat.begin());
            //full description of category
            std::pair<int, std::wstring> res{cat_id, std::move(cat_name)};
            return res;
        }

        void LoadToSQLClient::Menu()
        {
            bool show = true;
            while (true)
            {
                if (show)
                {
                    show = ShowMenu();
                }

                if (GetAsyncKeyState('1') & 0x8000)
                {
                    DestroyCategories();
                    show = true;
                    system("cls");
                };

                if (GetAsyncKeyState('2') & 0x8000)
                {
                    storage.LoadDictionary();
                    const auto &toload = *(storage.ForwardDictionary());
                    if (!toload.empty())
                    {
                        std::wcout << toload << '\n'
                                   << "WILL BE LOADED TO DB\n";

                        if (hf::Confirm())
                        {
                            loader::LoadDictionaryToDB(toload);
                            std::wcout << "ALL CATEGORIES ARE LOADED\n";
                            system("pause");
                        }
                    }
                    show = true;
                    system("cls");
                };

                if (GetAsyncKeyState('3') & 0x8000)
                {
                    DeleteWordsFromCategory();
                    show = true;
                    system("cls");
                };

                if (GetAsyncKeyState('4') & 0x8000)
                {
                    DeleteCoupleFromCategory();
                    show = true;
                    system("cls");
                };
               
                Sleep(10);
            }
        }
        
        void LoadToSQLClient::DeleteWordsFromCategory()
        {
            std::wcout << L"DELETING WORDS FROM CATEGORY\n";
            //select category
            auto category = SelectCategory();
            if (!category)
            {
                std::wcout << "THERE ARE NONE CATEGORIES YET\n";
                system("pause");
                return;
            }
            //get words which category contains
            auto words_result = psql::GetWordsFromTableById((*category).first);
            //turn pqxx::result into Dictionary   
            auto dict = psql::DictForward(words_result);
            std::wcout << dict;
            
            std::wcout << "ENTER WORDS WHICH YOU WANT TO DELETE\n";
            //type words, which you want to delete
            std::wstring wordline;
            std::getline(std::wcin, wordline);
            
            //split words to delete
            std::unordered_set<std::wstring> words_to_delete = hf::SplitBySymbolSet<std::wstring, wchar_t>(wordline, ' ');
            //delete words
            psql::DeleteWordsFromDB(words_to_delete , *category);
            std::wcout << "DELETE WORDS SUCESS\n";
            system("pause");
        }

        void LoadToSQLClient::DeleteCoupleFromCategory()
        {
            std::wcout << L"DELETING COUPLE WORDS FROM CATEGORY\n";
            //select category
            auto category = SelectCategory();
            if (!category)
            {
                std::wcout << "THERE ARE NONE CATEGORIES YET\n";
                system("pause");
                return;
            }
            //get words which category contains
            auto words_result = psql::GetWordsFromTableById((*category).first);
            //turn pqxx::result into Dictionary 
            auto dict = psql::DictForward(words_result);
            std::wcout << dict;
            
            std::wcout << "ENTER COUPLE WORDS WHICH YOU WANT TO DELETE\n";
            std::wstring wordline;
            std::getline(std::wcin, wordline);
            //split words to get a gouple
            auto couple = hf::SplitBySymbolVec<std::wstring, wchar_t>(wordline, ' ');
            
            if(couple.size() != 2) {
                std::wcout << "COUPLE OF WORDS MAY HAVE SIZE ONLY 2\n";
            }
            //delete couple
            psql::DeleteCoupleFromDB(couple, *category);
            std::wcout << L"DELETING COUPLE SUCERSS\n";
            system("pause");
        }

        void LoadToSQLClient::DestroyCategories()
        {
            //get all categories
            auto categories = psql::AllCategories();
            PrintCategoriesWhichAreInDB(categories);
            std::wcout << L"ENTER THE CATEGORIES ID WHICH YOU WANT TO DELETE (1 2 3 456 ...)\n";
            //select categories to delete
            auto matched_to_delete = MatchedCategories(categories);
            if (matched_to_delete.empty())
            {
                return;
            }
            for (int cat_to_delete : matched_to_delete)
            {
                //delete category
                psql::DestroyCategory(hf::WstrToStr(categories.at(cat_to_delete)));
            }
            std::wcout << "DELETE CATEGORIES SUCCESS\n";
            system("pause");
        };

}