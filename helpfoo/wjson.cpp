#include "wjson.h"

namespace json
{
 
   
    using namespace std;
    ////////////////////////////////////////////////////////////////////////////////////////////
    //                                                                                        //
    //                                                                                        //
    //                         ПСЕВДОНИМЫ ТИПОВ  И ОБЬЯВЛЕНИЯ                                 //
    //                                                                                        //
    //                                                                                        //
    ////////////////////////////////////////////////////////////////////////////////////////////

    using Number = std::variant<int, double>;
    Number DigitParser(std::wistream &input);
    Node LoadNode(wistream &input);
    std::wstring StringParser(std::wistream &input);
#define GAP3 "   "
#define GAP4 "    "
#define GAP5 "     "
#define GAP6 "      "
#define GAP7 "       "
    ////////////////////////////////////////////////////////////////////////////////////////////
    //                                                                                        //
    //                                                                                        //
    //                                     NODE                                               //
    //                                                                                        //
    //                                                                                        //
    ////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////
    //                          СРАВНЕНИЕ ТИПОВ                                               //
    ////////////////////////////////////////////////////////////////////////////////////////////

    bool Node::IsInt() const
    {
        return std::holds_alternative<int>(*this);
    }
    bool Node::IsDouble() const
    {
        if (std::holds_alternative<double>(*this) || std::holds_alternative<int>(*this))
            return true;
        else
            return 0;
    }

    bool Node::IsPureDouble() const
    {
        return std::holds_alternative<double>(*this);
    }

    bool Node::IsBool() const
    {
        return std::holds_alternative<bool>(*this);
    }
    bool Node::IsWString() const
    {
        return std::holds_alternative<std::wstring>(*this);
    }
    bool Node::IsNull() const
    {
        return std::holds_alternative<nullptr_t>(*this);
    }

    bool Node::IsArray() const
    {
        return std::holds_alternative<Array>(*this);
    }
    bool Node::IsMap() const
    {
        return std::holds_alternative<Dict>(*this);
    }
}

namespace json
{

    ////////////////////////////////////////////////////////////////////////////////////////////
    //                          ВЕРНУТЬ В КАЧ-ВЕ..                                            //
    ////////////////////////////////////////////////////////////////////////////////////////////

    int Node::AsInt() const
    {
        if (!IsInt())
            throw std::logic_error("non-valid type");
        return std::get<int>(*this);
    }
    double Node::AsDouble() const
    {
        using namespace std::literals;
        if (!IsDouble())
        {
            throw std::logic_error("Not a double"s);
        }
        return IsPureDouble() ? std::get<double>(*this) : AsInt();
    }
    double Node::AsPureDouble() const
    {
        if (!IsPureDouble())
            throw std::logic_error("non-valid type");
        return std::get<double>(*this);
    }
    bool Node::AsBool() const
    {
        if (!IsBool())
            throw std::logic_error("non-valid type");
        return std::get<bool>(*this);
    }

    const std::wstring &Node::AsString() const
    {
        if (!IsWString())
            throw std::logic_error("non-valid type");
        return std::get<std::wstring>(*this);
    }

    const Array &Node::AsArray() const
    {
        if (!IsArray())
            throw std::logic_error("non-valid type");
        return std::get<Array>(*this);
    }
    const Dict &Node::AsMap() const
    {
        if (!IsMap())
            throw std::logic_error("non-valid type");
        return std::get<Dict>(*this);
    }

    const Value &Node::GetValue() const
    {
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // неконстантые версии для move

    std::wstring &Node::AsString()
    {
        if (!IsWString())
            throw std::logic_error("non-valid type");
        return std::get<std::wstring>(*this);
    }

    Array &Node::AsArray()
    {
        if (!IsArray())
            throw std::logic_error("non-valid type");
        return std::get<Array>(*this);
    }
    Dict &Node::AsMap()
    {
        if (!IsMap())
            throw std::logic_error("non-valid type");
        return std::get<Dict>(*this);
    }

    Value &Node::GetValue()
    {
        return *this;
    }

}

namespace json
{



    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////
    //                                                                                        //
    //                                                                                        //
    //                         LOAD                                                           //
    //                                                                                        //
    //                                                                                        //
    ////////////////////////////////////////////////////////////////////////////////////////////

