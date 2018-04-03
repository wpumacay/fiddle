
#include <OptimizerPSO.h>

using namespace std;


namespace optimization
{


    OptimizerPSO::OptimizerPSO( int pNdim, 
                                double pDomainMin, 
                                double pDomainMax, 
                                int pMaxIter, 
                                int pPopulationSize,
                                double w, double c1, double c2, double k )
        : OptimizerInterface( pNdim, pDomainMin, pDomainMax, pMaxIter )
    {
        m_populationSize = pPopulationSize;

        m_particles = vector< PSOparticle >( m_populationSize, PSOparticle( pNdim ) );
        m_bestParticle = PSOparticle( pNdim );

        m_avgPosition = Vec( pNdim );

        m_w = w;
        m_c1 = c1;
        m_c2 = c2;
        m_k = k;

        m_vmin = -k * ( m_domainMax - m_domainMin ) / 2.0;
        m_vmax = k * ( m_domainMax - m_domainMin ) / 2.0;
    }

    OptimizerPSO::~OptimizerPSO()
    {
        m_particles.clear();
    }

    void OptimizerPSO::start()
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

    void OptimizerPSO::step()
    {
        OptimizerInterface::step();

        m_avgPosition = Vec::zeros( m_ndim );

        for ( PSOparticle& _p : m_particles )
        {
            _p.vel = m_w * _p.vel + 
                     m_c1 * Vec::randUniform( m_ndim, 0.0, 1.0 ) * ( _p.bestPos - _p.pos ) +
                     m_c2 * Vec::randUniform( m_ndim, 0.0, 1.0 ) * ( m_bestParticle.pos - _p.pos );

            double _v = Vec::norm( _p.vel );

            if ( _v > 0.0001 )
            {
                _v = CLAMP( _v, m_vmin, m_vmax );
                Vec::normalize( _p.vel );
                Vec::scale( _p.vel, _v );
            }

            _p.pos = _p.pos + _p.vel;
            Vec::clip( _p.pos, m_domainMin, m_domainMax );

            _p.cost = m_objFcn->eval( _p.pos );

            if ( _isBetter( _p.cost, _p.bestCost ) )
            {
                _p.bestCost = _p.cost;
                _p.bestPos = _p.pos;

                if ( _isBetter( _p.cost, m_bestParticle.cost ) )
                {
                    m_bestParticle.cost = _p.cost;
                    m_bestParticle.pos  = _p.pos;
                }
            }

            m_avgPosition = m_avgPosition + _p.pos;
        }

        Vec::scale( m_avgPosition, 1.0 / m_populationSize );

        cout << "iteration " << ( m_currentIter + 1 ) << "**********" << endl;

        cout << "best: " << endl;
        cout << m_bestParticle.pos.toString() << endl;

        cout << "cost: " << endl;
        cout << m_bestParticle.cost << endl;

        cout << "avg: " << endl;
        cout << m_avgPosition.toString() << endl;

        cout << "**************************************" << endl;
    }

    void OptimizerPSO::end()
    {
        // Do nothing for now
    }

    void OptimizerPSO::changeDomain( float pDomainMin, float pDomainMax )
    {
        OptimizerInterface::changeDomain( pDomainMin, pDomainMax );

        m_vmin = -m_k * ( m_domainMax - m_domainMin ) / 2.0;
        m_vmax = m_k * ( m_domainMax - m_domainMin ) / 2.0;
    }

    vector< Vec > OptimizerPSO::getParticlesPositions( int size )
    {
        vector< Vec > _particles;

        for ( int q = 0; q < m_particles.size(); q++ )
        {
            if ( q >= size )
            {
                break;
            }
            
            _particles.push_back( m_particles[q].pos );
        }

        return _particles;
    }

}