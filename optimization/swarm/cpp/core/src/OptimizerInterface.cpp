
#include <OptimizerInterface.h>

using namespace std;

namespace optimization
{

    OptimizerInterface::OptimizerInterface( int pNdim, double pDomainMin, double pDomainMax, int pMaxIter )
    {
        m_ndim = pNdim;
        m_domainMin = pDomainMin;
        m_domainMax = pDomainMax;
        m_maxIter = pMaxIter;

        m_objFcn = NULL;
        m_isWorking = false;
        m_currentIter = 0;
        m_isMinimization = true;

        m_optState = STATE_IDLE;
    }

    OptimizerInterface::~OptimizerInterface()
    {
        m_objFcn = NULL;
    }

    void OptimizerInterface::setObjectiveFunction( BaseFunction* objFcn, bool pIsMinimization )
    {
        m_objFcn = objFcn;
        m_isMinimization = pIsMinimization;
    }

    void OptimizerInterface::start()
    {
        m_currentIter = 0;
    }

    void OptimizerInterface::step()
    {
        // Override this
    }

    void OptimizerInterface::end()
    {
        // Override this
    }

    bool OptimizerInterface::stopCondition()
    {
        return m_currentIter >= m_maxIter;
    }

    void OptimizerInterface::update()
    {
        if ( m_optState == STATE_OPTIMIZING )
        {
            if ( !m_isWorking )
            {
                cout << "Finished Optimizing" << endl;

                m_optState = STATE_IDLE;
            }
        }
    }

    void OptimizerInterface::run()
    {
        m_isWorking = true;
        m_optState = STATE_OPTIMIZING;

        pthread_create( &m_threadHandle, NULL, OptimizerInterface::threadWorker, ( void* ) this );
    }

    void* OptimizerInterface::threadWorker( void* pOptimizer )
    {
        OptimizerInterface* _optimizer = ( OptimizerInterface* ) pOptimizer;

        _optimizer->start();

        while ( !_optimizer->stopCondition() )
        {
            _optimizer->step();
        }

        _optimizer->end();

        m_isWorking = false;
    }

}