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

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow(parent),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );


    Q3DSurface* _graph3dSurface = new Q3DSurface();

    QWidget* _graphContainer = QWidget::createWindowContainer( _graph3dSurface );

    QSize _screenSize = _graph3dSurface->screen()->size();
    _graphContainer->setFixedSize( QSize( _screenSize.width() / 2, _screenSize.height() / 1.6 ) );
    _graphContainer->setMaximumSize( _screenSize );
    _graphContainer->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    _graphContainer->setFocusPolicy( Qt::StrongFocus );

    QWidget* _centralWidget = new QWidget();
    setCentralWidget( _centralWidget );

    QHBoxLayout* _hlayout = new QHBoxLayout( _centralWidget );
    QVBoxLayout* _vlayout = new QVBoxLayout();
    _hlayout->addWidget( _graphContainer, 1 );
    _hlayout->addLayout( _vlayout );
    _vlayout->setAlignment( Qt::AlignTop );

    std::string _fnName( "Schwefel" );

    m_surfaceHandler = new optimization::SurfaceHandler( _graph3dSurface );

    m_optimizer = new optimization::OptimizerPSOcuda( 2, -500.0, 500.0, 100, 100000,
                                                      0.5, 2.0, 2.0, 0.5 );

    _setupObjectiveFcnById( _fnName );

    _vlayout->addWidget( new QLabel( QStringLiteral( "Test Options" ) ) );


    m_timer = new QTimer( this );
    connect( m_timer, SIGNAL( timeout() ), this, SLOT( _onTimerTick() ) );

    // test optimization
    m_optimizer->start();

    m_timer->start( 100 );
}

void MainWindow::_setupObjectiveFcnById( const string& objFcnStrId )
{
    if ( objFcnStrId == "Sphere" )
    {
        m_optimizer->changeDomain( -10.0, 10.0 );
        m_optimizer->setObjectiveFunction( new optimization::FunctionSphere(), true );
        m_surfaceHandler->changeObjectiveFunction( m_optimizer->objFunction(), -10.0, 10.0, 0.0, 100.0 );
    }
    else if ( objFcnStrId == "Ackley" )
    {
        m_optimizer->changeDomain( -32.0, 32.0 );
        m_optimizer->setObjectiveFunction( new optimization::FunctionAckley(), true );
        m_surfaceHandler->changeObjectiveFunction( m_optimizer->objFunction(), -32.0, 32.0, 0.0, 20.0 );
    }
    else if ( objFcnStrId == "Schwefel" )
    {
        m_optimizer->changeDomain( -500.0, 500.0 );
        m_optimizer->setObjectiveFunction( new optimization::FunctionSchwefel(), true );
        m_surfaceHandler->changeObjectiveFunction( m_optimizer->objFunction(), -500.0, 500.0, 0.0, 2000.0 );
    }
    else if ( objFcnStrId == "SchafferFcn6" )
    {
        m_optimizer->changeDomain( -10.0, 10.0 );
        m_optimizer->setObjectiveFunction( new optimization::FunctionShafferFcn6(), false );
        m_surfaceHandler->changeObjectiveFunction( m_optimizer->objFunction(), -10.0, 10.0, 0.0, 1.0 );
    }
}

void MainWindow::_onTimerTick()
{
    m_optimizer->step();
    cout << "bestCost: " << to_string( m_optimizer->bestCost() ) << endl;

    auto _psoParticles = reinterpret_cast< optimization::OptimizerPSOcuda* >( m_optimizer )->getParticlesPositions( NUM_PLOT_PARTICLES );
    auto _bestPos = reinterpret_cast< optimization::OptimizerPSOcuda* >( m_optimizer )->bestPos();
    auto _avgPos = reinterpret_cast< optimization::OptimizerPSOcuda* >( m_optimizer )->avgPosition();

    m_surfaceHandler->updateParticlesPositions( _psoParticles );
    m_surfaceHandler->updateBestParticle( _bestPos );
    m_surfaceHandler->updateAvgParticle( _avgPos );
}

MainWindow::~MainWindow()
{
    delete ui;
}
