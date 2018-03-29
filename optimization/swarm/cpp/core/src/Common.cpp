

#include <Common.h>

using namespace std;

namespace optimization
{

    Vec::Vec()
    {
        m_buff = NULL;
        m_ndim = 0;
    }

    Vec::Vec( int ndim )
    {
        m_ndim = ndim;
        m_buff = new double[ m_ndim ];
    }

    Vec::Vec( int ndim, double initVal )
    {
        m_ndim = ndim;
        m_buff = new double[ m_ndim ];

        for ( int q = 0; q < ndim; q++ ) { m_buff[q] = initVal; }
    }
    
    Vec::Vec( const Vec& other )
    {
        m_ndim = other.m_ndim;
        m_buff = new double[ m_ndim ];
    }

    void Vec::operator= ( const Vec& other )
    {
        if ( m_buff == NULL )
        {
            m_ndim = other.m_ndim;
            m_buff = new double[ other.m_ndim ];
        }
        else
        {
            // Assignment should be with same size with non-empty vectors
            assert( other.m_ndim == m_ndim );
        }

        for ( int q = 0; q < m_ndim; q++ ) { m_buff[q] = other.m_buff[q]; }
    }

    Vec::~Vec()
    {
        if ( m_buff != NULL )
        {
            delete[] m_buff;
            m_buff = NULL;
            m_ndim = 0;
        }
    }

    double Vec::operator[] ( int indx ) const
    {
        assert( indx >= 0 && indx < m_ndim );

        return m_buff[ indx ];
    }

    double Vec::get( int indx ) const
    {
        assert( indx >= 0 && indx < m_ndim );

        return m_buff[ indx ];
    }

    string Vec::toString()
    {
        string _res = "[ ";

        for ( int q = 0; q < m_ndim; q++ )
        {
            _res += to_string( m_buff[q] );
            _res += " ";
        }

        _res += "]";

        return _res;
    }

    // Static methods

    double Vec::dot( const Vec& v1, const Vec& v2 )
    {
        assert( v1.ndim() == v2.ndim() );

        int _ndim = v1.ndim();
        double _res = 0.0;

        for ( int q = 0; q < _ndim; q++ )
        {
            _res += v1.m_buff[q] * v2.m_buff[q];
        }

        return _res;
    }

    Vec Vec::randUniform( int ndim, double pMin, double pMax )
    {
        Vec _res( ndim, 0.0 );

        for ( int q = 0; q < ndim; q++ )
        {
            _res.m_buff[q] = RANDOM( pMin, pMax );
        }

        return _res;
    }

    Vec Vec::zeros( int ndim )
    {
        return Vec( ndim, 0.0 );
    }

    Vec Vec::ones( int ndim )
    {
        return Vec( ndim, 1.0 );
    }

    double Vec::norm( const Vec& v )
    {
        return sqrt( Vec::dot( v, v ) );
    }

    void Vec::normalize( Vec& v )
    {
        double _norm = Vec::norm( v );

        for ( int q = 0; q < v.m_ndim; q++ )
        {
            v.m_buff[q] /= _norm;
        }
    }

    void Vec::scale( Vec& v, double scale )
    {
        for ( int q = 0; q < v.m_ndim; q++ )
        {
            v.m_buff[q] *= scale;
        }
    }

    void Vec::clip( Vec& v, double vmin, double vmax )
    {
        for ( int q = 0; q < v.m_ndim; q++ )
        {
            double _val = v.m_buff[q];
            _val = CLAMP( _val, vmin, vmax );
            v.m_buff[q] = _val;
        }
    }

}

optimization::Vec operator+ ( const optimization::Vec& v1, const optimization::Vec& v2 )
{
    assert( v1.ndim() == v2.ndim() );

    int _ndim = v1.ndim();

    optimization::Vec _res( _ndim );

    double* _buff1 = v1.buff();
    double* _buff2 = v2.buff();
    double* _rbuff = _res.buff();

    for ( int q = 0; q < _ndim; q++ )
    {
        _rbuff[q] = _buff1[q] + _buff2[q];
    }

    return _res;
}

optimization::Vec operator- ( const optimization::Vec& v1, const optimization::Vec& v2 )
{
    assert( v1.ndim() == v2.ndim() );

    int _ndim = v1.ndim();

    optimization::Vec _res( _ndim );

    double* _buff1 = v1.buff();
    double* _buff2 = v2.buff();
    double* _rbuff = _res.buff();

    for ( int q = 0; q < _ndim; q++ )
    {
        _rbuff[q] = _buff1[q] - _buff2[q];
    }

    return _res;
}

optimization::Vec operator* ( const optimization::Vec& v1, const optimization::Vec& v2 )
{
    assert( v1.ndim() == v2.ndim() );

    int _ndim = v1.ndim();

    optimization::Vec _res( _ndim );

    double* _buff1 = v1.buff();
    double* _buff2 = v2.buff();
    double* _rbuff = _res.buff();

    for ( int q = 0; q < _ndim; q++ )
    {
        _rbuff[q] = _buff1[q] * _buff2[q];
    }

    return _res;
}

optimization::Vec operator* ( const optimization::Vec& v, double val )
{
    int _ndim = v.ndim();

    optimization::Vec _res( _ndim );

    double* _buff = v.buff();
    double* _rbuff = _res.buff();

    for ( int q = 0; q < _ndim; q++ )
    {
        _rbuff[q] = _buff[q] * val;
    }

    return _res;
}

optimization::Vec operator* ( double val, const optimization::Vec& v )
{
    int _ndim = v.ndim();

    optimization::Vec _res( _ndim );

    double* _buff = v.buff();
    double* _rbuff = _res.buff();

    for ( int q = 0; q < _ndim; q++ )
    {
        _rbuff[q] = _buff[q] * val;
    }

    return _res;
}

optimization::Vec operator/ ( const optimization::Vec& v, double val )
{
    int _ndim = v.ndim();

    optimization::Vec _res( _ndim );

    double* _buff = v.buff();
    double* _rbuff = _res.buff();

    for ( int q = 0; q < _ndim; q++ )
    {
        _rbuff[q] = _buff[q] / val;
    }

    return _res;
}

optimization::Vec operator/ ( double val, const optimization::Vec& v )
{
    int _ndim = v.ndim();

    optimization::Vec _res( _ndim );

    double* _buff = v.buff();
    double* _rbuff = _res.buff();

    for ( int q = 0; q < _ndim; q++ )
    {
        _rbuff[q] = _buff[q] / val;
    }

    return _res;
}
