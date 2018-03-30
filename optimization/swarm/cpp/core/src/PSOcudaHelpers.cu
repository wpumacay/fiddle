
#include <PSOcudaHelpers.h>

using namespace std;

#define MAX_THREADS_PER_BLOCK 1024


__device__ double DF_FunctionSphere( double* d_ppos, int ndim, int pIndx )
{
    double _res = 0.0;

    for ( int d = 0; d < ndim; d++ )
    {
        double _xd = d_ppos[ pIndx * ndim + d ];

        _res += _xd * _xd;
    }

    return _res;
}



__global__ void kernel_initializeRandomGenerator( unsigned int seed, int populationSize, curandState_t* states )
{
    int id = threadIdx.x + blockIdx.x * blockDim.x;

    if ( id > populationSize - 1 )
    {
        // Out of bounds of population buffer, just exit
        return;
    }

    curand_init( seed, id, 0, &states[id] );
}


__global__ void kernel_updateParticles( double* d_ppos, double* d_pvel, double* d_pbpos,
                                        double* d_pcost, double* d_pbcost, 
                                        double* d_gbestpos, double d_gbestcost,
                                        int ndim, int populationSize,
                                        double domainMin, double domainMax,
                                        double vmin, double vmax,
                                        double w, double c1, double c2, double k,
                                        curandState_t* randomStates )
{

    int p = threadIdx.x + blockIdx.x * blockDim.x;

    if ( p > populationSize - 1 )
    {
        // Out of bounds of population buffer, just exit
        return;
    }

    // Update velocity of the particle *******************************
    double _vnorm = 0.0;

    for ( int d = 0; d < ndim; d++ )
    {
        double _v_d = w * d_pvel[ p * ndim + d ] + 
                      c1 * curand_uniform( &randomStates[p] ) * ( d_pbpos[ p * ndim + d ] - d_ppos[ p * ndim + d ] ) +
                      c2 * curand_uniform( &randomStates[p] ) * ( d_gbestpos[ d ] - d_ppos[ p * ndim + d ] );

        d_pvel[ p * ndim + d ] = _v_d;
        _vnorm += _v_d * _v_d;
    }

    _vnorm = sqrt( _vnorm );
    if ( _vnorm > 0.001 )
    {
        double _vClamped = ( _vnorm < vmin ) ? ( vmin ) : ( ( _vnorm > vmax ) ? ( vmax ) : ( _vnorm ) );

        for ( int d = 0; d < ndim; d++ )
        {
            d_pvel[ p * ndim + d ] = ( d_pvel[ p * ndim + d ] / _vnorm ) * _vClamped;
        }
    }

    // Update position of the particle *******************************

    for ( int d = 0; d < ndim; d++ )
    {
        double _x_d = d_ppos[ p * ndim + d ];

        _x_d += d_pvel[ p * ndim + d ];

        _x_d = ( _x_d < domainMin ) ? ( domainMin ) : ( ( _x_d > domainMax ) ? ( domainMax ) : ( _x_d ) );

        d_ppos[ p * ndim + d ] = _x_d;
    }

    // Update cost of the particle ***********************************

    d_pcost[ p ] = DF_FunctionSphere( d_ppos, ndim, p );

    if ( d_pcost[ p ] < d_pbcost[ p ] )
    {
        d_pbcost[ p ] = d_pcost[ p ];
        for ( int d = 0; d < ndim; d++ ) 
        { 
            d_pbpos[ p * ndim + d ] = d_ppos[ p * ndim + d ]; 
        }
    }
}






namespace optimization
{


