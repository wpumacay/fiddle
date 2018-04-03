
#pragma once

#include <cmath>
#include <ctime>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <cassert>

#include <pthread.h>

using namespace std;

#define RANDOM( a, b ) ( a + ( b - a ) * ( rand() / ( float )RAND_MAX ) )
#define RAND() ( ( rand() / ( float )RAND_MAX ) )
#define CLAMP( x, a, b ) ( ( x < a ) ? a : ( ( x > b ) ? b : x ) )

#define _PI 3.141592653589793
#define _E 2.718281828459045

namespace optimization
{


	class Vec
	{

		private :

		double* m_buff;
		int m_ndim;

		public :

		Vec();
		Vec( int ndim );
		Vec( int ndim, double initVal );
		Vec( const Vec& other );
		void operator= ( const Vec& other );

		~Vec();

		int ndim() const { return m_ndim; }
		double* buff() const { return m_buff; }

		double operator[] ( int indx ) const;
		double get( int indx ) const;
		void set( int indx, double val );
		string toString() const;

		static double dot( const Vec& v1, const Vec& v2 );
		static Vec randUniform( int ndim, double pMin, double pMax );
		static Vec zeros( int ndim );
		static Vec ones( int ndim );
		static double norm( const Vec& v );
		static void normalize( Vec& v );
		static void scale( Vec& v, double scale );
		static void clip( Vec& v, double vmin, double vmax );
	};

}


optimization::Vec operator+ ( const optimization::Vec& v1, const optimization::Vec& v2 );
optimization::Vec operator- ( const optimization::Vec& v1, const optimization::Vec& v2 );
optimization::Vec operator* ( const optimization::Vec& v1, const optimization::Vec& v2 );
optimization::Vec operator* ( const optimization::Vec& v, double val );
optimization::Vec operator* ( double val, const optimization::Vec& v );
optimization::Vec operator/ ( const optimization::Vec& v, double val );
optimization::Vec operator/ ( double val, const optimization::Vec& v );