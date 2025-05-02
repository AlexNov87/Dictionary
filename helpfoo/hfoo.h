#pragma once
#include "definitions.h"
#include "wjson.h"
#include <boost/locale/encoding.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <io.h>    // для функции _setmode
#include <fcntl.h> // для константы _O_U16TEXT
#include <filesystem>
#include <fstream>
#include <iostream>

std::wostream &operator<<(std::wostream &os, const Dictionary &dictionary);
std::wostream &operator<<(std::wostream &os, const CategoriedDictionary &dictionary);
void SETGLOBALLOCALES();

namespace hf{

template<typename T>
std::vector<uint8_t> Convert16to8(const T&vec16)
{
    std::vector<uint8_t> vec8;
    for (uint16_t val : vec16)
    {
        vec8.push_back(static_cast<uint8_t>(val));        // младший байт
        vec8.push_back(static_cast<uint8_t>(val >> 8)); // старший байт   
    }
    return vec8;
}

template<typename T>
std::vector<uint16_t> Convert8to16(const T& vec8)
{
    std::vector<uint16_t> vec16;
    for(int i = 0; i< vec8.size(); i+=2){
      uint16_t val =  static_cast<uint16_t>(vec8[i]) | static_cast<uint16_t>(vec8[i+1] << 8);
       vec16.push_back(val);
    }
    return vec16;
}

/** @brief Filename without extention
 *  @details For example input will be "name.txt", output will be "name";
 *  @param pth Path
 *  @returns Returns the filename without extention
 *
 */
std::string GetFilename(const std::filesystem::path &pth);
/**
 * @brief Splits Line by symbol
 * @param line Line, which will be splitted
 * @param splitter char which role is delimeter
 * @returns Returns collection of unique words
 */
std::unordered_set<std::wstring> SplitBySymbolSet(const std::wstring &line, char splitter);

/**
 * @brief Splits Line by symbol
 * @param line Line, which will be splitted
 * @param splitter char which role is delimeter
 * @returns Returns collection of words , may be repeats
 */
std::vector<std::string> SplitBySymbolVec(const std::string &line, char splitter);

/**
 * @brief Reads info from file to string vector
 * @param pth Way to file which we must read
 * @returns Collection, which has been read from file
 */
std::vector<std::string> ReadFromFile(const std::filesystem::path &pth);

/**
 * @brief Put value into Round Brackets
 * @param str Value which we need to modify
 * @returns (str)
 */
template <typename T>
std::string ToRoundBuskets(const T &str)
{
    std::ostringstream oss;
    oss << '(' << str << ')';
    return oss.str();
};

/**
 * @brief Put value into unary quotes
 * @param str Value which we need to modify
 * @returns 'str'
 */
template <typename T>
std::string ToUnaryQuotes(const T &str)
{
    std::ostringstream oss;
    oss << '\'' << str << '\'';
    return oss.str();
}

/**
 * @brief Save anytype values to the stream after each value will be " , "
 * @param args Pack of parameters
 * @returns "value1 , value2 , value3"
 */
template <typename... Args>
std::string PackofParamsDividedByCommon(Args... args)
{
    std::ostringstream oss;
    (..., (oss << args << " , "));
    std::string str = move(oss.str());
    str = str.substr(0, str.size() - 3);
    return str;
}


/**
 * @brief Save anytype values to the stream after each value will be " , "
 * @param args Pack of parameters
 */
template <typename... Args>
void WPrintSynchro(std::wostream& os, Args... args)
{
    std::lock_guard<std::mutex> lgr(err_stream::syncprint_mtx);
    (..., (os << args << " "));
    os<<"\n";
}

/**
 * @brief Converts any type to wstring
 * @param value value to convert
 */
template <typename T>
std::wstring ToWstr(const T &value)
{
    return boost::locale::conv::utf_to_utf<wchar_t>(value);
}

/**
 * @brief Converts wstring to string
 * @param value value to convert
 */
std::string WstrToStr(const std::wstring &value);
}