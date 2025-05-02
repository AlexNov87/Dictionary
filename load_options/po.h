#pragma once
#include <boost/program_options.hpp>
#include "../helpfoo/hfoo.h"
#include <optional>
#include <iostream>

/**
 * @brief Structure which defines your server load options
 */
struct LoadOptions {
   //After exiting program, words will be updated to DB 
   bool auto_save_to_sql;
   DictionaryMode dmode;
   std::string ip;
   int port; 
};

/**
 * @brief Parses your command line
 */
std::optional<LoadOptions> ParseCommandLineServer(int args, const char** argv);