
import numpy as np
import matplotlib.pyplot as plt

from benchmarks import *

plt.ion()

_bmFunction = BMSphere( 2, BenchmarkFunction.PLOTTING_MODE_WIREFRAME )
# _bmFunction = BMShafferFcn6( 2, BenchmarkFunction.PLOTTING_MODE_WIREFRAME )

_buff = []

while True :

    _bmFunction.axes().cla()

    _bmFunction.plotBase()

    _p = np.random.uniform( _bmFunction.min(), _bmFunction.max(), ( 1, _bmFunction.ndim() ) )
    _buff.append( _p )

    _bmFunction.plotSequence( np.reshape( _buff, ( len( _buff ), _bmFunction.ndim() ) ) )

    plt.pause( 0.02 )


