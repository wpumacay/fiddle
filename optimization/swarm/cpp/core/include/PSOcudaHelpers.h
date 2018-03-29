
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

        int vecBufferSize;
        int costBufferSize;

        // Arrays of size "ndim * population"
        double* pos;
        double* vel;
        double* bpos;
        // Arrays of size "population"
        double* cost;
        double* bcost;
    };

    void cuPSOcreateParticles( PSOcudaParticlesInfo& devParticlesInfo );

    void cuPSOinitParticles( PSOcudaParticlesInfo& hostParticlesInfo,
                             PSOcudaParticlesInfo& devParticlesInfo,
                             double domainMin, double domainMax, BaseFunction* fcn );

    void cuPSOupdateParticles( PSOcudaParticlesInfo& hostParticlesInfo,
                               PSOcudaParticlesInfo& devParticlesInfo );

    void cuPSOreleaseParticles( PSOcudaParticlesInfo& devParticlesInfo );
}
