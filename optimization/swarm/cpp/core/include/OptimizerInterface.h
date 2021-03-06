
#pragma once

#include <Common.h>
#include <BenchmarkFunctions.h>

using namespace std;


namespace optimization
{


    class OptimizerInterface
    {

        protected :

        enum
        {
            STATE_IDLE,
            STATE_OPTIMIZING
        };

        int m_ndim;
        bool m_isMinimization;

        BaseFunction* m_objFcn;
        pthread_t m_threadHandle;

        int m_currentIter;
        int m_maxIter;

        double m_domainMin;
        double m_domainMax;

        bool m_isWorking;

        int m_optState;

        bool _isBetter( double newVal, double oldVal );

        public :

        OptimizerInterface( int pNdim, double pDomainMin, double pDomainMax, int pMaxIter );
        ~OptimizerInterface();

        void setObjectiveFunction( BaseFunction* objFcn, bool pIsMinimization );

        virtual void start();
        virtual void step();
        virtual void end();
        virtual bool stopCondition();

        virtual void changeDomain( float pDomainMin, float pDomainMax );

        void update();

        // To run parallelly
        void run();
        static void* threadWorker( void* pOptimizer );

        // Accessors

        int ndim() { return m_ndim; }
        int maxIter() { return m_maxIter; }
        int currentIter() { return m_currentIter; }
        BaseFunction* objFunction() { return m_objFcn; }
        double domainMin() { return m_domainMin; }
        double domainMax() { return m_domainMax; }
        bool isWorking() { return m_isWorking; }
        bool isMinimization() { return m_isMinimization; }
        int optState() { return m_optState; }

        // Swarm stuff

        virtual Vec bestPos() = 0;
        virtual double bestCost() = 0;
        virtual Vec avgPosition() = 0;
        virtual vector< Vec > getParticlesPositions( int size ) = 0;
    };


}