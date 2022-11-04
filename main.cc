#include <iostream>
#include <sstream>
#include "expr.h"
#include "parse.h"

int main() {
    std::string line;
    while( getline( std::cin, line ) ) {
        try {
            std::unique_ptr<Expr> exp = parse( line );
            std::cout << exp->prettyPrint() << std::endl;
        }
        catch( no_matching_expression ) {
            std::cerr << "Unable to parse expression" << std::endl;
        }
    }
}
