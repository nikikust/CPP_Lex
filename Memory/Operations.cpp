#include "Operations.h"


std::string convertOpToType(int op)	// based on "int Node::getRang()" in "Node.cpp"
{
	if (op > 1 && op <= 3)
		return "unar_op";
	else if (op <= 5)
		return "binar_op__double";
	else if (op <= 6)
		return "binar_op__int_int";
	else if (op <= 8)
		return "binar_op__bool";
	else if (op <= 11)
		return "binar_op__int";
	else if (op <= 13)
		return "binar_op__double";
	else if (op == 15)
		return "binar_op__assign";
	else
		return "err_op";
}
