
#include <OptimizerPSOcuda.h>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

int main( int argc, char** argv )
{
    cout << "argc: " << argc << endl;
    cout << "argv[0]: " << argv[0] << endl;
    cout << "argv[1]: " << argv[1] << endl;

    if ( argc < 2 )
    {
        cout << "usage: ./testPSO_2 functionName" << endl;
        return 0;
    }

    string _saveFileName( "results_iterations_" );
    _saveFileName += string( argv[1] );
    _saveFileName += ".txt";

    // ofstream _fhandle( _saveFileName.c_str(), std::ofstream::out | std::ofstream::app );
    ofstream _fhandle( _saveFileName.c_str() );

    // optimization::OptimizerPSOcuda _optimizer( 8, -100.0, 100.0, 100, 1000000,
    //                                            0.5, 2.0, 2.0, 0.5 );

    // _optimizer.setObjectiveFunction( new optimization::FunctionSphere(), true );

    // optimization::OptimizerPSOcuda _optimizer( 2, -32.0, 32.0, 100, 100000,
    //                                            0.5, 2.0, 2.0, 0.5 );

    // _optimizer.setObjectiveFunction( new optimization::FunctionAckley(), true );

    // optimization::OptimizerPSOcuda _optimizer( 2, -500.0, 500.0, 100, 200000,
    //                                            1.0, 2.0, 2.0, 0.75 );

    // _optimizer.setObjectiveFunction( new optimization::FunctionSchwefel(), true );

    optimization::OptimizerPSOcuda _optimizer( 8, -100.0, 100.0, 100, 200000,
                                               1.0, 2.0, 2.0, 0.75 );

    _optimizer.setObjectiveFunction( new optimization::FunctionShafferFcn6(), false );

    _optimizer.start();

    while( !_optimizer.stopCondition() )
    {
        _optimizer.step();
        _fhandle << to_string( _optimizer.bestCost() ) << endl;
    }
    // _fhandle << to_string( _optimizer.bestCost() ) << endl;

    _optimizer.end();

    _fhandle.close();

    return 0;
}