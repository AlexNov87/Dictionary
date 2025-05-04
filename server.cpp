#include <iostream>
#include <string>
#include <sstream>
#include "load_options/po.h"
#include "server/servloader.h"
#include "sql/create_base.h"
#include "helpfoo/wjson.h"

int main(int argc, const char **argv)
{
  SETGLOBALLOCALES();
  try
  {
    auto load_options = ParseCommandLineServer(argc, argv);

    if (!load_options)
    {
      return EXIT_SUCCESS;
    }

    const auto adress = asio::ip::make_address(load_options.value().ip);
    const asio::ip::port_type port = asio::ip::port_type(load_options.value().port);
    asio::ip::tcp::endpoint ep{adress, port};
    server::ServLoader load(ep, load_options.value().dmode);
    load.StartServer();
  }
  catch (const std::exception &ex)
  {
    hf::WPrintSynchro(err_stream::stream, "EXCEPTION: ", hf::ToWstr(ex.what()));
  }
  // srv --ip 127.0.0.1 --port 80 --loadsql --auto_add_sql
}