#include "request.h"

namespace client{
/////////////////////////////////////////////////////////////////////////////////////////////////
std::string RequestJsonBodyForAllCategories()
{
    json::Dict dct;
    dct[JSONREQ::ACTION] = json::Node(JSONREQ::GET_ALL_CATEGORIES);
    return Build8FromDict(std::move(dct));
}

std::string RequestJsonBodyForShowCategories(){
    json::Dict dct;
    dct[JSONREQ::ACTION] = json::Node(JSONREQ::SHOW_CATEGORIES);
    return Build8FromDict(std::move(dct));
}

std::string Build8FromDict(json::Dict dct){
    json::Node nd(std::move(dct));
    std::wostringstream wss;
    json::Print(nd, wss);
    return hf::WstrToStr(wss.str());
}
}