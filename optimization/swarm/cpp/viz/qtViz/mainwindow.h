
#pragma once

#include <QMainWindow>

#include <QTimer>

// 3D plot visualizer
#include <SurfaceViz.h>
// optimizer functionality
#include <OptimizerPSO.h>
#include <OptimizerPSOcuda.h>

using namespace std;

#define NUM_PARTICLES 100000

namespace Ui 
{
    class MainWindow;
}

namespace viz
{

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

        private:

        Ui::MainWindow *m_ui;

        QTimer* m_timer;
        bool m_isRunning;
        bool m_isUsingCuda;

        optimization::OptimizerPSOcuda* m_optimizerGpu;
        optimization::OptimizerPSO* m_optimizerCpu;

        optimization::OptimizerInterface* m_currentOptimizer;

        QWidget* m_surfaceContainer;
        viz::SurfaceViz* m_surfaceViz;

        QWidget* m_plotConvergenceContainer;
        // viz::PlotViz* m_plotConvergenceViz;

        string m_fcnName;
        int m_fcnDims;

        void _setupObjectiveFcnById( const string& objFcnStrId );
        void _switchOptimizer( bool useCuda );

        public Q_SLOTS:

        void _onTimerTick();

        void _onOptimizerSwitched( bool val );
        void _onFunctionDimensionsChanged( int val );
        void _onFunctionNameChanged( int indx );

        void _onSurfaceVizSwitched( bool val );
        void _onPlotVizSwitched( bool val );

        void _onTestRun();
        void _onTestStop();
        void _onTestReset();

        public:

        explicit MainWindow( QWidget* parent = NULL );
        ~MainWindow();
    };

}

