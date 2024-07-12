#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Lexer/Lexer.hpp"
#include "FirstParser/FirstParser.hpp"
#include "SecondParser/SecondParser.hpp"
#include "SecondParser/Parsers.cpp"

void printError(LexerExeption e)
{
    std::cout << red << "Error: " << reset << e.what() << " at line " << e.line + 1 << ":" << e.column + 1 <<std::endl;
}

void printError(ParserExeption e)
{
    std::cout << red << "Error: " << reset << e.what() << " at line " << e.line + 1 << std::endl;
}

std::string typeToString(Expression::Type type)
{
    switch (type)
    {
    case Expression::Type::Bool: return "bool";
    case Expression::Type::Char: return "char";
    case Expression::Type::Short: return "short";
    case Expression::Type::Int: return "int";
    case Expression::Type::Long: return "long";
    case Expression::Type::Float: return "float";
    case Expression::Type::Double: return "double";
    case Expression::Type::Void: return "void";
    }
    return "";
}

std::vector<std::string> printNode(Node* node)
{
    std::vector<std::string> result;
    if (dynamic_cast<VariableDec*>(node) != nullptr)
    {
        result.push_back("VariableDec " + getBlue((dynamic_cast<VariableDec*>(node))->name));
    }
    else if (dynamic_cast<Variable*>(node) != nullptr)
    {
        result.push_back("Variable " + getBlue((dynamic_cast<Variable*>(node))->name));
    }
    else if (dynamic_cast<LiteralConst*>(node) != nullptr)
    {
        result.push_back("LiteralConst");
    }
    else if (dynamic_cast<Operator*>(node) != nullptr)
    {
        result.push_back("Operator");
    }
    else if (dynamic_cast<Convertion*>(node) != nullptr)
    {
        result.push_back("Convertion");
    }
    else if (dynamic_cast<While*>(node) != nullptr)
    {
        result.push_back("While");
    }
    else if (dynamic_cast<DoWhile*>(node) != nullptr)
    {
        result.push_back("DoWhile");
    }
    else if (dynamic_cast<For*>(node) != nullptr)
    {
        result.push_back("For");
    }
    else if (dynamic_cast<Nope*>(node) != nullptr)
    {
        result.push_back("Nope");
    }
    else if (dynamic_cast<If*>(node) != nullptr)
    {
        result.push_back("If");
    }
    else if (dynamic_cast<Assignment*>(node) != nullptr)
    {
        result.push_back("Assignment to " + getBlue((dynamic_cast<Assignment*>(node))->variable->name));
    }
    else
    {
        result.push_back("Node");
    }
    if (dynamic_cast<Expression*>(node) != nullptr)
    {
        result.back() += " (" + getBlue(typeToString(dynamic_cast<Expression*>(node)->type)) + ")";
    }
    
    for (auto i : node->children)
    {
        std::vector<std::string> tmp = printNode(i);
        for (auto &&j : tmp)
        {
            j.insert(0, "    ");
            result.push_back(j);
        }
    }
    return result;
}

int main()
{
    std::ifstream file;
    file.open("test.aur");
    std::string code = "";
    std::string line;
    if (!file.is_open())
    {
        std::cout << red << "Error: File not found" << reset << std::endl;
        return 1;
    }
    while (std::getline(file, line))
    {
        code += line + "\n";
    }
    file.close();

    Lexer::Tokanized tokens;

    std::cout << "Lexing..." << std::endl;
    
    Lexer lexer(code);
    try
    {
        tokens = lexer.getTokens();
    }
    catch(const LexerExeption& e)
    {
        printError(e);
        return 1;
    }

    FirstParser fp(tokens);

    std::cout << "Parsing one..." << std::endl;

    try
    {
        fp.parseTokens();
    }
    catch(const ParserExeption& e)
    {
        printError(e);
        return 1;
    }

    SecondParser sp(fp.getRoot());

    std::vector<Node*> result;

    std::cout << "Parsing two..." << std::endl;
    
    try
    {
        std::vector<FirstParser::Node> nodes;
        for (auto i : fp.getRoot().children) nodes.push_back(i);
        result = sp.parse(nodes);
    }
    catch(const ParserExeption& e)
    {
        printError(e);
        return 1;
    }

    std::cout << "AST:\n" << std::endl;

    for (auto i : result)
    {
        std::vector<std::string> tmp = printNode(i);
        for (auto &&j : tmp)
        {
            std::cout << j << std::endl;
        }
    }

    return 0;
}
