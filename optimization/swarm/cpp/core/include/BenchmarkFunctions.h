
#pragma once

#include <Common.h>

using namespace std;

namespace optimization
{


	struct BaseFunction
	{
		virtual double operator() ( const Vec& vec ) = 0;
	};

	




}