
#include <OptimizerPSOcuda.h>

using namespace std;

int main()
{
    // optimization::OptimizerPSOcuda _optimizer( 2, -100.0, 100.0, 1000, 100000,
    //                                            0.5, 2.0, 2.0, 0.5 );

    // _optimizer.setObjectiveFunction( new optimization::FunctionSphere(), true );

    // optimization::OptimizerPSOcuda _optimizer( 2, -32.0, 32.0, 100, 200,
    //                                            0.5, 2.0, 2.0, 0.5 );

    // _optimizer.setObjectiveFunction( new optimization::FunctionAckley(), true );

    optimization::OptimizerPSOcuda _optimizer( 8, -500.0, 500.0, 1000, 1000000,
                                               1.0, 2.0, 2.0, 0.75 );

    _optimizer.setObjectiveFunction( new optimization::FunctionSchwefel(), true );

    // optimization::OptimizerPSOcuda _optimizer( 8, -100.0, 100.0, 1000, 100000,
    //                                            1.0, 2.0, 2.0, 0.75 );

    // _optimizer.setObjectiveFunction( new optimization::FunctionShafferFcn6(), false );

    _optimizer.start();

    while( !_optimizer.stopCondition() )
    {
        _optimizer.step();
    }

    _optimizer.end();

    return 0;
}