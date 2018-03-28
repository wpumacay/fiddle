
from __future__ import print_function

import numpy as np
import matplotlib.pyplot as plt

from benchmarks import *
from optimizers import *

plt.ion()

G_DIMENSION = 2
G_DOMAIN_MIN = -10
G_DOMAIN_MAX = 10

G_PSO_POPULATION_SIZE = 100
G_PSO_W = 0.5
G_PSO_C1 = 2
G_PSO_C2 = 2

# _bmFunction = BMSphere( G_DIMENSION, BenchmarkFunction.PLOTTING_MODE_WIREFRAME,
#                         xmin = G_DOMAIN_MIN, xmax = G_DOMAIN_MAX )

_bmFunction = BMShafferFcn6( G_DIMENSION, BenchmarkFunction.PLOTTING_MODE_WIREFRAME,
                           xmin = G_DOMAIN_MIN, xmax = G_DOMAIN_MAX )

_optimizer = PSOoptimizer( G_DIMENSION, G_DOMAIN_MIN, G_DOMAIN_MAX, 
                           G_PSO_POPULATION_SIZE, 
                           G_PSO_W, G_PSO_C1, G_PSO_C2 )

_optimizer.setFunctionTarget( _bmFunction )

_optimizer._start()

while True :

    _bmFunction.axes().cla()
    _bmFunction.axesContour().cla()

    _bmFunction.plotBase()

    if not _optimizer._stopCondition() :
        _optimizer._step()

    _particles = _optimizer.particles()
    _bestParticle = _optimizer.bestParticle()
    _avgPosition = _optimizer.avgPosition()

    print( 'bestPos: ', _bestParticle.pos )
    print( 'bestCost: ', _bestParticle.cost )
    print( 'avgPosition: ', _avgPosition )

    for _p in _particles :
        _bmFunction.plotSingle( _p.pos )

    plt.pause( 0.02 )


