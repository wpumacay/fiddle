
#pragma once

#include <Common.h>

using namespace std;

namespace optimization
{


	struct BaseFunction
	{
        double eval( double* vec, int dim );
        virtual double eval( const Vec& vec ) = 0;
	};

	struct FunctionSphere : public BaseFunction
    {
        double eval ( const Vec& vec ) override;
    };

    struct FunctionAckley : public BaseFunction
    {
        double eval ( const Vec& vec ) override;
    };

    struct FunctionSchwefel : public BaseFunction
    {
        double eval ( const Vec& vec ) override;
    };

    struct FunctionShafferFcn6 : public BaseFunction
    {
        double eval ( const Vec& vec ) override;
    };
}