    void cuPSOcreateParticles( PSOcudaParticlesInfo& devParticlesInfo )
    {
        cudaError_t _errorCode;

        _errorCode = cudaMalloc( ( void** ) &devParticlesInfo.pos, devParticlesInfo.vecBufferSize );
        cout << "errCode - pos: " << _errorCode << endl;

        _errorCode = cudaMalloc( ( void** ) &devParticlesInfo.vel, devParticlesInfo.vecBufferSize );
        cout << "errCode - vel: " << _errorCode << endl;        

        _errorCode = cudaMalloc( ( void** ) &devParticlesInfo.bpos, devParticlesInfo.vecBufferSize );
        cout << "errCode - bpos: " << _errorCode << endl;        

        _errorCode = cudaMalloc( ( void** ) &devParticlesInfo.cost, devParticlesInfo.costBufferSize );
        cout << "errCode - cost: " << _errorCode << endl;        

        _errorCode = cudaMalloc( ( void** ) &devParticlesInfo.bcost, devParticlesInfo.costBufferSize );
        cout << "errCode - bcost: " << _errorCode << endl;        

        _errorCode = cudaMalloc( ( void** ) &devParticlesInfo.gbestpos, devParticlesInfo.vecSize );
        cout << "errCode - gbestpos: " << _errorCode << endl;        

        // Initialize random number generator as well
        _errorCode = cudaMalloc( ( void** ) &devParticlesInfo.devRandStates, sizeof( curandState_t ) * devParticlesInfo.population );
        cout << "errCode - devRandStates: " << _errorCode << endl;        

        int _bx = ceil( ( ( float ) devParticlesInfo.population ) / MAX_THREADS_PER_BLOCK );

        dim3 _blockSize( MAX_THREADS_PER_BLOCK );
        dim3 _gridSize( _bx );

        kernel_initializeRandomGenerator<<< _gridSize, _blockSize >>>( time( 0 ), devParticlesInfo.population, devParticlesInfo.devRandStates );
    }

    void cuPSOinitParticles( PSOcudaParticlesInfo& hostParticlesInfo,
                             PSOcudaParticlesInfo& devParticlesInfo, 
                             BaseFunction* fcn )
    {

        hostParticlesInfo.gbestcost = 1000000.0;

        // Initialize host particles to be sent to device
        for ( int p = 0; p < hostParticlesInfo.population; p++ )
        {
            for ( int d = 0; d < hostParticlesInfo.ndim; d++ )
            {
                hostParticlesInfo.pos[ p * hostParticlesInfo.ndim + d ] = RANDOM( hostParticlesInfo.domainMin, hostParticlesInfo.domainMax );
                hostParticlesInfo.vel[ p * hostParticlesInfo.ndim + d ] = 0.0;
                hostParticlesInfo.bpos[ p * hostParticlesInfo.ndim + d ] = hostParticlesInfo.pos[ p * hostParticlesInfo.ndim + d ];
            }

            hostParticlesInfo.cost[ p ]  = fcn->eval( hostParticlesInfo.pos + p * hostParticlesInfo.ndim, hostParticlesInfo.ndim );
            hostParticlesInfo.bcost[ p ] = hostParticlesInfo.cost[ p ];

            if ( hostParticlesInfo.cost[ p ] < hostParticlesInfo.gbestcost )
            {
                hostParticlesInfo.gbestcost = hostParticlesInfo.cost[ p ];
                for ( int d = 0; d < hostParticlesInfo.ndim; d++ )
                {
                    hostParticlesInfo.gbestpos[ d ] = hostParticlesInfo.pos[ p * hostParticlesInfo.ndim + d ];
                }
            }
        }

        // Pass this info to the device particles
        devParticlesInfo.gbestcost = 1000000.0;

        cudaError_t _errorCode;

        _errorCode = cudaMemcpy( devParticlesInfo.pos, hostParticlesInfo.pos, 
                                 hostParticlesInfo.vecBufferSize, cudaMemcpyHostToDevice );
        cout << "errCode pos: " << _errorCode << endl;        

        _errorCode = cudaMemcpy( devParticlesInfo.vel, hostParticlesInfo.vel, 
                                 hostParticlesInfo.vecBufferSize, cudaMemcpyHostToDevice );
        cout << "errCode vel: " << _errorCode << endl;        

        _errorCode = cudaMemcpy( devParticlesInfo.bpos, hostParticlesInfo.bpos, 
                                 hostParticlesInfo.vecBufferSize, cudaMemcpyHostToDevice );
        cout << "errCode bpos: " << _errorCode << endl;        

        _errorCode = cudaMemcpy( devParticlesInfo.cost, hostParticlesInfo.cost, 
                                 hostParticlesInfo.costBufferSize, cudaMemcpyHostToDevice );
        cout << "errCode cost: " << _errorCode << endl;        

        _errorCode = cudaMemcpy( devParticlesInfo.bcost, hostParticlesInfo.bcost, 
                                 hostParticlesInfo.costBufferSize, cudaMemcpyHostToDevice );
        cout << "errCode bcost: " << _errorCode << endl;        
    }

