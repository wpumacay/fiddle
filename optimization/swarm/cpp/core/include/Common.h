
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>

#include <pthread.h>

using namespace std;

#define _RANDOM( a, b ) ( a + ( b - a ) * ( rand() / ( float )RAND_MAX ) )

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

		double operator[] ( int indx );

		int ndim() { return m_ndim; }

		static double dot( const Vec& v1, const Vec& v2 );
		static Vec randUniform( int ndim, double pMin, double pMax );
		static Vec zeros( int ndim );
		static Vec ones( int ndim );
	};






}