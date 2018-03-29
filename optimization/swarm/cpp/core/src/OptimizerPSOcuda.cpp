
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

        // Host
        m_particlesHostInfo.ndim        = pNdim;
        m_particlesHostInfo.population  = pPopulationSize;
        m_particlesHostInfo.pos         = new double[ pNdim * pPopulationSize ];
        m_particlesHostInfo.vel         = new double[ pNdim * pPopulationSize ];
        m_particlesHostInfo.bpos        = new double[ pNdim * pPopulationSize ];
        m_particlesHostInfo.cost        = new double[ pPopulationSize ];
        m_particlesHostInfo.bcost       = new double[ pPopulationSize ];

        // Device
        m_particlesDeviceInfo.ndim          = pNdim;
        m_particlesDeviceInfo.population    = pPopulationSize;
        cuPSOcreateParticles( m_particlesDeviceInfo );

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
        delete[] m_particlesHostInfo.pos;
        delete[] m_particlesHostInfo.vel;
        delete[] m_particlesHostInfo.bpos;
        delete[] m_particlesHostInfo.cost;
        delete[] m_particlesHostInfo.bcost;

        cuPSOreleaseParticles( m_particlesDeviceInfo );
    }

    void OptimizerPSOcuda::start()
    {
        OptimizerInterface::start();

        // Initialize population
        m_bestCost = m_isMinimization ? 1000000.0 : -1000000.0;

        cuPSOinitParticles( m_particlesHostInfo,
                            m_particlesDeviceInfo );

        cout << "Finished initializing PSO ptimizer" << endl;
    }

    void OptimizerPSOcuda::step()
    {
        OptimizerInterface::step();

        cuPSOupdateParticles( m_particlesHostInfo, m_particlesDeviceInfo );

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
