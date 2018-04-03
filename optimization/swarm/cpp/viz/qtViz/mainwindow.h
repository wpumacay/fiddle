
#pragma once

#include <QMainWindow>

#include <QTimer>

// 3D plot visualizer
#include <SurfaceHandler.h>
// optimizer functionality
#include <OptimizerPSOcuda.h>


namespace Ui 
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:

    Ui::MainWindow *ui;

    QTimer* m_timer;

    optimization::SurfaceHandler* m_surfaceHandler;
    optimization::OptimizerPSOcuda* m_optimizer;

    void _setupObjectiveFcnById( const string& objFcnStrId );

    public Q_SLOTS:

    void _onTimerTick();
    
    public:

    explicit MainWindow( QWidget* parent = NULL );
    ~MainWindow();
};

