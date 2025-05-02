#pragma once
#include <string>
#include "../helpfoo/hfoo.h"
#include "../helpfoo/wjson.h"
#define BOOST_BEAST_USE_STD_STRING_VIEW


namespace client{

std::string Build8FromDict(json::Dict dct);

/// @brief Returns GET request without body
/// @param adress Adress where request will be sended
/// @param user_agent User agent who will send request
template <typename T, typename U>
http::request<req_body> GetReqWithoutBody(
    const T &adress,
    const U &user_agent)
{
    http::request<req_body> req(http::verb::get, adress, 11);
    req.set(http::field::host, adress);
    req.set(http::field::user_agent, user_agent);
    return req;
};

/*---------------------------------------------------------------------------------*/

/// @brief Returns request body to get needed categories
/// @param categories catyegory names which needed
template <typename Container>
std::string RequestJsonBodyForNeededCategories(const Container &categories)
{
    json::Array arr;
    for(auto&& cat: categories ){
        arr.push_back(json::Node(cat));
    }
      json::Dict dct;
      dct [JSONREQ::ACTION] = json::Node(JSONREQ::GET_NEEDED_CATEGORIES); 
      dct [JSONREQ::PARAMETERS] = json::Node(std::move(arr));
      return Build8FromDict(std::move(dct));  
}

template <typename T, typename Container, typename U>
http::request<req_body> RequestToGetNeededCategories(
    const T &adress,
    const Container &categories,
    const U &user_agent)
{
    http::request<req_body> req = GetReqWithoutBody(adress, user_agent);
    req.body() = RequestJsonBodyForNeededCategories(categories);
    req.keep_alive(true);
    req.prepare_payload();
    return req;
};

/*---------------------------------------------------------------------------------*/

/// @brief Returns request body to get all categories
std::string RequestJsonBodyForAllCategories();

/// @brief Returns request to get all categories
/// @param adress Adress where request will be sended
/// @param user_agent User agent who will send request
template <typename T, typename U>
http::request<req_body> RequestToGetAllCategories(
    const T &adress,
    const U &user_agent)
{
    http::request<req_body> req = GetReqWithoutBody(adress, user_agent);
    req.body() = RequestJsonBodyForAllCategories();
    req.prepare_payload();
    return req;
};

/*---------------------------------------------------------------------*/

/// @brief Returns request body to show all categories
std::string RequestJsonBodyForShowCategories();

/// @brief Returns request to show all categories
/// @param adress Adress where request will be sended
/// @param user_agent User agent who will send request
template <typename T, typename U>
http::request<req_body> RequestToShowCategories(
    const T &adress,
    const U &user_agent)
{
    http::request<req_body> req = GetReqWithoutBody(adress, user_agent);
    req.body() = RequestJsonBodyForShowCategories();
    req.prepare_payload();
    return req;
};
}
