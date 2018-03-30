
#pragma once

#include <Common.h>
#include <BenchmarkFunctions.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <curand.h>
#include <curand_kernel.h>

namespace optimization
{

    struct PSOcudaParticlesInfo
    {
        int ndim;
        int population;

        int vecSize;
        int vecBufferSize;
        int costBufferSize;

        // Arrays of size "ndim * population"
        double* pos;
        double* vel;
        double* bpos;
        // Arrays of size "population"
        double* cost;
        double* bcost;
        // Global best pos
        double* gbestpos;
        
        // Global best cost
        double gbestcost;

        // cuda random states handler
        curandState_t* devRandStates;

        // space constraints
        double domainMin;
        double domainMax;
        // velocity constraints
        double vmin;
        double vmax;
    };

    void cuPSOcreateParticles( PSOcudaParticlesInfo& devParticlesInfo );

    void cuPSOinitParticles( PSOcudaParticlesInfo& hostParticlesInfo,
                             PSOcudaParticlesInfo& devParticlesInfo, 
                             BaseFunction* fcn );

    void cuPSOupdateParticles( PSOcudaParticlesInfo& hostParticlesInfo,
                               PSOcudaParticlesInfo& devParticlesInfo,
                               double w, double c1, double c2, double k );

    void cuPSOreleaseParticles( PSOcudaParticlesInfo& devParticlesInfo );
}
