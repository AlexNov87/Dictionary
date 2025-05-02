#include "hfoo.h"
using namespace std;
using namespace std::filesystem;
namespace ba = boost::algorithm;



std::wostream& operator <<(std::wostream& os, const Dictionary& dictionary){
    for(auto&& [word, translates] : dictionary){
        size_t cnt = 0; 
        os<< word <<L" ----> ";
        for (auto&& syn : translates){
            os << syn; 
            if(cnt + 1 == translates.size()){break;}   
            os << ",";
            ++cnt;
        }
        os<< endl;
    }
    os << L"===================================================="<< endl;
    return os;
};

std::wostream& operator <<(std::wostream& os, const CategoriedDictionary& dictionary) {
   for(auto && [cat_name, dict] : dictionary){
        os<< L"Category: " << cat_name << endl << L"------------------------------------------------------"<<endl;
        os<< dict << endl;
    }
    return os;
};

void SETGLOBALLOCALES(){
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin),  _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
}


namespace hf{
std::string GetFilename(const path &pth)
{
    //recieving full filename
    std::string complex = pth.filename().generic_string();
    //finding extension
    auto pos = complex.find(pth.extension().generic_string());
    //cutting extension
    complex = complex.substr(0, pos);
    ba::trim(complex);
    //return name
    return complex;
}

unordered_set<wstring> SplitBySymbolSet(const wstring &line, char splitter)
{
    unordered_set<wstring> res;
    std::wstring tmp;
    for (char c : line)
    {
        
        if (c != splitter)
        {
            tmp += c;
        }
        else
        {
            ba::trim(tmp);
            if (!tmp.empty())
            {
                res.insert(move(tmp));
            };
            tmp.clear();
        }
    }
    ba::trim(tmp);
    if (!tmp.empty())
    {
        res.insert(move(tmp));
    }
    return res;
}

vector<string> SplitBySymbolVec(const string &line, char splitter)
{
    vector<string> res;
    std::string tmp;
    for (char c : line)
    {
        
        if (c != splitter)
        {
            tmp += c;
        }
        else
        {
            ba::trim(tmp);
            if (!tmp.empty())
            {
                res.push_back(move(tmp));
            };
            tmp.clear();
        }
    }
    
    if (!tmp.empty())
    {
       ba::trim(tmp);
    }
    if (!tmp.empty())
    {
        res.push_back(move(tmp));
    }
    return res;
}


std::vector<std::string> ReadFromFile(const std::filesystem::path& pth){
    vector<string> res;
    ifstream ifs(pth);
    if(!ifs){
        throw std::runtime_error("Something in stream in " + pth.string());
    }
    while(ifs){
        string tmp;
        getline(ifs, tmp);
        ba::trim(tmp);
        if(tmp.empty()){continue;}
        res.push_back(move(tmp));
    }
    return res;
};

std::string WstrToStr(const std::wstring& value){
  return  boost::locale::conv::utf_to_utf<char>(value);
}

}

