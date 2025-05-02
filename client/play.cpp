#include "client.h"

namespace client{
std::vector<GameClient::LangPair> GameClient::GetExersize(const CategoriedDictionary &dct) const
{
    static std::vector<LangPair> exersicewords;
    if (!exersicewords.empty())
    {
        std::shuffle(exersicewords.begin(), exersicewords.end(), *random_.GetGenerator());
        return exersicewords;
    };

    for (auto &&pair : dct)
    {
        for (auto &&word_with_synonims : pair.second)
        {
            GameClient::LangPair lp{&pair.first, &word_with_synonims};
            exersicewords.push_back(lp);
        }
    }

    std::shuffle(exersicewords.begin(), exersicewords.end(), *random_.GetGenerator());
    return exersicewords;
}

bool GameClient::PlayWord(const LangPair& lp) const
{
    const auto selected_word_pair = lp.word_pair;
    
    const auto &syn_storage = (*selected_word_pair).second;

    while (true)
    {
        system("cls");
        std::wcout << L"YOUR CATEGORY IS : " << *lp.category << std::endl;
        std::wcout << L"HOW TRANSLATES : " << (*selected_word_pair).first << std::endl
                   << L"TYPE -D TO MISS THIS WORD " << std::endl
                   << L"TYPE -E TO END THE GAME " << std::endl
                   << L"TYPE -S TO SHOW ANSWERS " << std::endl;

        std::wcout << L"TYPE YOUR TRANSLATION: _ " << std::endl;
        std::wstring tmp;
        std::getline(std::wcin, tmp);
        if (tmp == L"-E")
        {
            return false;
        }
        if (tmp == L"-D")
        {
            return true;
        }
        if (tmp == L"-S")
        {
            system("cls");
            std::wcout << (*selected_word_pair).first << " --> Translates: ";
            for (auto &&tr : syn_storage)
            {
                std::wcout << tr << std::endl;
            }
            system("pause");
            continue;
        }

        if (syn_storage.count(tmp))
        {
            break;
        }
    }
    std::wcout << "Great!!!!" << std::endl;
    Sleep(500);
    system("cls");
    return true;
}

void GameClient::Play(const std::vector<GameClient::LangPair> &dict) const
{
    for (auto lpr : dict)
    {
        
        if (!PlayWord(lpr))
        {
            return;
        }
    }
};

void GameClient::PlayForward() const
{
    Play(GetExersize(*dictfwd));
}

void GameClient::PlayBackward() const
{
    Play(GetExersize(*dictbwd));
}

///////////////////////////////////////////////////////////////////////////////////

void GameClientWin::Menu() const
{
    bool flag = false;
    while (true)
    {
        if (GetAsyncKeyState('1') & 0x8000)
        {
            PlayForward();
            system("cls");
            flag = false;
        }
        if (GetAsyncKeyState('2') & 0x8000)
        {
            PlayBackward();
            system("cls");
            flag = false;
        }
        if (GetAsyncKeyState('E') & 0x8000)
        {
            break;
        }
        if (!flag)
        {
            std::wcout << L"Нажмите клавишу: \n1) Для игры Язык1 - Язык2 \n"
                          "2) Для игры Язык2 - Язык1\n"
                          "E) Для выхода\n";
        }
        flag = true;
    }
}

}