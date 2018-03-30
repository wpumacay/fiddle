
#pragma once

#include <OptimizerInterface.h>
#include <PSOcudaHelpers.h>

using namespace std;

namespace optimization
{

    class OptimizerPSOcuda : public OptimizerInterface
    {

        private :

        int m_populationSize;

        PSOcudaParticlesInfo m_particlesHostInfo;
        PSOcudaParticlesInfo m_particlesDeviceInfo;

        Vec m_bestPos;
        double m_bestCost;

        Vec m_avgPosition;

        double m_w;
        double m_c1;
        double m_c2;
        double m_k;

        double m_vmin;
        double m_vmax;

        public :

        OptimizerPSOcuda( int pNdim, 
                          double pDomainMin, 
                          double pDomainMax, 
                          int pMaxIter, 
                          int pPopulationSize,
                          double w, double c1, double c2, double k );
        ~OptimizerPSOcuda();

        void start() override;
        void step() override;
        void end() override;

        // Accessors

        Vec bestPos() { return m_bestPos; }
        double bestCost() { return m_bestCost; }
        Vec avgPosition() { return m_avgPosition; }
    };

}