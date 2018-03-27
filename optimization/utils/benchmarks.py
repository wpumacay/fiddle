
from __future__ import print_function

import numpy as np

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d

class BenchmarkFunction ( object ) :

    PLOTTING_MODE_WIREFRAME     = 0
    PLOTTING_MODE_SURFACE       = 1
    # PLOTTING_MODE_TRI_SURFACE   = 2

    def __init__( self, ndim, plottingMode, xmin = -10, xmax = 10, step = 1 ) :

        self.m_ndim = ndim
        self.m_plottingMode = plottingMode
        self.m_step = step

        self.m_min = xmin
        self.m_max = xmax

        self.m_fig = None
        self.m_axes = None

        self.m_figContour = None
        self.m_axesContour = None

        if self.m_ndim > 2 :
            print( 'BenchmarkFunction> info: plotting not enable for more than 2D' )
        elif self.m_ndim > 0 :
            self.m_fig = plt.figure()
            if self.m_ndim == 2 : 
                self.m_axes = self.m_fig.add_subplot( 111, projection = '3d' )
            else :
                self.m_axes = self.m_fig.add_subplot( 111 )

            if self.m_ndim == 2 :
                self.m_figContour = plt.figure()
                self.m_axesContour = self.m_figContour.add_subplot( 111 )

        else :
            print( 'BenchmarkFunction> warn: less than 1d was given' )
            raise AssertionError

    ## Evaluation method
    ## @param self the object pointer
    ## @param X a matrix with rows being the samples to evaluate
    def _eval( self, X ) :

        if X.shape[1] != self.m_ndim : 
            print( 'BenchmarkFunction> error: dimension mismatch' )
            raise AssertionError

        return np.zeros( ( X.shape[0], 1 ) )

    def fig( self ) :
        return self.m_fig

    def axes( self ) :
        return self.m_axes

    def figContour( self ) :
        return self.m_figContour

    def axesContour( self ) :
        return self.m_axesContour

    def __call__( self, val ) :
        self._eval( val )

    def _plotBase1D( self ) :

        self.m_axes.set_xlim( self.m_min, self.m_max )
        xx = np.arange( self.m_min, self.m_max, self.m_step )
        xx = np.reshape( xx, ( len( xx ), 1 ) )
        yy = self._eval( xx )

        self.m_axes.plot( xx, yy )

    def _plotBase2D( self ) :

        self.m_axes.set_xlim( self.m_min, self.m_max )
        self.m_axes.set_ylim( self.m_min, self.m_max )

        _xx = np.arange( self.m_min, self.m_max, self.m_step )
        _yy = np.arange( self.m_min, self.m_max, self.m_step )

        # make mesh
        _X, _Y = np.meshgrid( _xx, _yy )
        _dim = _X.shape
        _Z = np.zeros( _dim )

        for i in range( _dim[0] ) :
            for j in range( _dim[1] ) :
                _Z[i,j] = self._eval( np.array( [ [ _X[i,j], _Y[i,j] ] ] ) )

        # print( 'X: ' )
        # print( _X )

        # print( 'Y: ' )
        # print( _Y )

        # print( 'Z: ' )
        # print( _Z )

        if self.m_plottingMode == BenchmarkFunction.PLOTTING_MODE_WIREFRAME :
            self.m_axes.plot_wireframe( _X, _Y, _Z )

        elif self.m_plottingMode == BenchmarkFunction.PLOTTING_MODE_SURFACE :
            self.m_axes.plot_surface( _X, _Y, _Z )

        # elif self.m_plottingMode == BenchmarkFunction.PLOTTING_MODE_TRI_SURFACE :
        #     self.m_axes.plot_trisurf( _X, _Y, _Z, linewidth=0.2, antialised = True     )

        else :
            print( 'BenchmarkFunction> warn: wrong plotting mode set ', self.m_plottingMode )


    def plotBase( self ) :
        
        if self.m_ndim == 1 :
            self._plotBase1D()
        elif self.m_ndim == 2 :
            self._plotBase2D()

    def _plotSequence1D( self, seqX ) :

        _yy = self._eval( _xx )

        self.m_axes.plot( _xx, _yy, 'r' )

    def _plotSequence2D( self, seqX ) :

        _xx = seqX[:,0]
        _yy = seqX[:,1]
        _zz = self._eval( seqX )

        self.m_axes.plot( _xx, _yy, _zz, 'r' )

    ## Plot a sequence of samples in the function surface
    ## @param self the object pointer
    ## @param X a matrix with rows being the samples to plot in the function surface
    def plotSequence( self, seqX ) :

        if seqX.shape[1] != self.m_ndim :
            print( 'BenchmarkFunction> error: dimension mismatch' )
            raise AssertionError

        if self.m_ndim == 1 :
            self._plotSequence1D( seqX )
        elif self.m_ndim == 2 :
            self._plotSequence2D( seqX )        

    def plotSingle( self, X ) :
        # Override this
        pass


