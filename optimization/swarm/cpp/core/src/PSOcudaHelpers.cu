
#include <PSOcudaHelpers.h>

using namespace std;

namespace optimization
{


    void cuPSOcreateParticles( PSOcudaParticlesInfo& devParticlesInfo )
    {
        cudaError_t _errorCode;

        _errorCode = cudaMalloc( ( void** ) &devParticlesInfo.pos, devParticlesInfo.vecBufferSize );
        cout << "errCode: " << _errorCode << endl;

        _errorCode = cudaMalloc( ( void** ) &devParticlesInfo.vel, devParticlesInfo.vecBufferSize );
        cout << "errCode: " << _errorCode << endl;        

        _errorCode = cudaMalloc( ( void** ) &devParticlesInfo.bpos, devParticlesInfo.vecBufferSize );
        cout << "errCode: " << _errorCode << endl;        

        _errorCode = cudaMalloc( ( void** ) &devParticlesInfo.cost, devParticlesInfo.costBufferSize );
        cout << "errCode: " << _errorCode << endl;        

        _errorCode = cudaMalloc( ( void** ) &devParticlesInfo.bcost, devParticlesInfo.costBufferSize );
        cout << "errCode: " << _errorCode << endl;        
    }

    void cuPSOinitParticles( PSOcudaParticlesInfo& hostParticlesInfo,
                             PSOcudaParticlesInfo& devParticlesInfo,
                             double domainMin, double domainMax, BaseFunction* fcn )
    {
        // Initialize host particles to be sent to device
        for ( int p = 0; p < hostParticlesInfo.population; p++ )
        {
            for ( int d = 0; d < hostParticlesInfo.ndim; d++ )
            {
                hostParticlesInfo.pos[ p * hostParticlesInfo.ndim + d ] = RANDOM( domainMin, domainMax );
                hostParticlesInfo.vel[ p * hostParticlesInfo.ndim + d ] = 0.0;
                hostParticlesInfo.bpos[ p * hostParticlesInfo.ndim + d ] = hostParticlesInfo.pos[ p * hostParticlesInfo.ndim + d ];
            }

            hostParticlesInfo.cost[ p ]  = fcn->eval( hostParticlesInfo.pos + p * hostParticlesInfo.ndim, d );
            hostParticlesInfo.bcost[ p ] = hostParticlesInfo.cost[ p ];
        }

        // Pass this info to the device particles
        cudaError_t _errorCode;

        _errorCode = cudaMemcpy( devParticlesInfo.pos, hostParticlesInfo.pos, 
                                 hostParticlesInfo.vecBufferSize, cudaMemcpyHostToDevice );
        cout << "errCode: " << _errorCode << endl;        

        _errorCode = cudaMemcpy( devParticlesInfo.vel, hostParticlesInfo.vel, 
                                 hostParticlesInfo.vecBufferSize, cudaMemcpyHostToDevice );
        cout << "errCode: " << _errorCode << endl;        

        _errorCode = cudaMemcpy( devParticlesInfo.bpos, hostParticlesInfo.bpos, 
                                 hostParticlesInfo.vecBufferSize, cudaMemcpyHostToDevice );
        cout << "errCode: " << _errorCode << endl;        

        _errorCode = cudaMemcpy( devParticlesInfo.cost, hostParticlesInfo.cost, 
                                 hostParticlesInfo.costBufferSize, cudaMemcpyHostToDevice );
        cout << "errCode: " << _errorCode << endl;        

        _errorCode = cudaMemcpy( devParticlesInfo.bcost, hostParticlesInfo.bcost, 
                                 hostParticlesInfo.costBufferSize, cudaMemcpyHostToDevice );
        cout << "errCode: " << _errorCode << endl;        
    }

    void cuPSOupdateParticles( PSOcudaParticlesInfo& hostParticlesInfo,
                               PSOcudaParticlesInfo& devParticlesInfo )
    {
        // Copy from host to device


        // Launch kernels


        // Copy back from device to host



    }

    void cuPSOreleaseParticles( PSOcudaParticlesInfo& devParticlesInfo )
    {

    }




}