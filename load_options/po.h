#pragma once
#include <boost/program_options.hpp>
#include "../helpfoo/hfoo.h"
#include <optional>
#include <iostream>



/**
 * @brief Structure which defines your client load options
 */
struct LoadClientOptions {
   std::string ip;
   int port; 
};

/**
 * @brief Structure which defines your server load options
 */
struct LoadServerOptions : public LoadClientOptions {

   DictionaryMode dmode;
   
};

/**
 * @brief Parses your command line
 */
std::optional<LoadServerOptions> ParseCommandLineServer(int args, const char** argv);

/**
 * @brief Parses your command line
 */
std::optional<LoadClientOptions> ParseCommandLineClient(int args, const char** argv);