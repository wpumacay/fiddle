

#include <SurfaceHandler.h>

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>
#include <QtGui/QImage>
#include <QtCore/qmath.h>

#include <string>

using namespace std;
using namespace QtDataVisualization;

const int SAMPLE_COUNT_X = 100;
const int SAMPLE_COUNT_Z = 100;

namespace optimization
{

    SurfaceHandler::SurfaceHandler( Q3DSurface *pSurface )
    {
        m_graph = pSurface;

        m_graph->setAxisX( new QValue3DAxis() );
        m_graph->setAxisY( new QValue3DAxis() );
        m_graph->setAxisZ( new QValue3DAxis() );

        m_graph->axisX()->setLabelFormat( "%.2f" );
        m_graph->axisZ()->setLabelFormat( "%.2f" );
        m_graph->axisX()->setLabelAutoRotation( 30 );
        m_graph->axisY()->setLabelAutoRotation( 90 );
        m_graph->axisZ()->setLabelAutoRotation( 30 );

        m_dataProxy = new QSurfaceDataProxy();
        m_dataSeries = new QSurface3DSeries( m_dataProxy );

        m_graph->addSeries( m_dataSeries );

        // Initialize drawing particles *********************************************

        std::string _cubePathObj( RESOURCE_PATH );
        _cubePathObj += "cube.obj";

        QImage _colorBest = QImage( 10, 10, QImage::Format_RGB32 );
        _colorBest.fill( Qt::blue );

        m_bestParticle = new QCustom3DItem( _cubePathObj.c_str(),
                                            QVector3D( 0.0, 0.0, 0.0 ),
                                            QVector3D( 0.05, 0.05, 0.05 ),
                                            QQuaternion::fromAxisAndAngle( 0.0f, 1.0f, 0.0f, 0.0f ),
                                            _colorBest );

        QImage _colorAvg = QImage( 10, 10, QImage::Format_RGB32 );
        _colorAvg.fill( Qt::cyan );

        m_avgParticle = new QCustom3DItem( _cubePathObj.c_str(),
                                           QVector3D( 0.0, 0.0, 0.0 ),
                                           QVector3D( 0.05, 0.05, 0.05 ),
                                           QQuaternion::fromAxisAndAngle( 0.0f, 1.0f, 0.0f, 0.0f ),
                                           _colorAvg );

        m_graph->addCustomItem( m_bestParticle );
        m_graph->addCustomItem( m_avgParticle );

        QImage _colorSwarm = QImage( 10, 10, QImage::Format_RGB32 );
        _colorSwarm.fill( Qt::red );

        for ( int q = 0; q < NUM_PLOT_PARTICLES; q++ )
        {
            auto _plotPointItem = new QCustom3DItem( _cubePathObj.c_str(),
                                                     QVector3D( 0.0, 0.0, 0.0 ),
                                                     QVector3D( 0.025, 0.025, 0.025 ),
                                                     QQuaternion::fromAxisAndAngle( 0.0f, 1.0f, 0.0f, 0.0f ),
                                                     _colorSwarm );

            m_plotParticles.push_back( _plotPointItem );
            m_graph->addCustomItem( _plotPointItem );
        }

        // **************************************************************************
    }

    SurfaceHandler::~SurfaceHandler()
    {
        delete m_graph;
    }

    void SurfaceHandler::changeObjectiveFunction( BaseFunction* pObjFcn, 
                                                  float pDomainMin, float pDomainMax,
                                                  float pRangeMin, float pRangeMax )
    {
        m_objFcn = pObjFcn;
        changeDomain( pDomainMin, pDomainMax );
        changeRange( pRangeMin, pRangeMax );

        m_graph->removeSeries( m_dataSeries );

        _computeSurface();
        _resetParticlesPositions();

        m_dataSeries->setDrawMode( QSurface3DSeries::DrawSurfaceAndWireframe );        
        m_dataSeries->setFlatShadingEnabled( true );

        m_graph->addSeries( m_dataSeries );
    }

    void SurfaceHandler::changeRange( float pRangeMin, float pRangeMax )
    {
        m_rangeMin = pRangeMin;
        m_rangeMax = pRangeMax;
        m_graph->axisY()->setRange( m_rangeMin, m_rangeMax );

        cout << "new range> min: " << m_rangeMin << " max: " << m_rangeMax << endl;
    }

