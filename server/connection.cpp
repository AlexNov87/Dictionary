#include "servloader.h"

namespace server
{
class ServLoader;
beast::http::response<req_body> GetCrowl200Responce(int version = 11, bool keep_alive = false)
{
    beast::http::response<req_body> resp{beast::http::status::ok, version};
    resp.keep_alive(keep_alive);
    resp.set(boost::beast::http::field::content_type, "text/html");
    return resp;
}

beast::http::response<req_body> GetErrorResponce(std::string reason,
                                                 int version = 11, bool keep_alive = false)
{
    std::string ss = "Error reason: " + reason ;
    beast::http::response<req_body> resp{beast::http::status::bad_request, version};
    resp.keep_alive(keep_alive);
    resp.set(boost::beast::http::field::content_type, "application/octet-stream");
    resp.body() = std::move(ss);
    resp.prepare_payload();
    return resp;
}

}

namespace server
{
    Connection::Connection(tcp::socket socket, std::shared_ptr<loader::LanguageStorage> storage)
        : socket_(std::move(socket)), storage_(storage)
    {
    }

    void Connection::Run()
    {
        HandleSocket();
    };

    void Connection::HandleSocket()
    {

        try
        {
            beast::flat_buffer sbuffer;
            beast::http::request<req_body> request;
            beast::error_code ec;
            auto read_bytes = beast::http::read(socket_ , sbuffer, request, ec);
            if (read_bytes == 0)
            {
                return;
            }
            if (ec)
            {
                beast::http::write(socket_ , GetErrorResponce("Reading socket " + ec.message()));
                socket_.close();
                return;
            };

            MakeResponse(std::move(request));
        }
        catch (const std::exception &ex)
        {
            try
            {
                beast::http::write(socket_, GetErrorResponce("Error HandleSocket " + std::string(ex.what())));
                return;
            }
            catch (const std::exception &ex)
            {
                hf::WPrintSynchro(err_stream::stream, hf::ToWstr(ex.what()));
            }
        }
    }

    void Connection::MakeResponse(http::request<req_body> req)
    {

        try
        {
            auto objopt = BasicParseValue(socket_, req.body());
            if (!objopt)
            {
                HandleSocket();
            }
            auto &obj = *objopt;
            beast::http::response<req_body> responce = GetCrowl200Responce(req.version(), true);
            int action = (obj).at(JSONREQ::ACTION).AsInt();

            switch (action)
            {
            case 0: // SHOW_CATEGORIES
            
                responce.body() = ResponceShowCategories();
                break;
            case 1: // GET_ALL_CATEGORIES
                    
                responce.body() = ResponceAllCategoriesDictionary();
                break;
            case 2: // GET_NEEDED_CATEGORIES
            {
                 
              std::shared_ptr<std::unordered_set<std::wstring>>   categories = GetCategories(socket_, obj);
                if (!categories)
                {
                    HandleSocket();
                }
                responce.body() = ResponceNeededCategories(*categories);
            }
                break;

            default:
                beast::http::write(socket_ , GetErrorResponce("Unknown Action Command"));
                return;
                break;
            }

            responce.prepare_payload();
            beast::http::write(socket_, responce);
            HandleSocket();
        }
        catch (const std::exception &ex)
        {
            beast::http::write(socket_, GetErrorResponce("Global MakeResponce exception: " + std::string(ex.what())));
        }
    }

    std::string Connection::ReturnDictionary(const CategoriedDictionary &stor)
    {
        std::wstringstream wss;
        boost::archive::text_woarchive warch{wss};
        warch << stor;
        return hf::WstrToStr(wss.str());
    }

    std::string Connection::ResponceAllCategoriesDictionary()
    {
        return ReturnDictionary(*(storage_->ForwardDictionary()));
    }

    std::string Connection::ResponceShowCategories()
    {
        std::unordered_set<std::wstring> categories;
        for (auto &&ctg : *storage_->ForwardDictionary())
        {
            categories.insert(ctg.first);
        }
        std::wstringstream wss;
        boost::archive::text_woarchive warch{wss};
        warch << categories;
        return hf::WstrToStr(wss.str());
    }

    std::string Connection::ResponceNeededCategories(const std::unordered_set<std::wstring> &categories){
           CategoriedDictionary dct;
           for(auto && ctg : categories ){
              dct[ctg] = storage_->ForwardDictionary()->at(ctg);
           }
           return ReturnDictionary(dct);
    }

    std::shared_ptr<json::Dict> Connection::BasicParseValue(tcp::socket &socket, const std::string &body)
    {
        std::wstringstream wiss(hf::ToWstr(body));
        json::Node parsed_val(false);
        try
        {
            parsed_val = json::LoadNode(wiss);
        }
        catch (const std::exception &ex)
        {
            hf::WPrintSynchro(err_stream::stream,"Cannot Parse an object");
            beast::http::write(socket, GetErrorResponce("Cannot Parse an object"));
            return nullptr;
        }

        if (!parsed_val.IsMap())
        {
            hf::WPrintSynchro(err_stream::stream,"The value isn't an object");
            beast::http::write(socket, GetErrorResponce("The value isn't an object"));
            return nullptr;
        };
        json::Dict &obj = parsed_val.AsMap();
        if (!obj.contains(JSONREQ::ACTION))
        {
            hf::WPrintSynchro(err_stream::stream,"The object isn't contains ACTION");
            beast::http::write(socket, GetErrorResponce("The object isn't contains ACTION"));
            return nullptr;
        }

        if (!obj.at(JSONREQ::ACTION).IsInt())
        {
            hf::WPrintSynchro(err_stream::stream,"The object isn't INT at ACTION");
            beast::http::write(socket, GetErrorResponce("The object isn't INT at ACTION"));
            return nullptr;
        }
        return std::make_shared<json::Dict>(parsed_val.AsMap());
    }

    std::shared_ptr<std::unordered_set<std::wstring>> Connection::GetCategories(tcp::socket &socket, json::Dict &obj)
    {

        if (!obj.contains(JSONREQ::PARAMETERS))
        {
            hf::WPrintSynchro(err_stream::stream,"Request has no parameters");
            beast::http::write(socket, GetErrorResponce("Request has no parameters"));
            return nullptr;
        }

        if (!obj.at(JSONREQ::PARAMETERS).IsArray())
        {
             hf::WPrintSynchro(err_stream::stream,"Request at parameters is not array");
            beast::http::write(socket, GetErrorResponce("Request at parameters is not array"));
            return nullptr;
        }

        if (obj.at(JSONREQ::PARAMETERS).AsArray().empty())
        {
             hf::WPrintSynchro(err_stream::stream,"Parameters are empty");
            beast::http::write(socket, GetErrorResponce("Parameters are empty"));
            return nullptr;
        }

        std::unordered_set<std::wstring> result;

        for (auto &&node : obj.at(JSONREQ::PARAMETERS).AsArray())
        {
            if (!node.IsWString())
            {
                hf::WPrintSynchro(err_stream::stream,"Parameter error is not string");
                beast::http::write(socket, GetErrorResponce("Parameter error is not string"));
                return nullptr;
            }

            if (storage_->ForwardDictionary()->contains(node.AsString()))
            {
                result.insert(std::move(node.AsString()));
            }
        }

        if (result.empty())
        {
            return nullptr;
        }
        return std::make_shared<std::unordered_set<std::wstring>>(std::move(result));
    }

}

