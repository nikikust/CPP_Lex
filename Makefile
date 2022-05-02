all:
    g++ *.cpp AST/*.cpp AST/Node/*.cpp Lexer/*.cpp Parser/*.cpp Token/*.cpp VarTable/*.cpp VarTable/Coin/*.cpp RPN/*.cpp -lboost_regex -O2 -std=c++2a