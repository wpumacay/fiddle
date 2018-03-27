
from __future__ import print_function

import numpy as np
import matplotlib.pyplot as plt


class BaseOptimizer ( object ) :


	def __init__( self, ndim, maxIter = 100 ) :

		self.m_ndim = ndim

		self.m_currentNumIters = 0
		self.m_maxIter = maxIter

		self.m_refFunctionTarget = None
		self.m_refPlotFig = None
		self.m_refPlotAxes = None

		self.m_currentBest = None

	def setFunctionTarget( self, fcnTarget ) :

		self.m_refFunctionTarget = fcnTarget

		self.m_refPlotFig = fcnTarget.fig()
		self.m_refPlotAxes = fcnTarget.axes()

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
		self.vel = np.zeros( ( 1, ndim ) )


class PSOoptimizer( BaseOptimizer ) :


	def __init__( self, ndim, populationSize, maxIter = 100 ) :

		super( PSOoptimizer, self ).__init__( self, maxIter )

		self.m_populationSize = populationSize
		self.m_particles = [ PSOparticle( ndim ) for _ in range( ndim ) ]

	def _start( self ) :
		# initialize population


	def _step( self ) :

		pass

	def _end( self ) :

		pass