class BMSphere( BenchmarkFunction ) :


    def __init__( self, ndim, plottingMode, xmin = -10, xmax = 10, step = 0.1 ) :

        super( BMSphere, self ).__init__( ndim, plottingMode )

    def _eval( self, X ) :
        super( BMSphere, self )._eval( X )

        return np.sum( X * X, axis=1 )


class BMAckley( BenchmarkFunction ) :

    def __init__( self, a, b, c, ndim, plottingMode, xmin = -40, xmax = 40, step = 1 ) :

        super( BMAckley, self ).__init__( ndim, plottingMode, xmin, xmax, step )

        self.m_a = a
        self.m_b = b
        self.m_c = c

    def _eval( self, X ) :
        super( BMAckley, self )._eval( X )

        a = self.m_a
        b = self.m_b
        c = self.m_c
        d = self.m_ndim

        # Ackley function :
        # -a * exp( -b * sqrt( (1./d) * sumOS( X * X ) ) ) - exp( (1/d) * sumOS( cos( X ) ) ) + a + e
        # = _term1 + _term2 + _term3
        # global minimum = ( 0, 0, ... )

        _term1 = -a * np.exp( -b * np.sqrt( ( 1.0 / d ) * np.sum( X * X, axis = 1 ) ) )
        _term2 = -np.exp( ( 1.0 / d ) * np.sum( np.cos( c * X ), axis = 1 ) )
        _term3 = a + np.e

        return _term1 + _term2 + _term3
        

class BMSchwefel( BenchmarkFunction ) :

    def __init__( self, ndim, plottingMode, xmin = -500, xmax = 500, step = 10 ) :

        super( BMSchwefel, self ).__init__( ndim, plottingMode, xmin, xmax, step )

    def _eval( self, X ) :
        super( BMSchwefel, self )._eval( X )

        d = self.m_ndim

        # Schwefel function
        # 418.9829 * d - sumOS( X * sin( sqrt( abs( X ) ) ) , axis = 1 )
        # global minimum = ( 420.9867, 420.9867, ... )

        return 418.9829 * d - np.sum( X * np.sin( np.sqrt( np.abs( X ) ) ) , axis = 1 )


class BMFunction3( BenchmarkFunction ) :

    def __init__( self, ndim, plottingMode, xmin = -10, xmax = 10, step = 0.2 ) :

        super( BMFunction3, self ).__init__( ndim, plottingMode, xmin, xmax, step )

    def _eval( self, X ) :
        super( BMFunction3, self )._eval( X )

        d = self.m_ndim

        # Function3 function
        # 0.5 - ( np.square( np.sin( np.sqrt( np.sum( X * X, axis = 1 ) ) ) ) - 0.5 ) / np.square( 1.0 + 0.001 * ( np.sum( X * X, axis = 1 ) ) )
        # global minimum = ( 0, 0, ... )

        _denominator = np.square( 1.0 + 0.001 * np.sum( X * X, axis = 1 ) )
        _numerator   = np.square( np.sin( np.sqrt( np.sum( X * X, axis = 1 ) ) ) ) - 0.5

        return 0.5 - _numerator / _denominator