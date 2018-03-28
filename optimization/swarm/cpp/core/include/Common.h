
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

namespace optimization
{


	class Vec
	{

		private :

		double* m_buff;
		int m_size;

		public :

		Vec();
		Vec( int size );
		Vec( int size, double initVal );
		Vec( const Vec& other );
		void operator= ( const Vec& other );

		~Vec();
	};






}