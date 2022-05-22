OBJS	= DSL_Main.o AST/AST.o AST/Node/Node.o Interpreter/Interpreter.o Lexer/Lexer.o Memory/Memory.o Memory/Operations.o Memory/Class/Class.o Memory/Coin/Coin.o Memory/Coin/Numeric.o Memory/Coin/Object.o Memory/Coin/String.o Memory/Coin/Void.o Memory/Function/Function.o Memory/Pointer/Pointer.o Parser/Parser.o RPN/RPN_Element.o Token/Token.o
SOURCE	= DSL_Main.cpp AST/AST.cpp AST/Node/Node.cpp Interpreter/Interpreter.cpp Lexer/Lexer.cpp Memory/Memory.cpp Memory/Operations.cpp Memory/Class/Class.cpp Memory/Coin/Coin.cpp Memory/Coin/Numeric.cpp Memory/Coin/Object.cpp Memory/Coin/String.cpp Memory/Coin/Void.cpp Memory/Function/Function.cpp Memory/Pointer/Pointer.cpp Parser/Parser.cpp RPN/RPN_Element.cpp Token/Token.cpp
HEADER	= AST/AST.h AST/Node/Node.h Interpreter/Interpreter.h Lexer/Lexer.h Memory/Memory.h Memory/Operations.h Memory/Class/Class.h Memory/Coin/Coin.h Memory/Coin/Numeric.h Memory/Coin/Object.h Memory/Coin/String.h Memory/Coin/Void.h Memory/Function/Function.h Memory/Pointer/Pointer.h Parser/Parser.h RPN/RPN_Element.h Token/Token.h
OUT	= NK_DSL
CC	 = g++
FLAGS	 = -g -c -lboost_regex -O2 -m64 -std=c++2a
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

DSL_Main.o: DSL_Main.cpp
	$(CC) $(FLAGS) DSL_Main.cpp 

AST/AST.o: AST/AST.cpp
	$(CC) $(FLAGS) AST/AST.cpp 

AST/Node/Node.o: AST/Node/Node.cpp
	$(CC) $(FLAGS) AST/Node/Node.cpp 

Interpreter/Interpreter.o: Interpreter/Interpreter.cpp
	$(CC) $(FLAGS) Interpreter/Interpreter.cpp 

Lexer/Lexer.o: Lexer/Lexer.cpp
	$(CC) $(FLAGS) Lexer/Lexer.cpp 

Memory/Memory.o: Memory/Memory.cpp
	$(CC) $(FLAGS) Memory/Memory.cpp 

Memory/Operations.o: Memory/Operations.cpp
	$(CC) $(FLAGS) Memory/Operations.cpp 

Memory/Class/Class.o: Memory/Class/Class.cpp
	$(CC) $(FLAGS) Memory/Class/Class.cpp 

Memory/Coin/Coin.o: Memory/Coin/Coin.cpp
	$(CC) $(FLAGS) Memory/Coin/Coin.cpp 

Memory/Coin/Numeric.o: Memory/Coin/Numeric.cpp
	$(CC) $(FLAGS) Memory/Coin/Numeric.cpp 

Memory/Coin/Object.o: Memory/Coin/Object.cpp
	$(CC) $(FLAGS) Memory/Coin/Object.cpp 

Memory/Coin/String.o: Memory/Coin/String.cpp
	$(CC) $(FLAGS) Memory/Coin/String.cpp 

Memory/Coin/Void.o: Memory/Coin/Void.cpp
	$(CC) $(FLAGS) Memory/Coin/Void.cpp 

Memory/Function/Function.o: Memory/Function/Function.cpp
	$(CC) $(FLAGS) Memory/Function/Function.cpp 

Memory/Pointer/Pointer.o: Memory/Pointer/Pointer.cpp
	$(CC) $(FLAGS) Memory/Pointer/Pointer.cpp 

Parser/Parser.o: Parser/Parser.cpp
	$(CC) $(FLAGS) Parser/Parser.cpp 

RPN/RPN_Element.o: RPN/RPN_Element.cpp
	$(CC) $(FLAGS) RPN/RPN_Element.cpp 

Token/Token.o: Token/Token.cpp
	$(CC) $(FLAGS) Token/Token.cpp 


clean:
	rm -f $(OBJS) $(OUT)
