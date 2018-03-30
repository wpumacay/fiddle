
#pragma once

#include <Common.h>

using namespace std;

#define OPT_FUNCTION_SPHERE 0
#define OPT_FUNCTION_ACKLEY 1
#define OPT_FUNCTION_SCHWEFEL 2
#define OPT_FUNCTION_SCHAFFER6 3

namespace optimization
{

	struct BaseFunction
	{
        int id;
        double eval( double* vec, int dim );
        virtual double eval( const Vec& vec ) = 0;
	};

	struct FunctionSphere : public BaseFunction
    {
        FunctionSphere()
        {
            this->id = OPT_FUNCTION_SPHERE;
        }

        double eval ( const Vec& vec ) override;
    };

    struct FunctionAckley : public BaseFunction
    {
        FunctionAckley()
        {
            this->id = OPT_FUNCTION_ACKLEY;
        }

        double eval ( const Vec& vec ) override;
    };

    struct FunctionSchwefel : public BaseFunction
    {
        FunctionSchwefel()
        {
            this->id = OPT_FUNCTION_SCHWEFEL;
        }

        double eval ( const Vec& vec ) override;
    };

    struct FunctionShafferFcn6 : public BaseFunction
    {
        FunctionShafferFcn6()
        {
            this->id = OPT_FUNCTION_SCHAFFER6;
        }
        
        double eval ( const Vec& vec ) override;
    };
}