#include "servloader.h"


namespace server{
  
   
   ServLoader::ServLoader(asio::ip::tcp::endpoint epoint, DictionaryMode mode) : epoint_(std::move(epoint))
    {
        MakeAcceptor();
        MakeStorage(mode);
    }

    void  ServLoader::StartServer()
    {
       std::wcout<< L"Acceptor Waiting....." << std::endl;
        assert(acceptor_ != nullptr);
        acceptor_->async_accept(strand_, [this](boost::system::error_code ec, tcp::socket socket)
                                {     
                                    
                                    std::thread th1([](tcp::socket socket, std::shared_ptr<loader::LanguageStorage> storage ) {
                                              std::make_shared<Connection>(std::move(socket), storage)->Run();
                                    }, std::move(socket), storage_ );
                                    th1.detach();
                                  //  
                                    StartServer(); });
        ioc_.run();
    };

     void ServLoader::CheckPort(short port)
    {
        if (port > 65500)
        {
            hf::WPrintSynchro(err_stream::stream, L"The port is invalid");
            abort();
        }
    }

    void ServLoader::MakeAcceptor()
    {
        CheckPort(epoint_.port());
        try
        {
            acceptor_ = std::make_shared<tcp::acceptor>(strand_, epoint_);
            acceptor_->listen(asio::socket_base::max_listen_connections);
        }
        catch (const std::exception &ex)
        {   
            hf::WPrintSynchro(err_stream::stream, L"Excepion in making Acceptor: " , ex.what());
            abort();
        }
    }

    void ServLoader::MakeStorage(DictionaryMode mode)
    {
        switch (mode)
        {
        case DictionaryMode::SQL:
            storage_ = std::make_shared<loader::LanguageStorageSql>();
            break;
        case DictionaryMode::FILE:
            storage_ = std::make_shared<loader::LanguageStorageFile>();
            break;
        }
    }
}