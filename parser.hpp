#ifndef YAML_PARSER_H
#define YAML_PARSER_H
#include "lexer.hpp"
#include "data.hpp"

using namespace YamlLexer;
using namespace YamlData;

namespace YamlParser
{

struct parser
{
    yaml_lexer *ylexer = nullptr;
    struct token *ytoken = nullptr;

    parser(cpint8 filename)
    {
        ylexer = new yaml_lexer(filename);
        ytoken = ylexer->return_token();
    }

    template<typename T>
        requires std::is_same<T, struct parser>::value
    void delete_instance(T *instance)
    {
        if(instance)
            delete instance;
        instance = nullptr;
    }

    ~parser()
    {
        if(ylexer)
        {
            ylexer->delete_instance<struct token> (ytoken);
            ylexer->delete_instance<yaml_lexer> (ylexer);
        }
        ylexer = nullptr;
    }
};

class yaml_parser
{
private:
    struct parser *yparser = nullptr;
    yaml_data *ydata = nullptr;

    inline void get_token_from_lexer()
    {
        yparser->ylexer->get_token();
        yparser->ytoken = yparser->ylexer->return_token();
    }

    void parse_yaml()
    {
        reloop:
        switch(yparser->ytoken->id)
        {
            case YamlTokens::user_def: {
                puint8 user_defined_var = yparser->ytoken->token_value;
                
                get_token_from_lexer();
                get_token_from_lexer();

                switch(yparser->ytoken->id)
                {
                    case YamlTokens::hex: {
                        ydata->add_yaml_data(user_defined_var, (puint16) yparser->ytoken->token_value, data_types::Hex);
                        delete user_defined_var;
                        break;
                    }
                    case YamlTokens::number: {
                        break;
                    }
                    case YamlTokens::string: {
                        break;
                    }
                    case YamlTokens::character: {
                        break;
                    }
                    default: break;
                }
                break;
            }
            default: break;
        }
    }

public:
    yaml_parser(cpint8 filename)
    {
        yparser = new struct parser(filename);
        ydata = new yaml_data;

        /* Get the first token. */
        get_token_from_lexer();
        parse_yaml();
    }

    template<typename T>
        requires std::is_same<T, struct parser>::value
            || std::is_same<T, yaml_parser>::value
    void delete_instance(T *instance)
    {
        if(instance)
            delete instance;
        instance = nullptr;
    }

    ~yaml_parser()
    {
        if(yparser) yparser->delete_instance<struct parser> (yparser);
        yparser = nullptr;

        if(ydata) ydata->delete_instance<yaml_data> (ydata);
        ydata = nullptr;

        std::cout << "[DEBUG -> PARSER]\tDeleted `yaml_parser` instance." << std::endl;
    }
};

}

#endif