    Node LoadArray(wistream &input)
    {
        Array result;
        for (wchar_t c; input >> c && c != ']';)
        {
            if (c != ',')
            {
                input.putback(c);
            }
            result.push_back(LoadNode(input));
        }
        if (!input){
            hf::WPrintSynchro(err_stream::stream, "Bad stream\n");
       
            throw runtime_error("Bad stream"s);
            }
        return Node(move(result));
    }

    Node LoadDigit(wistream &input)
    {
        auto result = DigitParser(input);
        if (std::holds_alternative<double>(result))
        {
            return Node(get<double>(result));
        }
        return Node(get<int>(result));
    }

    Node LoadString(wistream &input)
    {
        wstring line = StringParser(input);
        return Node(move(line));
    }

    Node LoadDict(wistream &input)
    {
        Dict result;
        for (wchar_t c; input >> c && c != '}';)
        {
            if (c == ',')
            {
                input >> c;
            }
            wstring key = LoadString(input).AsString();
            input >> c;
            result.insert({move(key), LoadNode(input)});
            if (!input) {
                
               hf::WPrintSynchro(err_stream::stream, L"Bad stream\n");
               throw runtime_error("Bad stream"s);
            }
        }
        //   if (result.empty())throw ParsingError("Bad stream"s);
        return Node(move(result));
    }

    Node LoadBoolorNull(std::wistream &input)
    {
        string line;
        while (std::isalpha(input.peek()))
        {
            line.push_back(static_cast<char>(input.get()));
        }
        if (line == "null")
        {
            return Node(nullptr);
        }
        if (line == "false")
        {
            return Node(false);
        }
        if (line == "true")
        {
            return Node(true);
        }
        
        hf::WPrintSynchro(err_stream::stream, L"Incorrect symb");
        throw runtime_error("Incorrect symb");
     }

    Node LoadNode(wistream &input)
    {
        wchar_t c;
        input >> c;

        switch (c)
        {

        case '[':
            return LoadArray(input);
            break;

        case '{':
            return LoadDict(input);
            break;

        case '"':
            return LoadString(input);
            break;

        case 't':
            input.putback(c);
            return LoadBoolorNull(input);
        case 'f':
            input.putback(c);
            return LoadBoolorNull(input);
        case 'n':
            input.putback(c);
            return LoadBoolorNull(input);
            break;

        default:
            input.putback(c);
            return LoadDigit(input);
            break;
        }
    }

}

