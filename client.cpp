#include <iostream>
#include "load_options/po.h"
#include "client/client.h"
#include "sql/create_base.h"
#include "sql/do_connection_str.h"

int main(int argc, const char **argv)
{
    
   try{ 
    SETGLOBALLOCALES();
    client::GameClientWin client("127.0.0.1", 80);
    client.Menu();
   
   }
   catch(const std::exception& ex){
      hf::WPrintSynchro(err_stream::stream , "EXCEPTION: ", hf::ToWstr(ex.what()));
      abort();
   }
   


//     auto load_options = ParseCommandLine(argc, argv);
   // CreateDataBase();
   // LanguageStorageSql sqp;
    
    
 // GameClientWin gc("127.0.0.1",80);
   //gc.ForceLoadDictionary(sqp);
   //gc.Menu();

}