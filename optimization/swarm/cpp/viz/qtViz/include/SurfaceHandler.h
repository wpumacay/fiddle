
#pragma once

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>
#include <QCustom3DItem>

#include <BenchmarkFunctions.h>

#ifndef RESOURCE_PATH
    #define RESOURCE_PATH "../../../res"
#endif

#define NUM_PLOT_PARTICLES 1000

using namespace std;
using namespace QtDataVisualization;

namespace optimization
{

    class SurfaceHandler
    {
        
        private:
            
        Q3DSurface *m_graph;
        QSurfaceDataProxy *m_dataProxy;
        QSurface3DSeries *m_dataSeries;

        float m_domainMin;
        float m_domainMax;
        float m_rangeMin;
        float m_rangeMax;

        BaseFunction* m_objFcn;

        vector< QCustom3DItem* > m_plotParticles;
        QCustom3DItem* m_bestParticle;
        QCustom3DItem* m_avgParticle;

        void _computeSurface();
        void _resetParticlesPositions();

        public :

        SurfaceHandler( Q3DSurface *surface );
        ~SurfaceHandler();

        void changeObjectiveFunction( BaseFunction* pNewObjFcn, 
                                      float pDomainMin, float pDomainMax,
                                      float pRangeMin, float pRangeMax );
        void changeRange( float pRangeMin, float pRangeMax );
        void changeDomain( float pDomainMin, float pDomainMax );

        float getDomainMin() { return m_domainMin; }
        float getDomainMax() { return m_domainMax; }

        void updateParticlesPositions( const vector< optimization::Vec >& psoParticles );
        void updateBestParticle( const optimization::Vec& position );
        void updateAvgParticle( const optimization::Vec& position );
    };

}
