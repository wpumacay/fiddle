
#pragma once

#include <OptimizerInterface.h>

using namespace std;


namespace optimization
{

    struct PSOparticle
    {
        Vec pos;
        Vec vel;
        double cost;

        Vec bestPos;
        double bestCost;

        PSOparticle()
        {
            this->cost = 0.0;
            this->bestCost = 0.0;
        }

        PSOparticle( int ndim )
        {
            this->pos = Vec::zeros( ndim );
            this->vel = Vec::zeros( ndim );
            this->cost = 0.0;

            this->bestPos = Vec::zeros( ndim );
            this->bestCost = 0.0;
        }
    };



    class OptimizerPSO : public OptimizerInterface
    {

        private :

        int m_populationSize;

        vector< PSOparticle > m_particles;
        PSOparticle m_bestParticle;

        Vec m_avgPosition;

        double m_w;
        double m_c1;
        double m_c2;
        double m_k;

        double m_vmin;
        double m_vmax;

        public :

        OptimizerPSO( int pNdim, 
                      double pDomainMin, 
                      double pDomainMax, 
                      int pMaxIter, 
                      int pPopulationSize,
                      double w, double c1, double c2, double k );
        ~OptimizerPSO();

        void start() override;
        void step() override;
        void end() override;

        void changeDomain( float pDomainMin, float pDomainMax ) override;

        // Accessors

        vector< PSOparticle > particles() { return m_particles; }
        PSOparticle bestParticle() { return m_bestParticle; }

        Vec bestPos() override { return m_bestParticle.pos; } ;
        double bestCost() override { return m_bestParticle.cost; };
        Vec avgPosition() override { return m_avgPosition; }
        vector< Vec > getParticlesPositions( int size ) override;
    };






}