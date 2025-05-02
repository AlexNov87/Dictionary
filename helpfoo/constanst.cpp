#include "definitions.h"
using namespace std;


const string SQL::lang1 = "Lang1Words";
const string SQL::lang2 = "Lang2Words";
const string SQL::tr_base1 = "TrBase1";
const string SQL::tr_base2 = "TrBase2";

const string SQL::PORT = "PORT";
const string SQL::TABLENAME = "TABLENAME";
const string SQL::USERNAME = "USERNAME";
const string SQL::HOST = "HOST";
const string SQL::PASSWORD = "PASSWORD";

//////////////////////////////////////////

const std::wstring JSONREQ::ACTION = L"ACTION";
const std::wstring JSONREQ::PARAMETERS = L"PARAMETERS";
const int JSONREQ::SHOW_CATEGORIES = 0;
const int JSONREQ::GET_ALL_CATEGORIES = 1;
const int JSONREQ::GET_NEEDED_CATEGORIES = 2;

std::wostream& err_stream::stream = std::wcerr;
std::mutex err_stream::syncprint_mtx;


