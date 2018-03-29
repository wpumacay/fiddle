
#pragma once

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

        // Arrays of size "ndim * population"
        double* pos;
        double* vel;
        double* bpos;
        // Arrays of size "population"
        double* cost;
        double* bcost;
    };

    void cuPSOupdateParticles( const PSOcudaParticlesInfo& hostParticlesInfo,
                               const PSOcudaParticlesInfo& devParticlesInfo );

    

}
