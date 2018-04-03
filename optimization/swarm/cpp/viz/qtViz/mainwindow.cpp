#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtDataVisualization>
#include <QtDataVisualization/Q3DSurface>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QRadioButton>

using namespace std;
using namespace QtDataVisualization;

namespace viz
{

    const string FCN_NAMES[4] = { "Sphere", "Ackley", "Schwefel", "SchafferFcn6" };

    MainWindow::MainWindow( QWidget *parent ) :
        QMainWindow( parent ),
        m_ui( new Ui::MainWindow )
    {
        m_ui->setupUi( this );

        // Create Surface related resources **************************************

        Q3DSurface* _graph3dSurface = new Q3DSurface();

        m_surfaceContainer = QWidget::createWindowContainer( _graph3dSurface );

        QSize _screenSize = _graph3dSurface->screen()->size();
        m_surfaceContainer->setFixedSize( QSize( _screenSize.width() / 2, _screenSize.height() / 1.6 ) );
        m_surfaceContainer->setMaximumSize( _screenSize );
        m_surfaceContainer->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
        m_surfaceContainer->setFocusPolicy( Qt::StrongFocus );

        // ***********************************************************************

        // Setup UI **************************************************************

        m_fcnName = FCN_NAMES[0];
        m_fcnDims = 2;
        m_isRunning = false;
        m_isUsingCuda = false;

        m_ui->m_cobxFcnName->addItem( FCN_NAMES[0].c_str() );
        m_ui->m_cobxFcnName->addItem( FCN_NAMES[1].c_str() );
        m_ui->m_cobxFcnName->addItem( FCN_NAMES[2].c_str() );
        m_ui->m_cobxFcnName->addItem( FCN_NAMES[3].c_str() );

        m_ui->m_chbxSurface->setEnabled( true );
        m_ui->m_chbxPlot->setEnabled( true );

        m_ui->m_spboxDimns->setRange( 2, 20 );

        connect( m_ui->m_chbxCuda,
                 SIGNAL( clicked(bool) ),
                 this,
                 SLOT( _onOptimizerSwitched(bool) ) );

        connect( m_ui->m_cobxFcnName, 
                 static_cast< void ( QComboBox::* )( int ) >( &QComboBox::currentIndexChanged ),
                 this,
                 &MainWindow::_onFunctionNameChanged );

        connect( m_ui->m_spboxDimns,
                 SIGNAL( valueChanged(int) ),
                 this,
                 SLOT( _onFunctionDimensionsChanged(int) ) );

        connect( m_ui->m_chbxSurface,
                 SIGNAL( clicked(bool) ),
                 this,
                 SLOT( _onSurfaceVizSwitched(bool) ) );

        connect( m_ui->m_chbxPlot,
                 SIGNAL( clicked(bool) ),
                 this,
                 SLOT( _onPlotVizSwitched(bool) ) );

        connect( m_ui->m_btnRun,
                 SIGNAL( clicked() ),
                 this,
                 SLOT( _onTestRun() ) );

        connect( m_ui->m_btnStop,
                 SIGNAL( clicked() ),
                 this,
                 SLOT( _onTestStop() ) );

        connect( m_ui->m_btnReset,
                 SIGNAL( clicked() ),
                 this,
                 SLOT( _onTestReset() ) );

        // Setup optimization resources

        m_optimizerGpu = new optimization::OptimizerPSOcuda( m_fcnDims, -500.0, 500.0, 100, NUM_PARTICLES,
                                                             0.5, 2.0, 2.0, 0.5 );

        m_optimizerCpu = new optimization::OptimizerPSO( m_fcnDims, -500.0, 500.0, 100, NUM_PARTICLES,
                                                         0.5, 2.0, 2.0, 0.5 );

        m_currentOptimizer = m_optimizerCpu;

        m_surfaceViz = new viz::SurfaceViz( _graph3dSurface );

        _setupObjectiveFcnById( m_fcnName );

        m_timer = new QTimer( this );
        connect( m_timer, SIGNAL( timeout() ), this, SLOT( _onTimerTick() ) );
    }