  namespace json { 
////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
//                                                                                        //
//                         HELP FOO                                                       //
//                                                                                        //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////
wostream &operator<<(wostream &os, const Node &nod)
{
    std::visit(NodeVisiter{os}, nod.GetValue());
    return os;
}


void Print(const Node& node, std::wostream& output)
{
    std::visit(NodeVisiter{output}, node.GetValue());
}




Number DigitParser(std::wistream &input)
{
    using namespace std::literals;

    std::wstring parsed_num;

    // Считывает в parsed_num очередной символ из input
    auto read_char = [&parsed_num, &input]
    {
        parsed_num += static_cast<wchar_t>(input.get());
        if (!input)
        {
            hf::WPrintSynchro(err_stream::stream, L"Failed to read number from stream\n");
            throw runtime_error("Incorrect symb"s);
        }
    };

    // Считывает одну или более цифр в parsed_num из input
    auto read_digits = [&input, read_char]
    {
        if (!std::isdigit(input.peek()))
        {
            hf::WPrintSynchro(err_stream::stream, "A digit is expected\n");
            throw runtime_error("A digit is expected"s);
        }
        while (std::isdigit(input.peek()))
        {
            read_char();
        }
    };

    if (input.peek() == '-')
    {
        read_char();
    }
    // Парсим целую часть числа
    if (input.peek() == '0')
    {
        read_char();
        // После 0 в JSON не могут идти другие цифры
    }
    else
    {
        read_digits();
    }

    bool is_int = true;
    // Парсим дробную часть числа
    if (input.peek() == '.')
    {
        read_char();
        read_digits();
        is_int = false;
    }

    // Парсим экспоненциальную часть числа
    if (int ch = input.peek(); ch == 'e' || ch == 'E')
    {
        read_char();
        if (ch = input.peek(); ch == '+' || ch == '-')
        {
            read_char();
        }
        read_digits();
        is_int = false;
    }

    try
    {
        if (is_int)
        {
            // Сначала пробуем преобразовать строку в int
            try
            {
                return std::stoi(parsed_num);
            }
            catch (...)
            {
                // В случае неудачи, например, при переполнении,
                // код ниже попробует преобразовать строку в double
            }
        }
        return std::stod(parsed_num);
    }
    catch (...)
    {
        hf::WPrintSynchro(err_stream::stream, "Failed to convert to number\n");
        throw runtime_error("Failed to convert to number"s);
    }
}

// Считывает содержимое строкового литерала JSON-документа
// Функцию следует использовать после считывания открывающего символа ":
std::wstring StringParser(std::wistream &input)
{
    using namespace std::literals;
    
     
    auto it = std::istreambuf_iterator<wchar_t>(input);
    auto end = std::istreambuf_iterator<wchar_t>();
    std::wstring s;
    while (true)
    {
        if (it == end)
        {
            // Поток закончился до того, как встретили закрывающую кавычку?
            hf::WPrintSynchro(err_stream::stream, "String parsing error\n");
            throw runtime_error("String parsing error"s);
        }
        const wchar_t ch = *it;
        if (ch == '"')
        {
            // Встретили закрывающую кавычку
            ++it;
            break;
        }
        else if (ch == '\\')
        {
            // Встретили начало escape-последовательности
            ++it;
            if (it == end)
            {
                // Поток завершился сразу после символа обратной косой черты
                hf::WPrintSynchro(err_stream::stream, "String parsing error\n");
                throw runtime_error("String parsing error"s);
            }
            const wchar_t escaped_char = *(it);
            // Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
            switch (escaped_char)
            {
            case 'n':
                s.push_back('\n');
                break;
            case 't':
                s.push_back('\t');
                break;
            case 'r':
                s.push_back('\r');
                break;
            case '"':
                s.push_back('"');
                break;
            case '\\':
                s.push_back('\\');
                break;
            default:
                // Встретили неизвестную escape-последовательность
                 hf::WPrintSynchro(err_stream::stream, "Unrecognized escape sequence\n");
                 throw runtime_error("Unrecognized escape sequence"s);
              
            }
        }
        else if (ch == '\n' || ch == '\r')
        {
            // Строковый литерал внутри- JSON не может прерываться символами \r или \n
                 hf::WPrintSynchro(err_stream::stream , "Unexpected end of line\n");
                 throw runtime_error("Unexpected end of line"s);
          
        }
        else
        {
            // Просто считываем очередной символ и помещаем его в результирующую строку
            s.push_back(ch);
        }
        ++it;
    }
    return s;
}
}

  namespace json { 

////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        //
//                                                                                        //
//                                     NODE_VISITOR                                       //
//                                                                                        //
//                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////

void NodeVisiter::operator()(std::nullptr_t) const
{
    out << L"null";
}
void NodeVisiter::operator()(bool v) const
{
    out << std::boolalpha << v;
}
void NodeVisiter::operator()(double v) const
{
    out << v;
}
void NodeVisiter::operator()(int v) const
{
    out << v;
}

void NodeVisiter::operator()(const std::wstring &v) const
{
    out << L"\"";
    for (char c : v)
    {
        switch (c)
        {
        case '\\':
            out << L"\\\\";
            break;

        case '"':
            out << L"\\\"";
            break;

        case '\n':
            out << L"\\n";
            break;

        case '\r':
            out << L"\\r";
            break;

        case '\t':
            out << L"\\t";
            break;

        default:
            out << c;
            break;
        }
    }
    out << L"\"";
}

void NodeVisiter::operator()(const Array &v) const
{
    out << std::endl
        << GAP5 << L"[" << std::endl;
    for (size_t i = 0; i < v.size(); ++i)
    {
        out << GAP7;
        std::visit(NodeVisiter{out}, v[i].GetValue());
        if (i + 1 == v.size())
        {
            break;
        }
        out << L"," << std::endl;
    }
    out << L"\n"
        << GAP5 << L"]" << std::endl;
}
void NodeVisiter::operator()(const Dict &v) const
{

    size_t lim = v.size();
    out << endl
        << L"{" << std::endl;
    for (const auto &i : v)
    {
        out << GAP4 << L"\"" << i.first << L"\": ";
        std::visit(NodeVisiter{out}, i.second.GetValue());
        --lim;
        if (lim == 0)
            break;
        out << L"," << std::endl;
    }
    out << std::endl
        << L"}" << std::endl;
}
}
