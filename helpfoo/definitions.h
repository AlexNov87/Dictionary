#pragma once
#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <syncstream>
#include <iostream>
#include <memory>

#include <boost/archive/text_woarchive.hpp>
#include <boost/archive/text_wiarchive.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unordered_set.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>


struct err_stream {
  static std::wostream& stream;
  static std::mutex syncprint_mtx;
};



namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace asio = boost::asio;
namespace ip = asio::ip;
using tcp = asio::ip::tcp;
using req_body = http::string_body;

using Dictionary = std::unordered_map<std::wstring, std::unordered_set<std::wstring>>;
using CategoriedDictionary = std::unordered_map<std::wstring, Dictionary>;

struct SQL
{
    static const std::string lang1;
    static const std::string lang2;
    static const std::string tr_base1;
    static const std::string tr_base2;

    static const std::string PORT;
    static const std::string TABLENAME;
    static const std::string USERNAME;
    static const std::string HOST;
    static const std::string PASSWORD;

private:
    SQL() {};
};



struct JSONREQ
{
    static const std::wstring ACTION;
    static const std::wstring PARAMETERS;
    static const int SHOW_CATEGORIES;
    static const int GET_ALL_CATEGORIES;
    static const int GET_NEEDED_CATEGORIES;

private:
    JSONREQ() = delete;
};

/**
 * @brief Mode of dictionary load to server
 */
enum class DictionaryMode
{
    SQL,
    FILE
};