    void MainWindow::_setupObjectiveFcnById( const string& objFcnStrId )
    {
        if ( objFcnStrId == "Sphere" )
        {
            m_optimizerCpu->changeDomain( -10.0, 10.0 );
            m_optimizerCpu->setObjectiveFunction( new optimization::FunctionSphere(), true );

            m_optimizerGpu->changeDomain( -10.0, 10.0 );
            m_optimizerGpu->setObjectiveFunction( new optimization::FunctionSphere(), true );

            m_surfaceViz->changeObjectiveFunction( m_optimizerGpu->objFunction(), -10.0, 10.0, 0.0, 100.0 );
        }
        else if ( objFcnStrId == "Ackley" )
        {
            m_optimizerCpu->changeDomain( -32.0, 32.0 );
            m_optimizerCpu->setObjectiveFunction( new optimization::FunctionAckley(), true );

            m_optimizerGpu->changeDomain( -32.0, 32.0 );
            m_optimizerGpu->setObjectiveFunction( new optimization::FunctionAckley(), true );

            m_surfaceViz->changeObjectiveFunction( m_optimizerGpu->objFunction(), -32.0, 32.0, 0.0, 20.0 );
        }
        else if ( objFcnStrId == "Schwefel" )
        {
            m_optimizerCpu->changeDomain( -500.0, 500.0 );
            m_optimizerCpu->setObjectiveFunction( new optimization::FunctionSchwefel(), true );

            m_optimizerGpu->changeDomain( -500.0, 500.0 );
            m_optimizerGpu->setObjectiveFunction( new optimization::FunctionSchwefel(), true );
            
            m_surfaceViz->changeObjectiveFunction( m_optimizerGpu->objFunction(), -500.0, 500.0, 0.0, 2000.0 );
        }
        else if ( objFcnStrId == "SchafferFcn6" )
        {
            m_optimizerCpu->changeDomain( -10.0, 10.0 );
            m_optimizerCpu->setObjectiveFunction( new optimization::FunctionShafferFcn6(), false );

            m_optimizerGpu->changeDomain( -10.0, 10.0 );
            m_optimizerGpu->setObjectiveFunction( new optimization::FunctionShafferFcn6(), false );
            
            m_surfaceViz->changeObjectiveFunction( m_optimizerGpu->objFunction(), -10.0, 10.0, 0.0, 1.0 );
        }
    }

    void MainWindow::_switchOptimizer( bool useCuda )
    {
        m_isUsingCuda = useCuda;

        if ( useCuda )
        {
            m_currentOptimizer = m_optimizerGpu;
        }
        else
        {
            m_currentOptimizer = m_optimizerCpu;
        }
    }

    void MainWindow::_onTimerTick()
    {
        if ( m_currentOptimizer == NULL )
        {
            cout << "wops, haven't set an optimizer" << endl;
            return;
        }

        m_currentOptimizer->step();
        cout << "bestCost: " << to_string( m_currentOptimizer->bestCost() ) << endl;

        if ( m_fcnDims == 2 )
        {
            auto _psoParticles = m_currentOptimizer->getParticlesPositions( NUM_PLOT_PARTICLES );
            auto _bestPos = m_currentOptimizer->bestPos();
            auto _avgPos = m_currentOptimizer->avgPosition();

            m_surfaceViz->updateParticlesPositions( _psoParticles );
            m_surfaceViz->updateBestParticle( _bestPos );
            m_surfaceViz->updateAvgParticle( _avgPos );
        }
    }

    void MainWindow::_onOptimizerSwitched( bool val )
    {
        _switchOptimizer( val );
    }

    void MainWindow::_onFunctionDimensionsChanged( int val )
    {
        m_fcnDims = val;
        cout << "changed to " << m_fcnDims << " dimensions" << endl;

        // reset the optimizers
        {
            if ( m_optimizerGpu != NULL )
            {
                delete m_optimizerGpu;
            }

            m_optimizerGpu = new optimization::OptimizerPSOcuda( m_fcnDims, -500.0, 500.0, 100, NUM_PARTICLES,
                                                                 0.5, 2.0, 2.0, 0.5 );
        }
        
        {
            if ( m_optimizerCpu != NULL )
            {
                delete m_optimizerCpu;
            }            

            m_optimizerCpu = new optimization::OptimizerPSO( m_fcnDims, -500.0, 500.0, 100, NUM_PARTICLES,
                                                             0.5, 2.0, 2.0, 0.5 );
        }

        _setupObjectiveFcnById( m_fcnName );
        _switchOptimizer( m_isUsingCuda );

        if ( m_fcnDims == 2 )
        {
            cout << "enabling surface viz" << endl;
            m_ui->m_chbxSurface->setEnabled( true );
        }
        else
        {
            cout << "disabling surface viz" << endl;
            m_ui->m_chbxSurface->setEnabled( false );
            m_surfaceContainer->hide();
        }
    }

    void MainWindow::_onFunctionNameChanged( int indx )
    {
        m_fcnName = FCN_NAMES[indx];
        _setupObjectiveFcnById( m_fcnName );
    }

    void MainWindow::_onSurfaceVizSwitched( bool val )
    {
        if ( val )
        {
            m_surfaceContainer->show();
        }
        else
        {
            m_surfaceContainer->hide();
        }
    }

    void MainWindow::_onPlotVizSwitched( bool val )
    {
        // Do nothing, not implemented yet
    }


    void MainWindow::_onTestRun()
    {
        if ( m_currentOptimizer == NULL )
        {
            cout << "wops, haven't set an optimizer" << endl;
            return;
        }

        if ( !m_isRunning )
        {
            // run current optimizer
            m_currentOptimizer->start();

            m_isRunning = true;
        }

        m_timer->start( 33 );
    }

    void MainWindow::_onTestStop()
    {
        m_timer->stop();
    }

    void MainWindow::_onTestReset()
    {
        m_isRunning = false;
        m_timer->stop();
    }


    MainWindow::~MainWindow()
    {
        delete m_ui;
    }

}