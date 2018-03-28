
from __future__ import print_function

import numpy as np
import matplotlib.pyplot as plt


class BaseOptimizer ( object ) :


    def __init__( self, ndim, xmin, xmax, maxIter = 100 ) :

        self.m_ndim = ndim
        self.m_xmin = xmin
        self.m_xmax = xmax

        self.m_currentNumIters = 0
        self.m_maxIter = maxIter
        self.m_mustMinimize = True

        self.m_refFunctionTarget = None
        self.m_refPlotFig = None
        self.m_refPlotAxes = None
        self.m_refPlotFigContour = None
        self.m_refPlotAxesContour = None

        self.m_currentBest = None

    def setFunctionTarget( self, fcnTarget, mustMinimize ) :

        self.m_refFunctionTarget = fcnTarget
        self.m_mustMinimize = mustMinimize

        self.m_refPlotFig = fcnTarget.fig()
        self.m_refPlotAxes = fcnTarget.axes()
        self.m_refPlotFigContour = fcnTarget.figContour()
        self.m_refPlotAxesContour = fcnTarget.axesContour()

    # If minimizing, true if new is less than old
    # If maximizing, true if new is greater than old
    def _isBetter( self, valNew, valOld ) :

        if self.m_mustMinimize :
            return valOld > valNew
        else :
            return valOld < valNew

    def _stopCondition( self ) :

        return self.m_currentNumIters >= self.m_maxIter

    def _start( self ) :

        self.m_currentBest = None
        self.m_currentNumIters = 0

    def _step( self ) :
        # Override this
        pass

    def _end( self ) :

        pass

    def run( self ) :

        if self.m_refFunctionTarget is None :
            print( 'BaseOptimizer> did not set target function' )
            raise AssertionError

        self._start()

        while not self._stopCondition() :

            self._step()

        self._end()


class PSOparticle :

    def __init__( self, ndim ) :

        self.pos = np.zeros( ( 1, ndim ) )
        self.pBestPos = np.zeros( ( 1, ndim ) )
        self.pBestCost = 0.0
        self.vel = np.zeros( ( 1, ndim ) )
        self.cost = 0.0

class PSOoptimizer( BaseOptimizer ) :


    def __init__( self, ndim, xmin, xmax, populationSize, w, c1, c2, k = 0.5, maxIter = 100 ) :

        super( PSOoptimizer, self ).__init__( ndim, xmin, xmax, maxIter )

        self.m_populationSize = populationSize
        self.m_particles = [ PSOparticle( ndim ) for _ in range( populationSize ) ]

        self.m_bestParticle = PSOparticle( ndim )
        self.m_avgPosition = np.zeros( ( 1, ndim ) )

        self.m_w = w
        self.m_c1 = c1
        self.m_c2 = c2
        self.m_k = k # velocity clamping

    def isInDomain( self, pos ) :

        for i in range( len( pos ) ) :

            if pos[i] < self.m_xmin or pos[i] > self.m_xmax :
                return False

        return True

    def particles( self ) :
        return self.m_particles

    def bestParticle( self ) :
        return self.m_bestParticle

    def avgPosition( self ) :
        return self.m_avgPosition

    def _start( self ) :
        # initialize population

        self.m_bestParticle = PSOparticle( self.m_ndim )
        if self.m_mustMinimize :
            self.m_bestParticle.cost = 1000000.0
        else :
            self.m_bestParticle.cost = -1000000.0

        for p in self.m_particles :

            # p.vel = np.random.uniform( 0.0, 0.01, ( 1, self.m_ndim ) )
            p.vel = np.zeros( ( 1, self.m_ndim ) )
            p.pos = np.random.uniform( self.m_xmin, self.m_xmax, ( 1, self.m_ndim ) )
            p.cost = self.m_refFunctionTarget( p.pos )
            p.pBestPos = p.pos
            p.pBestCost = p.cost

            if self._isBetter( p.cost, self.m_bestParticle.cost ) :
                self.m_bestParticle.cost = p.cost
                self.m_bestParticle.pos = np.copy( p.pos )

        print( 'initialized optimizer' )

    def _step( self ) :

        w = self.m_w
        c1 = self.m_c1
        c2 = self.m_c2
        k = self.m_k
        vmin = -k * ( self.m_xmax - self.m_xmin ) / 2.0
        vmax = k * ( self.m_xmax - self.m_xmin ) / 2.0

        # print( 'vmin: ', vmin )
        # print( 'vmax: ', vmax )

        # Asynchronus PSO implementation

        for p in self.m_particles :

            # Update each particle
            _velInertial = w * p.vel
            _velCognitive = c1 * np.random.random( p.vel.shape ) * ( p.pBestPos - p.pos )
            _velSocial = c2 * np.random.random( p.vel.shape ) * ( self.m_bestParticle.pos - p.pos )

            p.vel = _velInertial + _velCognitive + _velSocial
            # clamp velocity
            _v = np.linalg.norm( p.vel )
            if _v > 0.0001 :
                _vClipped = np.clip( _v, vmin, vmax )
                p.vel = ( p.vel / _v ) * _vClipped

            p.pos = p.pos + p.vel
            p.pos = np.clip( p.pos, self.m_xmin, self.m_xmax )

            # Evaluate each particle 
            # TODO: Should vectorize to speed up calculation
            p.cost = self.m_refFunctionTarget( p.pos )

            if self._isBetter( p.cost, p.pBestCost ) :
                p.pBestCost = p.cost
                p.pBestPos = np.copy( p.pos )

                if self._isBetter( p.cost, self.m_bestParticle.cost ) :
                    # Using shared reference to best - is it safe? - it seems so
                    self.m_bestParticle.cost = p.cost
                    self.m_bestParticle.pos = p.pos

        self.m_avgPosition = np.zeros( ( 1, self.m_ndim ) )

        for i in range( len( self.m_particles ) ) :
            self.m_avgPosition += self.m_particles[i].pos

        self.m_avgPosition = self.m_avgPosition / self.m_populationSize

        print( 'bestPos: ', self.m_bestParticle.pos )
        print( 'bestCost: ', self.m_bestParticle.cost )
        print( 'avgPosition: ', self.m_avgPosition )

        # plot stuff
        for p in self.m_particles :

            self.m_refFunctionTarget.plotSingle( p.pos )

    def _end( self ) :

        pass