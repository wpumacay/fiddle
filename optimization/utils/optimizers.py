
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

		self.m_refFunctionTarget = None
		self.m_refPlotFig = None
		self.m_refPlotAxes = None
		self.m_refPlotFigContour = None
		self.m_refPlotAxesContour = None

		self.m_currentBest = None

	def setFunctionTarget( self, fcnTarget ) :

		self.m_refFunctionTarget = fcnTarget

		self.m_refPlotFig = fcnTarget.fig()
		self.m_refPlotAxes = fcnTarget.axes()
		self.m_refPlotFigContour = fcnTarget.figContour()
		self.m_refPlotAxesContour = fcnTarget.axesContour()

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


	def __init__( self, ndim, xmin, xmax, populationSize, w, c1, c2, maxIter = 100 ) :

		super( PSOoptimizer, self ).__init__( self, ndim, xmin, xmax, maxIter )

		self.m_populationSize = populationSize
		self.m_particles = [ PSOparticle( ndim ) for _ in range( populationSize ) ]

		self.m_bestParticle = None

		self.m_w = w
		self.m_c1 = c1
		self.m_c2 = c2

	def _start( self ) :
		# initialize population

		self.m_bestParticle = None

		for p in self.m_particles :

			p.vel = np.random.uniform( 0.0, 0.1, ( 1, self.m_ndim ) )
			p.pos = np.random.uniform( self.m_xmin, self.m_xmax, ( 1, self.m_ndim ) )
			p.cost = self.m_refFunctionTarget( p.pos )
			p.pBestPos = p.pos
			p.pBestCost = p.cost

			if self.m_bestParticle is None :
				self.m_bestParticle = p

			elif self.m_bestParticle.cost > p.cost :
				self.m_bestParticle = p

	def _step( self ) :

		w = self.m_w
		c1 = self.m_c1
		c2 = self.m_c2

		# Asynchronus PSO implementation

		for p in self.m_particles :

			# Update each particle
			p.vel = w * p.vel + 
					c1 * np.random.random( p.vel.shape ) * ( p.pBestPos - p.pos ) +
					c2 * np.random.random( p.vel.shape ) * ( self.m_bestParticle.pos - p.pos )
			p.pos = p.pos + p.vel

			# Evaluate each particle 
			# TODO: Should vectorize to speed up calculation
			p.cost = self.m_refFunctionTarget( p.pos )

			if p.cost <= p.pBestCost :
				p.pBestCost = p.cost
				p.pBestPos = np.copy( p.pos )

				if p.cost <= self.m_bestParticle.cost :
					# Using shared reference to best - is it safe? - it seems so
					self.m_bestParticle = p


	def _end( self ) :

		pass