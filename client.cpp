#include <iostream>
#include "load_options/po.h"
#include "client/client.h"
#include "sql/create_base.h"
#include "sql/do_connection_str.h"

int main(int argc, const char **argv)
{
  SETGLOBALLOCALES();
  try
  {
    auto load_options = ParseCommandLineClient(argc, argv);

    if (!load_options)
    {
      return EXIT_SUCCESS;
    }
    client::GameClientWin client(load_options.value().ip, load_options.value().port);
    client.Menu();
  }
  catch (const std::exception &ex)
  {
    hf::WPrintSynchro(err_stream::stream, "EXCEPTION: ", hf::ToWstr(ex.what()));
    abort();
  }

}