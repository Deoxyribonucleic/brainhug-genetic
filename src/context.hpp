#pragma once

#include "types.hpp"

namespace bh
{
	struct context final
	{
		bh::stack stack;
		instruction const* program;
	};
}

