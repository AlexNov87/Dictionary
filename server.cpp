#include<iostream>
#include <string>
#include <sstream>
#include "load_options/po.h"
#include "server/servloader.h"
#include "sql/create_base.h"
#include "helpfoo/wjson.h"





int main(int argc, const char** argv) {

SETGLOBALLOCALES();
 
 auto load_options = ParseCommandLineServer(argc, argv);
 const auto adress = asio::ip::make_address(load_options.value().ip);
 const asio::ip::port_type port = asio::ip::port_type(load_options.value().port);
 asio::ip::tcp::endpoint  ep{adress, port};
 server::ServLoader load(ep, load_options.value().dmode);
 load.StartServer();
 //srv --ip 127.0.0.1 --port 80 --loadsql --auto_add_sql
}