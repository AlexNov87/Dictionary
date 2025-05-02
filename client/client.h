#pragma once
#include "../loader/loader.h"
#include "request.h"
#include <algorithm>
#include <windows.h>

namespace client
{

     enum class FromServerMode
     {
          PARTIAL,
          ALL
     };

     class GameClient : public  loader::LanguageStorage
     {
     private:
          Rand random_;
          struct LangPair
          {
               const std::wstring *category;
               const std::pair<const std::wstring, std::unordered_set<std::wstring>> *word_pair;
          };

          asio::io_context ioc_;
          tcp::endpoint epoint_;

     public:
          virtual void Menu() const = 0;
          template <typename T>
          GameClient(const T &dict_adress, unsigned int port)
              : epoint_(tcp::endpoint{ip::make_address(dict_adress),
                                      boost::asio::ip::port_type(port)})
          {
          }

     protected:
          /**
           * @brief Play in Mode Lang1-Lang2
           */
          void PlayForward() const;
          /**
           * @brief Play in Mode Lang2-Lang1
           */
          void PlayBackward() const;

          /// @brief Shows categories
          void ShowCategiries();

          /// @brief Uloads All categories
          void UploadAllCategories();

          /// @brief Uploads categories which you choose
          void UploadNeededCategories();

     private:
          /**
           * @brief Builds vector, with which you'll play
           * @param dct Source, from which vector will be builded.
           */
          std::vector<LangPair> GetExersize(const CategoriedDictionary &dct) const;

          /**
           * @brief One word, which you must guess
           * @param Word with its translates
           */
          bool PlayWord(const LangPair &lp) const;

          /**
           * @brief Your nessesary game
           * @param dict Vector with which you'll play
           */
          void Play(const std::vector<LangPair> &dict) const;

     private:
          /// @brief Builds backward dictionary
          void BuildBWDDictionary();

          /// @brief Get categories from server which you need
          /// @param sock socket
          /// @param categories list of categories
          /// @return Ready Categoried dictionary which you need
          std::shared_ptr<CategoriedDictionary> GetDictionaryCategories(tcp::socket &sock,
                                                                        std::shared_ptr<std::unordered_set<std::wstring>> categories);

          /// @brief Matched Categories
          /// @param categories total list of categories
          /// @return Matched lost of categories
          std::shared_ptr<std::unordered_set<std::wstring>> GetMatchedCategories(std::shared_ptr<std::unordered_set<std::wstring>> categories);

          /// @brief Shows all categories which are avaliable on server
          /// @param sock socket for interaction with server
          /// @return list of which are avaliable on server
          std::shared_ptr<std::unordered_set<std::wstring>> GetCategoriesFromServer(tcp::socket &sock);
     };

     ///////////////////////////////////////////////////////////////////////

     class GameClientWin : public GameClient
     {
     public:
          /**
           * @brief Play For Windows
           */
          void Menu() const override;

          template <typename T>
          GameClientWin(const T &dict_adress, int port) : GameClient(dict_adress, port)
          {
               LoadDictionary();
          };

     private:
          void LoadDictionary() override;
     };

}
