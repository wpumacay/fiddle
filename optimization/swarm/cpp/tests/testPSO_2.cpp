
#include <OptimizerPSOcuda.h>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

int main( int argc, char** argv )
{
    if ( argc < 2 )
    {
        cout << "usage: ./testPSO_2 functionName" << endl;
        cout << "functionName: Sphere, Ackley, Schwefel, SchafferFcn6" << endl;
        return 0;
    }

    string _fnName( argv[1] );

    if ( _fnName != "Sphere" && _fnName != "Ackley" && 
         _fnName != "Schwefel" && _fnName != "SchafferFcn6" )
    {
        cout << "functionName should be one of: Sphere, Ackley, Schwefel, SchafferFcn6" << endl;
        return 0;
    }

    string _saveFileName( "results_iterations_" );
    _saveFileName += string( argv[1] );
    _saveFileName += ".txt";

    // ofstream _fhandle( _saveFileName.c_str(), std::ofstream::out | std::ofstream::app );
    ofstream _fhandle( _saveFileName.c_str() );

    optimization::OptimizerPSOcuda* _optimizer = NULL;

    if ( _fnName == "Sphere" )
    {
        _optimizer = new optimization::OptimizerPSOcuda( 8, -100.0, 100.0, 100, 100000,
                                                         0.5, 2.0, 2.0, 0.5 );

        _optimizer->setObjectiveFunction( new optimization::FunctionSphere(), true );
    }
    else if ( _fnName == "Ackley" )
    {
        _optimizer = new optimization::OptimizerPSOcuda( 8, -32.0, 32.0, 100, 100000,
                                                         0.5, 2.0, 2.0, 0.5 );

        _optimizer->setObjectiveFunction( new optimization::FunctionAckley(), true );
    }
    else if ( _fnName == "Schwefel" )
    {
        _optimizer = new optimization::OptimizerPSOcuda( 8, -500.0, 500.0, 100, 100000,
                                                         1.0, 2.0, 2.0, 0.75 );

        _optimizer->setObjectiveFunction( new optimization::FunctionSchwefel(), true );
    }
    else if ( _fnName == "SchafferFcn6" )
    {
        _optimizer = new optimization::OptimizerPSOcuda( 8, -100.0, 100.0, 100, 100000,
                                                         1.0, 2.0, 2.0, 0.75 );

        _optimizer->setObjectiveFunction( new optimization::FunctionShafferFcn6(), false );
    }

    _optimizer->start();

    while( !_optimizer->stopCondition() )
    {
        _optimizer->step();
        _fhandle << to_string( _optimizer->bestCost() ) << endl;
    }

    _optimizer->end();

    _fhandle.close();

    return 0;
}