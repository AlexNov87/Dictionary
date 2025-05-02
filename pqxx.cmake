set(CMAKE_PREFIX_PATH  "F:\\COMPILESOFT\\vcpkg\\installed\\x64-mingw-static\\share")
#показать пути к vcpkg include библиотекам
include_directories("F:\\COMPILESOFT\\vcpkg\\installed\\x64-mingw-static\\include")
link_directories("F:\\COMPILESOFT\\vcpkg\\installed\\x64-mingw-static\\lib")
 
#установка параметров postgre
set(PostgreSQL_INCLUDE_DIR "F:\\COMPILESOFT\\POSTGRE\\17.0.1\\include")
set(PostgreSQL_LIBRARY_DIR "F:\\COMPILESOFT\\POSTGRE\\17.0.1\\lib")
set(PostgreSQL_ROOT "F:\\COMPILESOFT\\POSTGRE\\17.0.1")
#показать пути к postgre
include_directories("F:\\COMPILESOFT\\POSTGRE\\17.0.1\\include")
link_directories("F:\\COMPILESOFT\\POSTGRE\\17.0.1\\lib")
 
#установка параметров open ssl
set(OPENSSL_INCLUDE_DIR "F:/COMPILESOFT/vcpkg/packages/openssl_x64-mingw-static/include/openssl")
set(OPENSSL_CRYPTO_LIBRARY "F:/COMPILESOFT/vcpkg/packages/openssl_x64-mingw-static/lib/libcrypto.a")
set(OPENSSL_SSL_LIBRARY "F:/COMPILESOFT/vcpkg/packages/openssl_x64-mingw-static/lib/liblibssl.a")
set(OPENSSL_ROOT_DIR "F:/COMPILESOFT/vcpkg/packages/openssl_x64-mingw-static")
#показать пути к openssl
link_directories("F:/COMPILESOFT/vcpkg/packages")
include_directories("F:/COMPILESOFT/vcpkg/packages/openssl_x64-mingw-static/include/openssl")
 
find_package(OpenSSL REQUIRED)
if(OpenSSL_FOUND)
    message(STATUS "------------------>>>>> OpenSSL found: ${OpenSSL_VERSION}")
    message(STATUS "------------------>>>>> OpenSSL found: ${OpenSSL_ROOT}")
else()
    message(FATAL_ERROR "OpenSSL not found")
endif()
 
find_package(libpqxx REQUIRED)
if(libpqxx_FOUND)
    message(STATUS "------------------>>>>> libpqxx found: ${libpqxx_VERSION}")
    message(STATUS "------------------>>>>> libpqxx found: ${libpqxx_ROOT}")
else()
    message(FATAL_ERROR "libpqxx not found")
endif()