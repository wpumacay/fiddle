
import numpy as np
import matplotlib.pyplot as plt

# calculate the fourier series of a square wave

class SquareWave :


    def __init__ ( self, nCoeffs = 100 ) :

        self.m_nCoeffs = nCoeffs

        self.m_an = []
        self.m_bn = []
        self.m_a0 = 0

        self.m_tMin = 0
        self.m_tMax = 2.0 * np.pi

        self.m_nSamples = 1000

        self._calculateFourierCoefficients()

    def eval( self, tt ) :

        ss = tt - ( tt // ( 2 * np.pi ) ) * 2 * np.pi
        xx = np.array( [ 1.0 if ( 0 <= t <= np.pi ) else 0.0 for t in tt ] )

        return xx

    def plot( self ) :
        
        # plot square wave

        tt = np.linspace( self.m_tMin, self.m_tMax, self.m_nSamples )
        xx = self.eval( tt )

        # plot fourier series

        xxp = self.m_a0 * np.ones( self.m_nSamples )

        for i in range( self.m_nCoeffs ) :
            xxp = xxp + self.m_an[i] * np.sin( ( i + 1 ) * tt ) + self.m_bn[i] * np.cos( ( i + 1 ) + tt )

        plt.plot( tt, xx, 'b' )
        plt.plot( tt, xxp, 'r' )
        plt.show()

    def _calculateFourierCoefficients( self ) :

        # a0 = <x(t)>, an = < x(t)sin(nt) >, bn = < x(t)cos(nt) >
        # a0 = 0.5, an = 2 / n, bn = 0

        self.m_a0 = 0.5
        self.m_an = [ 2.0 / ( np.pi * ( n + 1 ) ) if ( n + 1 ) % 2 == 1 else 0 for n in range( self.m_nCoeffs ) ]
        self.m_bn = [ 0.0 for n in range( self.m_nCoeffs )  ]
        
        ## print( 'an: ', self.m_an )
        ## print( 'bn: ', self.m_bn )

_swave = SquareWave( 10 )

_swave.plot()

