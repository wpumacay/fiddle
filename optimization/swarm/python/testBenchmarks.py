
import numpy as np
import matplotlib.pyplot as plt

from benchmarks import *


# _bmSphere = BMSphere( 2, BenchmarkFunction.PLOTTING_MODE_WIREFRAME )
# _bmSphere = BMAckley( 20, 0.2, 2 * np.pi, 2, BenchmarkFunction.PLOTTING_MODE_WIREFRAME )
# _bmSphere = BMSchwefel( 2, BenchmarkFunction.PLOTTING_MODE_WIREFRAME )
_bmSphere = BMShafferFcn6( 2, BenchmarkFunction.PLOTTING_MODE_WIREFRAME )

_bmSphere.plotBase()


plt.show()
