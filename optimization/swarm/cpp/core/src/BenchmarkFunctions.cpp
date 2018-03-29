
#include <BenchmarkFunctions.h>

using namespace std;


namespace optimization
{
    double BaseFunction::eval( double* vec, int dim )
    {
        Vec _v( dim );
        double* _vBuff = _v.buff();

        memcpy( _vBuff, vec, sizeof( double ) * dim );

        return this->eval( _v );
    }

    double FunctionSphere::eval( const Vec& vec )
    {
        return Vec::dot( vec, vec );
    }
    
    double FunctionAckley::eval( const Vec& vec )
    {
        // a = 20, b = 0.2, c = 2.0 * pi

        int _ndim = vec.ndim();

        double _t1 = -20.0 * exp( -0.2 * sqrt( Vec::dot( vec, vec ) / _ndim ) );
        double _sumcos = 0.0;

        for ( int q = 0; q < _ndim; q++ )
        {
            _sumcos += cos( 2.0 * _PI * vec[q] );
        }

        double _t2 = -exp( _sumcos / _ndim );
        double _t3 = 20.0 + _E;

        return _t1 + _t2 + _t3;
    }

    double FunctionSchwefel::eval( const Vec& vec )
    {
        int _ndim = vec.ndim();

        double _res = 418.9829 * _ndim;

        for ( int q = 0; q < _ndim; q++ )
        {
            _res -= vec[q] * sin( sqrt( abs( vec[q] ) ) );
        }

        return _res;
    }

    double FunctionShafferFcn6::eval( const Vec& vec )
    {
        double _sumsquares = Vec::dot( vec, vec );

        double _tmp1 = sin( sqrt( _sumsquares ) );
        double _numerator = _tmp1 * _tmp1 - 0.5;

        double _tmp2 = 1.0 + 0.001 * _sumsquares;
        double _denominator = _tmp2 * _tmp2;

        return 0.5 - _numerator / _denominator;
    }
}