    void cuPSOupdateParticles( PSOcudaParticlesInfo& hostParticlesInfo,
                               PSOcudaParticlesInfo& devParticlesInfo,
                               double w, double c1, double c2, double k )
    {
        // Copy from host to device - only best cost and best pos
        cudaError_t _errorCode;

        _errorCode = cudaMemcpy( devParticlesInfo.gbestpos, hostParticlesInfo.gbestpos, 
                                 hostParticlesInfo.vecSize, cudaMemcpyHostToDevice );
        cout << "errCode gbestpos: " << _errorCode << endl;       

        devParticlesInfo.gbestcost = hostParticlesInfo.gbestcost;// this one is just a copy in host

        // Launch kernels ******************************************************

        int _bx = ceil( ( ( float ) devParticlesInfo.population ) / MAX_THREADS_PER_BLOCK );

        dim3 _blockSize( MAX_THREADS_PER_BLOCK );
        dim3 _gridSize( _bx );

        kernel_updateParticles<<< _gridSize, _blockSize >>>( devParticlesInfo.pos, devParticlesInfo.vel, devParticlesInfo.bpos,
                                                             devParticlesInfo.cost, devParticlesInfo.bcost,
                                                             devParticlesInfo.gbestpos, devParticlesInfo.gbestcost,
                                                             devParticlesInfo.ndim, devParticlesInfo.population,
                                                             devParticlesInfo.domainMin, devParticlesInfo.domainMax,
                                                             devParticlesInfo.vmin, devParticlesInfo.vmax,
                                                             w, c1, c2, k,
                                                             devParticlesInfo.devRandStates );

        // *********************************************************************


        // Copy back from device to host ***************************************

        _errorCode = cudaMemcpy( hostParticlesInfo.pos, devParticlesInfo.pos, 
                                 hostParticlesInfo.vecBufferSize, cudaMemcpyDeviceToHost );
        cout << "errCode--pos: " << _errorCode << endl;        

        _errorCode = cudaMemcpy( hostParticlesInfo.vel, devParticlesInfo.vel, 
                                 hostParticlesInfo.vecBufferSize, cudaMemcpyDeviceToHost );
        cout << "errCode--vel: " << _errorCode << endl;        

        _errorCode = cudaMemcpy( hostParticlesInfo.bpos, devParticlesInfo.bpos, 
                                 hostParticlesInfo.vecBufferSize, cudaMemcpyDeviceToHost );
        cout << "errCode--bpos: " << _errorCode << endl;        

        _errorCode = cudaMemcpy( hostParticlesInfo.cost, devParticlesInfo.cost, 
                                 hostParticlesInfo.costBufferSize, cudaMemcpyDeviceToHost );
        cout << "errCode--cost: " << _errorCode << endl;        

        _errorCode = cudaMemcpy( hostParticlesInfo.bcost, devParticlesInfo.bcost, 
                                 hostParticlesInfo.costBufferSize, cudaMemcpyDeviceToHost );
        cout << "errCode--bcost: " << _errorCode << endl;       

        // *********************************************************************

        // Do check for new best in here - host ********************************

        int _bestParticleIndx = -1;

        for ( int p = 0; p < hostParticlesInfo.population; p++ )
        {
            if ( hostParticlesInfo.bcost[ p ] < hostParticlesInfo.gbestcost )
            {
                hostParticlesInfo.gbestcost = hostParticlesInfo.bcost[ p ];
                _bestParticleIndx = p;
            }
        }

        if ( _bestParticleIndx != -1 )
        {
            for ( int d = 0; d < hostParticlesInfo.ndim; d++ )
            {
                hostParticlesInfo.gbestpos[ d ] = hostParticlesInfo.bpos[ _bestParticleIndx * hostParticlesInfo.ndim + d ];
            }
        }
    }

    void cuPSOreleaseParticles( PSOcudaParticlesInfo& devParticlesInfo )
    {
        cudaFree( devParticlesInfo.pos );
        cudaFree( devParticlesInfo.vel );
        cudaFree( devParticlesInfo.bpos );
        cudaFree( devParticlesInfo.cost );
        cudaFree( devParticlesInfo.bcost );
        cudaFree( devParticlesInfo.gbestpos );
        cudaFree( devParticlesInfo.devRandStates );
    }




}