    void SurfaceHandler::changeDomain( float pDomainMin, float pDomainMax )
    {
        m_domainMin = pDomainMin;
        m_domainMax = pDomainMax;
        m_graph->axisX()->setRange( m_domainMin, m_domainMax );
        m_graph->axisZ()->setRange( m_domainMin, m_domainMax );

        cout << "new domain> min: " << m_domainMin << " max: " << m_domainMax << endl;
    }

    void SurfaceHandler::_computeSurface()
    {
        float _stepX = ( m_domainMax - m_domainMin ) / ( ( float )( SAMPLE_COUNT_X - 1 ) );
        float _stepZ = ( m_domainMax - m_domainMin ) / ( ( float )( SAMPLE_COUNT_Z - 1 ) );

        QSurfaceDataArray* _dataArray = new QSurfaceDataArray();
        _dataArray->reserve( SAMPLE_COUNT_Z );

        for ( int i = 0 ; i < SAMPLE_COUNT_Z ; i++ ) 
        {
            QSurfaceDataRow* _newRow = new QSurfaceDataRow( SAMPLE_COUNT_X );
            // Keep values within range bounds, since just adding step can cause minor drift due
            // to the rounding errors.
            float _z = qMin( m_domainMax, ( i * _stepZ + m_domainMin ) );
            int _index = 0;

            for ( int j = 0; j < SAMPLE_COUNT_X; j++ ) 
            {
                float _x = qMin( m_domainMax, ( j * _stepX + m_domainMin ) );

                optimization::Vec _pos( 2, 0.0 );
                _pos.set( 0, _x );
                _pos.set( 1, _z );

                float _y = m_objFcn->eval( _pos );

                ( *_newRow )[ _index++ ].setPosition( QVector3D( _x, _y, _z ) );
            }

            *_dataArray << _newRow;
        }

        m_dataProxy->resetArray( _dataArray );
    }

    void SurfaceHandler::_resetParticlesPositions()
    {
        if ( m_objFcn == NULL )
        {
            cout << "wops, you should have set the objective function beforehand" << endl;
            return;
        }

        cout << "updating particles" << endl;

        float _xOptima = 0.0;
        float _zOptima = 0.0;
        optimization::Vec _v( 2 );
        _v.set( 0, _xOptima );
        _v.set( 1, _zOptima );

        float _yOptima = m_objFcn->eval( _v );

        m_bestParticle->setPosition( QVector3D( _xOptima, _yOptima, _zOptima ) );

        for ( int q = 0; q < NUM_PLOT_PARTICLES; q++ )
        {
            float _x = RANDOM( m_domainMin, m_domainMax );
            float _z = RANDOM( m_domainMin, m_domainMax );

            optimization::Vec _vpos( 2 );
            _vpos.set( 0, _x );
            _vpos.set( 1, _z );

            float _y = m_objFcn->eval( _vpos );

            m_plotParticles[q]->setPosition( QVector3D( _x, _y, _z ) );
        }
    }

    void SurfaceHandler::updateParticlesPositions( const vector< optimization::Vec >& psoParticles )
    {
        if ( psoParticles.size() < NUM_PLOT_PARTICLES )
        {
            cout << "should provide " << NUM_PLOT_PARTICLES << " particles to the visualizer" << endl;
            return;
        }

        for ( int q = 0; q < NUM_PLOT_PARTICLES; q++ )
        {
            float _y = m_objFcn->eval( psoParticles[q] );

            // cout << "v: " << psoParticles[q].toString() << endl;

            m_plotParticles[q]->setPosition( QVector3D( psoParticles[q][0], _y, psoParticles[q][1] ) );
        }
    }

    void SurfaceHandler::updateBestParticle( const optimization::Vec& position )
    {
        float _y = m_objFcn->eval( position );

        m_bestParticle->setPosition( QVector3D( position[0], _y, position[1] ) );
    }

    void SurfaceHandler::updateAvgParticle( const optimization::Vec& position )
    {
        float _y = m_objFcn->eval( position );

        m_avgParticle->setPosition( QVector3D( position[0], _y, position[1] ) );
    }
}
