
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

        m_bestPos = Vec( pNdim );
        m_bestCost = 0.0;

        m_avgPosition = Vec( pNdim );

        m_w = w;
        m_c1 = c1;
        m_c2 = c2;
        m_k = k;

        m_vmin = -k * ( m_domainMax - m_domainMin ) / 2.0;
        m_vmax = k * ( m_domainMax - m_domainMin ) / 2.0;

        // Initialize host and device info

        // Host
        m_particlesHostInfo.ndim            = pNdim;
        m_particlesHostInfo.population      = pPopulationSize;
        m_particlesHostInfo.vecBufferSize   = sizeof( double ) * pNdim * pPopulationSize;
        m_particlesHostInfo.costBufferSize  = sizeof( double ) * pPopulationSize;
        m_particlesHostInfo.vecSize         = sizeof( double ) * pNdim;
        m_particlesHostInfo.pos             = new double[ pNdim * pPopulationSize ];
        m_particlesHostInfo.vel             = new double[ pNdim * pPopulationSize ];
        m_particlesHostInfo.bpos            = new double[ pNdim * pPopulationSize ];
        m_particlesHostInfo.cost            = new double[ pPopulationSize ];
        m_particlesHostInfo.bcost           = new double[ pPopulationSize ];
        m_particlesHostInfo.gbestpos        = new double[ pNdim ];
        m_particlesHostInfo.devRandStates   = NULL;// Not needed for host
        m_particlesHostInfo.gbestcost       = 0.0;// Initialize it when calling start -> Inf or -Inf
        m_particlesHostInfo.domainMin       = m_domainMin;
        m_particlesHostInfo.domainMax       = m_domainMax;
        m_particlesHostInfo.vmin            = m_vmin;
        m_particlesHostInfo.vmax            = m_vmax;

        // Device
        m_particlesDeviceInfo.ndim              = pNdim;
        m_particlesDeviceInfo.population        = pPopulationSize;
        m_particlesDeviceInfo.vecBufferSize     = sizeof( double ) * pNdim * pPopulationSize;
        m_particlesDeviceInfo.costBufferSize    = sizeof( double ) * pPopulationSize;
        m_particlesDeviceInfo.vecSize           = sizeof( double ) * pNdim;
        cuPSOcreateParticles( m_particlesDeviceInfo );
        m_particlesDeviceInfo.gbestcost         = 0.0;
        m_particlesDeviceInfo.domainMin         = m_domainMin;
        m_particlesDeviceInfo.domainMax         = m_domainMax;
        m_particlesDeviceInfo.vmin              = m_vmin;
        m_particlesDeviceInfo.vmax              = m_vmax;
    }

    OptimizerPSOcuda::~OptimizerPSOcuda()
    {
        delete[] m_particlesHostInfo.pos;
        delete[] m_particlesHostInfo.vel;
        delete[] m_particlesHostInfo.bpos;
        delete[] m_particlesHostInfo.cost;
        delete[] m_particlesHostInfo.bcost;
        delete[] m_particlesHostInfo.gbestpos;

        cuPSOreleaseParticles( m_particlesDeviceInfo );
    }

    void OptimizerPSOcuda::start()
    {
        OptimizerInterface::start();

        // Initialize population
        m_bestCost = m_isMinimization ? 1000000.0 : -1000000.0;

        cuPSOinitParticles( m_particlesHostInfo,
                            m_particlesDeviceInfo,
                            m_objFcn, m_isMinimization );

        cout << "Finished initializing PSO ptimizer" << endl;
    }

    void OptimizerPSOcuda::step()
    {
        OptimizerInterface::step();

        cuPSOupdateParticles( m_particlesHostInfo, m_particlesDeviceInfo,
                              m_w, m_c1, m_c2, m_k, 
                              m_objFcn->id, m_isMinimization );

        for ( int d = 0; d < m_ndim; d++ )
        {
            m_bestPos.set( d, m_particlesHostInfo.gbestpos[ d ] );
        }

        m_avgPosition = Vec::zeros( m_ndim );

        for ( int p = 0; p < m_populationSize; p++ )
        {
            for ( int d = 0; d < m_ndim; d++ )
            {
                m_avgPosition.set( d, m_avgPosition.get( d ) + ( m_particlesHostInfo.pos[ p * m_ndim + d ] / m_populationSize ) );
            }
        }

        m_bestCost = m_particlesHostInfo.gbestcost;

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

    void OptimizerPSOcuda::changeDomain( float pDomainMin, float pDomainMax )
    {
        OptimizerInterface::changeDomain( pDomainMin, pDomainMax );

        m_vmin = -m_k * ( m_domainMax - m_domainMin ) / 2.0;
        m_vmax = m_k * ( m_domainMax - m_domainMin ) / 2.0;

        // Update both host and device information
        m_particlesHostInfo.domainMin       = m_domainMin;
        m_particlesHostInfo.domainMax       = m_domainMax;
        m_particlesHostInfo.vmin            = m_vmin;
        m_particlesHostInfo.vmax            = m_vmax;

        m_particlesDeviceInfo.domainMin         = m_domainMin;
        m_particlesDeviceInfo.domainMax         = m_domainMax;
        m_particlesDeviceInfo.vmin              = m_vmin;
        m_particlesDeviceInfo.vmax              = m_vmax;
    }

    vector< Vec > OptimizerPSOcuda::getParticlesPositions( int size )
    {
        vector< Vec > _particles;

        for ( int q = 0; q < m_populationSize; q++ )
        {
            if ( q >= size )
            {
                break;
            }

            Vec _v = Vec::zeros( m_ndim );
            
            for ( int d = 0; d < m_ndim; d++ )
            {
                _v.set( d, m_particlesHostInfo.pos[ q * m_ndim + d ] );
            }

            _particles.push_back( _v );
        }

        return _particles;
    }

}
