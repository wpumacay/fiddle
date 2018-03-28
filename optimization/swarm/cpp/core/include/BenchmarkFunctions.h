
#pragma once

#include <Common.h>

using namespace std;

namespace optimization
{


	struct BaseFunction
	{
        virtual double eval( const Vec& vec ) = 0;
	};

	struct FunctionSphere
    {
        double eval ( const Vec& vec ) override;
    };




}