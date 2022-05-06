all:
	g++ *.cpp				\
	AST/*.cpp				\
	AST/Node/*.cpp			\
	Lexer/*.cpp				\
	Memory/*.cpp			\
	Memory/Class/*.cpp		\
	Memory/Coin/*.cpp		\
	Memory/Function/*.cpp	\
	Memory/Pointer/*.cpp	\
	Parser/*.cpp			\
	RPN/*.cpp				\
	Token/*.cpp				\
	-lboost_regex -O2 -m64 -std=c++2a

