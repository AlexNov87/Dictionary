#include "po.h"
namespace po = boost::program_options;
using namespace std;

/**
 * @brief Checks for correct mode of loaning dictionary may be "loadsql" or "loadfile"
 */
void CheckDictionaryLoadModeError(const po::variables_map &vm)
{
    bool condition1 = (vm.count("loadsql") && vm.count("loadfile"));
    bool condition2 = (!vm.count("loadsql") && !vm.count("loadfile"));
    if (condition1 || condition2)
    {
        hf::WPrintSynchro(err_stream::stream, L"Invalid arguments to load dictionary on server (SQL or File mode error)");
        abort();
    }
}

std::optional<LoadOptions> ParseCommandLineServer(int argc, const char **argv)
{
    try
    {
        LoadOptions lo;
        po::options_description options{"Options"};
        options.add_options()("help,h", "Show Help")("auto_add_sql", "After Exit Saves Values To SQL Base")("loadsql", "Loads server dictyionary from SQL")("loadfile", "Loads server dictyionary from file")("ip", po::value(&lo.ip)->value_name("ip"), "Ip-adress where wil server be avalible")("port", po::value(&lo.port)->value_name("port"), "Port to coonect");

        po::variables_map vm;
        po::parsed_options parsed_res = po::parse_command_line(argc, argv, options);
        po::store(std::move(parsed_res), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << options << std::endl;
            return std::nullopt;
        }
        CheckDictionaryLoadModeError(vm);

        ///////////////////////////////////////////////////
        if (vm.count("auto_add_sql"))
        {
            lo.auto_save_to_sql = true;
            wcout << L"Saving to SQL parameter enabled" << endl;
        }
        //////////////////////////////////////////////////
        if (vm.count("loadsql"))
        {
            lo.dmode = DictionaryMode::SQL;
        }
        else
        {
            lo.dmode = DictionaryMode::FILE;
        }
        ////////////////////////////////////////////////
        if (!vm.count("ip"))
        {
            hf::WPrintSynchro(err_stream::stream, L"Saving to SQL parameter enabled");
            abort();
        }

        if (!vm.count("port"))
        {
            hf::WPrintSynchro(err_stream::stream, L"Saving to SQL parameter enabled");
            abort();
        }

        return lo;
    }
    catch (const std::exception &ex)
    {
      hf::WPrintSynchro(err_stream::stream, hf::ToWstr(ex.what()));
      abort();
    }

    return nullopt;
};

