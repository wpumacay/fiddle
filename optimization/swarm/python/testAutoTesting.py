
from __future__ import print_function

import numpy as np
import matplotlib.pyplot as plt

import config

plt.ion()

# _bmFunction, _optimizer, _cfgFcn, _cfgOpt = config.InitializeProblem( 'Sphere', 'PSO', 2 )
# _bmFunction, _optimizer, _cfgFcn, _cfgOpt = config.InitializeProblem( 'Ackley', 'PSO', 2 )
_bmFunction, _optimizer, _cfgFcn, _cfgOpt = config.InitializeProblem( 'Schwefel', 'PSO', 2 )
# _bmFunction, _optimizer, _cfgFcn, _cfgOpt = config.InitializeProblem( 'ShafferFcn6', 'PSO', 2 )

_optimizer._start()

while True :

    if _bmFunction.axes() is not None :
        _bmFunction.axes().cla()

    if _bmFunction.axesContour() is not None :
        _bmFunction.axesContour().cla()

    _bmFunction.plotBase()

    if not _optimizer._stopCondition() :
        _optimizer._step()

    plt.pause( 0.02 )


