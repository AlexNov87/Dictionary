#include "client.h"

namespace client
{

    void GameClient::ShowCategiries()
    {
        tcp::socket sock_{ioc_};
        sock_.connect(epoint_);

        auto categories = GetCategoriesFromServer(sock_);
        if (!categories)
        {
            hf::WPrintSynchro(std::wcerr, "Server sent empty categories");
            throw std::runtime_error("Server sent empty categories \n");
        }

        int cnt = 1;
        for (auto &&ctg : *categories)
        {
            std::wcout << ctg << " ";
            ++cnt;
            if (cnt % 5 == 0)
            {
                std::wcout << "\n";
            }
        };
    }

    void GameClient::UploadAllCategories()
    {
        tcp::socket sock_{ioc_};
        sock_.connect(epoint_);

        auto categories = GetCategoriesFromServer(sock_);
        if (!categories)
        {
            hf::WPrintSynchro(std::wcerr, "Server sent empty categories");
            throw std::runtime_error("Server sent empty categories \n");
        }

        auto dict = GetDictionaryCategories(sock_, categories);
        if (!dict)
        {
            hf::WPrintSynchro(std::wcerr, "Something in dictionary");
            throw std::runtime_error("Something in dictionary \n");
        }
        dictfwd = dict;
        BuildBWDDictionary();
    }

    void GameClient::UploadNeededCategories()
    {
        try
        {
            tcp::socket sock_{ioc_};
            sock_.connect(epoint_);

            auto categories = GetCategoriesFromServer(sock_);
            if (!categories)
            {
                hf::WPrintSynchro(std::wcerr, "Server sent empty categories");
                throw std::runtime_error("Server sent empty categories \n");
            }
            auto matched_categories = GetMatchedCategories(categories);
            if (!matched_categories)
            {
                hf::WPrintSynchro(std::wcerr, "Something in matched categories");
                throw std::runtime_error("Something in matched categories \n");
            }
            auto dict = GetDictionaryCategories(sock_, matched_categories);
            if (!dict)
            {
                hf::WPrintSynchro(std::wcerr, "Something in dictionary");
                throw std::runtime_error("Something in dictionary \n");
            }
            dictfwd = dict;
            BuildBWDDictionary();
        }
        catch (const std::exception &ex)
        {
            std::wcout << "Exception upload needed categories: " << hf::ToWstr(ex.what()) << '\n';
        }
    }

    void GameClient::BuildBWDDictionary()
    {
        if (dictfwd == nullptr)
        {
            hf::WPrintSynchro(std::wcerr, "First dictiomary is not ready");
            abort();
        }

        dictbwd = std::make_shared<CategoriedDictionary>();
        CategoriedDictionary d;
        for (auto &&cat : *dictfwd)
        {
            for (auto &&word : cat.second)
            {
                for (auto &&synonim : word.second)
                {
                    (*dictbwd)[cat.first][synonim].insert(word.first);
                }
            }
        }
    }

    std::shared_ptr<CategoriedDictionary> GameClient::GetDictionaryCategories(tcp::socket &sock,
                                                                              std::shared_ptr<std::unordered_set<std::wstring>> categories)
    {

        http::request<req_body> requset_needed = RequestToGetNeededCategories(epoint_.address().to_string(), *categories, "CLIENT_BROTHER");
        http::write(sock, requset_needed);

        beast::flat_buffer buffer;
        boost::system::error_code ec;

        http::response<req_body> responce;
        http::read(sock, buffer, responce, ec);
        if (ec)
        {
            throw std::runtime_error(ec.message());
        }

        std::wstringstream wss(hf::ToWstr(responce.body()));
        boost::archive::text_wiarchive wiarch{wss};
        CategoriedDictionary dict;
        wiarch >> dict;

        if (dict.empty())
        {
            return nullptr;
        }
        return std::make_shared<CategoriedDictionary>(std::move(dict));
    }

    std::shared_ptr<std::unordered_set<std::wstring>> GameClient::GetMatchedCategories(std::shared_ptr<std::unordered_set<std::wstring>> categories)
    {
        try
        {
            std::map<int, std::wstring> cat_numetator;
            int ord = 1;
            for (auto &ctg : *categories)
            {
                cat_numetator[ord++] = std::move(ctg);
            }
            auto matched_categoties = loader::GetCategoryIndexes(cat_numetator);
            if (matched_categoties.empty())
            {
                return nullptr;
            };

            categories->clear();

            for (int ind : matched_categoties)
            {
                categories->insert(std::move(cat_numetator.at(ind)));
            }
            return categories;
        }
        catch (const std::exception &ex)
        {
            std::wcout << hf::ToWstr(ex.what()) << '\n';
            throw std::runtime_error(ex.what());
        }
    }

    std::shared_ptr<std::unordered_set<std::wstring>> GameClient::GetCategoriesFromServer(tcp::socket &sock)
    {

        http::request<req_body> requset_show = RequestToShowCategories(epoint_.address().to_string(), "CLIENT_BROTHER");
        http::write(sock, requset_show);

        beast::flat_buffer buffer;
        boost::system::error_code ec;

        http::response<req_body> response_show;
        http::read(sock, buffer, response_show, ec);

        if (ec)
        {
            throw std::runtime_error(ec.message());
        }

        std::wstringstream wss(hf::ToWstr(response_show.body()));
        boost::archive::text_wiarchive wiarch{wss};
        std::unordered_set<std::wstring> categories;
        wiarch >> categories;
        if (categories.empty())
        {
            return nullptr;
        }
        return std::make_shared<std::unordered_set<std::wstring>>(std::move(categories));
    }

}

namespace client
{

    void GameClientWin::LoadDictionary()
    {

        std::wcout << L"НАЖМИТЕ:\n1.ДЛЯ ЗАГРУЗКИ ПОЛНОГО СЛОВАРЯ.\n2.ДЛЯ ЗАГРУЗКИ ОТДЕЛЬНЫХ КАТЕГОРИЙ.\n";
        while (true)
        {
            if (GetAsyncKeyState('1') & 0x8000)
            {
                system("cls");
                UploadAllCategories();
                break;
            }
            if (GetAsyncKeyState('2') & 0x8000)
            {
                system("cls");
                UploadNeededCategories();
                break;
            }
        }
    };

}