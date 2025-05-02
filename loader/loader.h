#pragma once
#include "../helpfoo/definitions.h"
#include<iostream>
#include<memory>
#include<optional>
#include "mthread.h"
#include "../helpfoo/hfoo.h"
#include "../random/rand.h"

/**
 * @file This file implements storage of words. 
 */
namespace loader{

class LanguageStorage
{
public:
     
     /**
      * @brief Returns storage [category][language1][synonims language2]
      */
     const std::shared_ptr<CategoriedDictionary> &ForwardDictionary() const
     {
          return dictfwd;
     }
     /**
      * @brief Returns storage [category][language2][synonims language1]
      */
     const std::shared_ptr<CategoriedDictionary> &BackWardDictionary() const
     {
          return dictbwd;
     }
     /**
      * @brief Prints All dictionaries
      */
     void Print() const;
     LanguageStorage() = default ;
protected:
        
     /**
      * @brief Loads dictionaries from different places
      */
     virtual void LoadDictionary() = 0;
     /// @brief Contains language1 - language2
     std::shared_ptr<CategoriedDictionary> dictfwd;
     /// @brief Contains language2 - language1
     std::shared_ptr<CategoriedDictionary> dictbwd;
};

/**
 * @brief Loads Storage from Folder with files  
 */
class LanguageStorageFile : public LanguageStorage
{
     void LoadDictionary() override;
 public:    
     LanguageStorageFile(){
          LoadDictionary();
     }
};

/**
 * @brief Loads Storage from PostgreSQL Base  
 */
class LanguageStorageSql : public LanguageStorage
{
      void LoadDictionary() override;
      public: 
      LanguageStorageSql(){
          LoadDictionary();
     }

    
};

/**
 * @brief Indexes of categories
 * @param original collection of numbers
 * @param max_value max value, which can be matched
 * @returns Indexes, which could be valid in vector of pathes
 */
std::unordered_set<int> GetCategoryIndexes(const std::map<int, std::wstring> &cat_numerator);


/** @brief
 *  @returns Pairs of dictionaries from files
 */
std::pair<std::unordered_map<std::wstring, Dictionary>, std::unordered_map<std::wstring, Dictionary>> GetLangPairsFromTexts(std::string folder);

}