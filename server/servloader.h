#pragma once
#include "../loader/loader.h"
#include "../helpfoo/hfoo.h"
#include <cassert>
#include <string>
#include "../helpfoo/wjson.h"

namespace server{
class Connection : public std::enable_shared_from_this<Connection>
{
public:
    /// @brief Constructor
    /// @param socket socket to handle connecrion
    /// @param storage pointer to storage where are words
    Connection(tcp::socket socket, std::shared_ptr<loader::LanguageStorage> storage);
    /// @brief Start Handle connection
    void Run();

private:
    /// @brief Start Handle connection
    /// @param socket socket 
    void HandleSocket();
    
    /// @brief Makes responce to client
    /// @param req Request which need to handle
    void MakeResponse(http::request<req_body> req);
    
    /// @brief Returns serialized dictionary to put in to responce body
    /// @param stor Storage which you need to derialize
    /// @return Returns serialized dictionary to put in to responce body
    std::string ReturnDictionary(const CategoriedDictionary &stor);
    
    /// @brief Serialized All categories, which are in storage
    /// @return Serialized All categories, which are in storage
    std::string ResponceAllCategoriesDictionary();
    
    /// @brief Serialized names of all categories, which are in storage
    /// @return Serialized names of categories, which are in storage
    std::string ResponceShowCategories();

     /// @brief Serialized categories, which are you need
    /// @return Serialized categories, which are you need
    std::string ResponceNeededCategories(const std::unordered_set<std::wstring> &categories);
    
    /// @brief Parses request body to return json::Dict or writes to socket exception 
    /// @param socket socket
    /// @param body 
    /// @return json::Dict with parameters, which will be used to make responce;
    std::shared_ptr<json::Dict> BasicParseValue(tcp::socket &socket, const std::string &body);
    
    /// @brief Returns names of categories, which are id stortage or writes to socket exception
    /// @param socket socket
    /// @param obj CategoriedDictinary, which contains category
    /// @return  names of categories;
    std::shared_ptr<std::unordered_set<std::wstring>> GetCategories(tcp::socket &socket, json::Dict &obj);
private:    
    tcp::socket socket_;
    std::shared_ptr<loader::LanguageStorage> storage_;
};

class ServLoader
{
public:
    /// @brief Constructor
    /// @param epoint Endpoint, which comprised of ip and port
    /// @param mode SQL or file mode, which will be used to construct storages 
    ServLoader(asio::ip::tcp::endpoint epoint, DictionaryMode mode);
    /// @brief Stars server
    void StartServer();

private:
    /// @brief Check port that value is correct  
    /// @param port Port which will be used to listen connections
    void CheckPort(short port);
    /// @brief create an acceptor to listen connections
    void MakeAcceptor();
    /// @brief Create storage from files or from sql
    /// @param mode SQL of File Mode make the storages.
    void MakeStorage(DictionaryMode mode);

    /// @brief storage, where are words.  
    std::shared_ptr<loader::LanguageStorage> storage_;
    std::shared_ptr<tcp::acceptor> acceptor_ = nullptr;
    asio::io_context ioc_;
    asio::strand<asio::io_context::executor_type> strand_{asio::make_strand(ioc_)};
    asio::ip::tcp::endpoint epoint_;
};

}
