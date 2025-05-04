#include "sql_editor/editor.h"
#include "sql/sql_translator_build.h"
#include<unordered_set>
#include<string>


int main(){
    SETGLOBALLOCALES();
  


   sql_editor::LoadToSQLClient cli;
   cli.Menu();
}