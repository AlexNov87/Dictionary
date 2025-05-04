#include "hfoo.h"
using namespace std;
using namespace std::filesystem;

std::wostream &operator<<(std::wostream &os, const Dictionary &dictionary)
{
    for (auto &&[word, translates] : dictionary)
    {
        size_t cnt = 0;
        os << word << L" ----> ";
        for (auto &&syn : translates)
        {
            os << syn;
            if (cnt + 1 == translates.size())
            {
                break;
            }
            os << ",";
            ++cnt;
        }
        os << endl;
    }
    os << L"====================================================" << endl;
    return os;
};

std::wostream &operator<<(std::wostream &os, const CategoriedDictionary &dictionary)
{
    for (auto &&[cat_name, dict] : dictionary)
    {
        os << L"Category: " << cat_name << endl
           << L"------------------------------------------------------" << endl;
        os << dict << endl;
    }
    return os;
};

void SETGLOBALLOCALES()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
}

namespace hf
{
    std::string GetFilename(const path &pth)
    {
        // recieving full filename
        std::string complex = pth.filename().generic_string();
        // finding extension
        auto pos = complex.find(pth.extension().generic_string());
        // cutting extension
        complex = complex.substr(0, pos);
        ba::trim(complex);
        // return name
        return complex;
    }
    
    std::vector<std::string> ReadFromFile(const std::filesystem::path &pth)
    {
        vector<string> res;
        ifstream ifs(pth);
        if (!ifs)
        {
            throw std::runtime_error("Something in stream in " + pth.string());
        }
        while (ifs)
        {
            string tmp;
            getline(ifs, tmp);
            ba::trim(tmp);
            if (tmp.empty())
            {
                continue;
            }
            res.push_back(move(tmp));
        }
        return res;
    };

    std::string WstrToStr(const std::wstring &value)
    {
        return boost::locale::conv::utf_to_utf<char>(value);
    }

    bool Confirm()
    {
        std::wcout << "ARE YOU SURE?\n"
                      "Y,y - yes , N,n - no\n";
        wchar_t c;
        std::wcin >> c;
        if (c == 'Y' || c == 'y')
        {
            return true;
        }
        return false;
    };

    TextEncodingDetect::Encoding GetEncode(const std::filesystem::path &path)
    {

        // Open file in binary mode
        FILE *file = _wfopen(path.c_str(), L"rb");

        if (file == NULL)
        {
            wprintf(L"\nCould not open file.\n");
            throw("File can not be open");
        }

        // Get file size
        fseek(file, 0, SEEK_END);
        long fsize = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Read it all in
        unsigned char *buffer = new unsigned char[fsize];
        fread(buffer, fsize, 1, file);
        fclose(file);

        // Detect the encoding
        TextEncodingDetect textDetect;
        TextEncodingDetect::Encoding encoding = textDetect.DetectEncoding(buffer, fsize);

        return encoding;
    };

}
