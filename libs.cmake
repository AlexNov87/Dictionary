set(helps 
helpfoo/hfoo.cpp
helpfoo/hfoo.h
helpfoo/definitions.h
helpfoo/constanst.cpp
helpfoo/wjson.h
helpfoo/wjson.cpp
)
add_library(hlp ${helps})
target_link_libraries(hlp ${Boost_LIBRARIES})


set(load_options 
load_options/po.h
load_options/po.cpp
)
add_library(parse_command ${load_options})
target_link_libraries(parse_command ${Boost_LIBRARIES})


set (sql_works 
sql/do_connection_str.h
sql/do_connection_str.cpp
sql/create_base.h
sql/create_base.cpp
sql/sql_translator_build.h
sql/sql_translator_build.cpp
)
add_library(sql ${sql_works})
target_link_libraries(sql ${Boost_LIBRARIES} libpqxx::pqxx)


set(loader_words 
loader/loader.h
loader/loader.cpp
loader/mthread.cpp
loader/mthread.h
random/rand.h 
)
add_library(word_loader ${loader_words})
target_link_libraries(word_loader ${Boost_LIBRARIES} sql)



set (servercomplect
server/servloader.h
server/servloader.cpp
server/connection.cpp
)
add_library(srvlib ${servercomplect})
target_link_libraries(srvlib ${Boost_LIBRARIES} hlp sql)


set(clientcomplect
client/client.h
client/play.cpp
client/build.cpp
client/request.h
client/request.cpp
helpfoo/constanst.cpp
)
add_library(clientlib ${clientcomplect})
target_link_libraries(clientlib ${Boost_LIBRARIES} word_loader sql)

#######################
set (LIBS parse_command word_loader sql hlp srvlib clientlib) 
############################