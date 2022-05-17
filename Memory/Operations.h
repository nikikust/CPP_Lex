#pragma once

#include <unordered_map>
#include <string>


static std::unordered_map<std::string,	// choose 1st operand type
	   std::unordered_map<std::string,	// possible operation
	   std::unordered_map<std::string,	// possible 2nd operand type
	   std::string>>>					// return type	
	   BinarOperations = 
{
	{ "int", 
		{
			{ "binar_op__double", 
				{
					{ "int", "int" },
					{ "double", "double" },
					{ "bool", "int" }
				}
			},
			{ "binar_op__int",
				{
					{ "int", "int" },
					{ "bool", "int"}
				}
			},
			{ "binar_op__int_int",
				{
					{ "int", "int" },
					{ "bool", "int"}
				}
			},
			{ "binar_op__bool",
				{
					{ "int", "bool" },
					{ "double", "bool" },
					{ "bool", "bool" }
				}
			}
		}
	},
	{ "double",
		{
			{ "binar_op__double",
				{
					{ "int", "double" },
					{ "double", "double" },
					{ "bool", "double" }
				}
			},
			{ "binar_op__bool",
				{
					{ "int", "bool" },
					{ "double", "bool" },
					{ "bool", "bool" }
				}
			}
		}
	},
	{ "bool",
		{
			{ "binar_op__double",
				{
					{ "int", "int" },
					{ "double", "double" },
					{ "bool", "int" }
				}
			},
			{ "binar_op__int",
				{
					{ "int", "int" },
					{ "bool", "bool"}
				}
			},
			{ "binar_op__bool",
				{
					{ "int", "bool" },
					{ "double", "bool" },
					{ "bool", "bool" }
				}
			}
		}
	},
	{ "string", 
		{
			{ "+",
				{
					{ "int", "string" },
					{ "double", "string" },
					{ "bool", "string" },
					{ "string", "string" }
				}
			},
			{ "*",
				{
					{ "int", "string" },
					{ "bool", "string" }
				}
			},
			{ "binar_op__bool",
				{
					{ "string", "bool" }
				}
			}
		} 
	},
	{ "ptr",
		{
			{ "==",
				{
					{ "ptr", "bool" }
				}
			},
			{ "!=",
				{
					{ "ptr", "bool" }
				}
			}
		}
	}
};

std::string convertOpToType(int op);	// based on "int Node::getRang()" in "Node.cpp"