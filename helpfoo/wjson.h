#pragma once
#include "hfoo.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>


namespace json
{
  using namespace std::literals;
  ////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                                        //
  //                                                                                        //
  //                         ПСЕВДОНИМЫ ТИПОВ  И ОБЬЯВЛЕНИЯ                                 //
  //                                                                                        //
  ////////////////////////////////////////////////////////////////////////////////////////////
  class Node; 
  class Document;
  struct NodeVisiter;
 
  using Dict = std::map<std::wstring, Node>;
  using Array = std::vector<Node>;
  using Value = std::variant<std::nullptr_t, bool, int, double, std::wstring, Array, Dict>;

  /// @brief Loads Node from json;
  /// @param input wstream of symbols which contains json
  /// @return ready json::node
  Node LoadNode(std::wistream &input);
  
  /// @brief Prints Node
  /// @param node Node which you need to print
  /// @param output wstream where Node must be printed
  void Print(const Node &node, std::wostream &output);
  
  /// @brief Prints the Node
  /// @param os stream
  /// @param nod Node which you need to print
  /// @return & to wstream where Node must be printed
  std::wostream &operator<<(std::wostream &os, const Node &nod);



  ////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                                        //
  //                                                                                        //
  //                                     NODE                                               //
  //                                                                                        //
  //                                                                                        //
  ////////////////////////////////////////////////////////////////////////////////////////////

  class Node : public Value
  {
  public:
    ////////////////////////////////////////////////////////////////////////////////////////////
    //                          СРАВНЕНИЕ ТИПОВ                                               //
    ////////////////////////////////////////////////////////////////////////////////////////////
    
    bool IsInt() const;
    bool IsDouble() const;
    bool IsPureDouble() const;
    bool IsBool() const;
    bool IsWString() const;
    bool IsNull() const;
    bool IsArray() const;
    bool IsMap() const;
    
    bool operator==(const Node &n) const { return this->GetValue() == n.GetValue(); };
    bool operator!=(const Node &n) const { return !(*this == n); };

    ////////////////////////////////////////////////////////////////////////////////////////////
    //                          ВЕРНУТЬ В КАЧ-ВЕ..                                            //
    ////////////////////////////////////////////////////////////////////////////////////////////

    int AsInt() const;
    double AsDouble() const;
    double AsPureDouble() const;
    bool AsBool() const;
    const std::wstring &AsString() const;
    const Array &AsArray() const;
    const Dict &AsMap() const;
    const Value &GetValue() const;

    /*none constant versions*/
    std::wstring &AsString();
    Array &AsArray();
    Dict &AsMap();
    Value &GetValue();

  private:
 
  };

  ////////////////////////////////////////////////////////////////////////////////////////////
  //                                                                                        //
  //                                                                                        //
  //                                     NODE_VISITOR                                       //
  //                                                                                        //
  //                                                                                        //
  ////////////////////////////////////////////////////////////////////////////////////////////
  struct NodeVisiter
  {
    std::wostream &out;
    void operator()(std::nullptr_t) const;
    void operator()(bool v) const;
    void operator()(double v) const;
    void operator()(int v) const;
    void operator()(const std::wstring &v) const;
    void operator()(const Array &v) const;
    void operator()(const Dict &v) const;
  };

 

}
