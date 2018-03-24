
from __future__ import print_function

import numpy as np

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d

class BenchmarkFunction ( object ) :

    PLOTTING_MODE_WIREFRAME     = 0
    PLOTTING_MODE_SURFACE       = 1
    PLOTTING_MODE_TRI_SURFACE   = 2

    def __init__( self, ndim, plottingMode, xmin = -10, xmax = 10, step = 0.1 ) :

        self.m_ndim = ndim
        self.m_plottingMode = plottingMode
        self.m_step = step

        self.m_min = xmin
        self.m_max = xmax

        if self.m_ndim > 2 :
            print( 'BenchmarkFunction> info: plotting not enable for more than 2D' )
        elif self.m_ndim > 0 :
            self.m_fig = plt.figure()
            if self.m_ndim == 2 : 
                self.m_ax = fig.add_subplot( 111, projection = '3d' )
            else :
                self.m_ax = fig.add_subplot( 111 )
        else :
            print( 'BenchmarkFunction> warn: less than 1d was given' )
            raise AssertionError


    def _eval( self, X ) :

        if len( X ) != self.m_ndim : 
            print( 'BenchmarkFunction> error: dimension mismatch' )
            raise AssertionError

        return 0

    def __call__( self, val ) :
        self._eval( val )

    def _plotBase1D( self ) :

        self.m_ax.set_xlim( self.m_min, self.m_max )
        xx = np.arange( self.m_min, self.m_max, self.m_step )
        yy = self._eval( xx )

        self.m_ax.plot( xx, yy )

    def _plotBase2D( self ) :

        self.m_ax.set_xlim( self.m_min, self.m_max )
        self.m_ax.set_ylim( self.m_min, self.m_max )

        _xx = np.arange( self.m_min, self.m_max, self.m_step )
        _yy = np.arange( self.m_min, self.m_max, self.m_step )

        # make mesh
        _X, _Y = np.meshgrid( _xx, _yy )
        _dim = _X.shape
        _Z = np.zeros( _dim )

        for i in range( _dim[0] ) :
            for j in range( _dim[1] ) :
                _Z[i,j] = self_eval( np.array( [ _X[i,j], _Y[i,j] ] ) )

        if self.m_plottingMode == BenchmarkFunction.PLOTTING_MODE_WIREFRAME :
            self.m_ax.plot_wireframe( _X, _Y, _Z )

        elif self.m_plottingMode == BenchmarkFunction.PLOTTING_MODE_SURFACE :
            self.m_ax.plot_surface( _X, _Y, _Z )

        elif self.m_plottingMode == BenchmarkFunction.PLOTTING_MODE_TRI_SURFACE :
            self.m_ax.plot_trisurf( _X, _Y, _Z, linewidth=0.2, antialised = True )

        else :
            print( 'BenchmarkFunction> warn: wrong plotting mode set ', self.m_plottingMode )


    def plotBase( self ) :
        
        if self.m_ndim == 1 :
            self._plotBase1D()
        elif self.m_ndim == 2 :
            self._plotBase2D()

    def _plotSequence1D( self, seqX ) :

        _xx = np.array( [ _x for _x in seqX ] )
        _yy = self._eval( _xx )

        self.m_ax.plot( _xx, _yy, 'r' )

    def _plotSequence2D( self, seqX ) :

        _xx = np.array( [ _xy[0] for _xy in seqX ] )
        _yy = np.array( [ _xy[1] for _xy in seqX ] )
        _zz = np.zeros( _xx.shape )

        for i in range( len( _xx ) ) :
            _zz[i] = self._eval( _xx[i], _yy[i] )

        self.m_ax.plot( _xx, _yy, _zz, 'r' )

    def plotSequence( self, seqX ) :

        if self.m_ndim == 1 :
            self._plotSequence1D( seqX )
        elif self.m_ndim == 2 :
            self._plotSequence2D( seqX )        

    def plotSingle( self, X ) :
        # Override this
        pass


class Sphere( BenchmarkFunction ) :


    def __init__( self, ndim, plottingMode, xmin = -10, xmax = 10, step = 0.1 ) :

        super( Sphere, self ).__init__( ndim )

    def _eval( self, X ) :
        super( Sphere, self )._eval( X )

        return sum( X * X )


class Ackley( BenchmarkFunction ) :

    def __init__( self, a, b, c, ndim, plottingMode, xmin = -10, xmax = 10, step = 0.1 ) :

        super( Sphere, self ).__init__( ndim, plottingMode, xmin, xmax, step )

        self.m_a = a
        self.m_b = b
        self.m_c = c

    def _eval( self, X ) :
        super( Sphere, self )._eval( X )

        a = self.m_a
        b = self.m_b
        c = self.m_c
        d = self.m_ndim

        

