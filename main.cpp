#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "debugprinter.h"

using namespace std;

int main(int argc, char* argv[])
{
    bool testcase = false;
    std::string src_filename = "first_test.txt";
    if (argc > 1)
    {
        if (std::string(argv[1]) == "--testcase")
        {
            testcase = true;
            src_filename = std::string(argv[2]);
        }
    }

    ifstream src(src_filename);

    if (src.good())
    {
        std::string cont;
        std::stringstream buffer;
        buffer<<src.rdbuf();
        cont = buffer.str()+"\n";
        src.close();
        Lexer lexer(&cont);
        Parser parser;

        std::stringstream output;

        Token t;
        do
        {
            t = lexer.next();
            if (!testcase)
            {
                cout<<"Line "<<t.line_num<<": "<<token_debug_names.at(t.type);
                if (t.type == TOKEN_IDENTIFIER || t.type == TOKEN_STR_LITERAL)
                    cout<<": "<<*t.str_val<<endl;
                if (t.type == TOKEN_INT_LITERAL)
                    cout<<": "<<*t.int_val<<endl;
                if (t.type != TOKEN_IDENTIFIER && t.type != TOKEN_STR_LITERAL && t.type != TOKEN_INT_LITERAL)
                    cout<<endl;
            }
            parser.feed(t);

        } while (t.type != TOKEN_EOF);

        Library kek = parser.finish();

        DebugPrinter::print_debug_library(kek, testcase, 0);
    }
    return 0;
}
