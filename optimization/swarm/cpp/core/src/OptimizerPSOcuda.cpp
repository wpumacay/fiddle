
#include <OptimizerPSOcuda.h>


using namespace std;

namespace optimization
{


    OptimizerPSOcuda::OptimizerPSOcuda( int pNdim, 
                                        double pDomainMin, 
                                        double pDomainMax, 
                                        int pMaxIter, 
                                        int pPopulationSize,
                                        double w, double c1, double c2, double k )
        : OptimizerInterface( pNdim, pDomainMin, pDomainMax, pMaxIter )
    {
        m_populationSize = pPopulationSize;

        // Initialize host and device info

        

        m_bestPos = Vec( pNdim );
        m_bestCost = 0.0;

        m_avgPosition = Vec( pNdim );

        m_w = w;
        m_c1 = c1;
        m_c2 = c2;
        m_k = k;

        m_vmin = -k * ( m_domainMax - m_domainMin ) / 2.0;
        m_vmax = k * ( m_domainMax - m_domainMin ) / 2.0;
    }

    OptimizerPSOcuda::~OptimizerPSOcuda()
    {
        
    }

    void OptimizerPSOcuda::start()
    {
        OptimizerInterface::start();

        // Initialize population
        m_bestParticle.cost = m_isMinimization ? 1000000.0 : -1000000.0;

        cout << "particles positions" << endl;

        for ( int q = 0; q < m_particles.size(); q++ )
        {
            m_particles[q].vel  = Vec::zeros( m_ndim );
            m_particles[q].pos  = Vec::randUniform( m_ndim, m_domainMin, m_domainMax );
            m_particles[q].cost = m_objFcn->eval( m_particles[q].pos );

            m_particles[q].bestPos  = m_particles[q].pos;
            m_particles[q].bestCost = m_particles[q].cost;

            if ( _isBetter( m_particles[q].cost, m_bestParticle.cost ) )
            {
                m_bestParticle.cost = m_particles[q].cost;
                m_bestParticle.pos  = m_particles[q].pos;
            }
        }

        cout << "Finished initializing PSO ptimizer" << endl;
    }

    void OptimizerPSOcuda::step()
    {
        OptimizerInterface::step();




        cout << "iteration " << m_currentIter << "**********" << endl;

        cout << "best: " << endl;
        cout << m_bestPos.toString() << endl;

        cout << "cost: " << endl;
        cout << m_bestCost << endl;

        cout << "avg: " << endl;
        cout << m_avgPosition.toString() << endl;

        cout << "**************************************" << endl;
    }

    void OptimizerPSOcuda::end()
    {
        // Do nothing for now
    }

















}
