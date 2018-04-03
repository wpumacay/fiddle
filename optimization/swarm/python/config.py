
from __future__ import print_function

import numpy as np
import sys

import optimizers
import benchmarks

# Objective functions config ####################################

class ConfigFcnSphere :

    def __init__( self, ndim ) :

        self.ID = 'Sphere'
        self.DIMENSION = ndim
        self.MINIMIZE = True
        self.GLOBAL_OPTIMA_POSITION = np.zeros( ( 1, ndim ) )
        self.GLOBAL_OPTIMA_VALUE = 0.0
        self.DOMAIN = [ -10, 10 ]
        self.RANGE = [ 0, 100 ]

class ConfigFcnAckley :

    def __init__( self, ndim ) :

        self.ID = 'Ackley'
        self.DIMENSION = ndim
        self.MINIMIZE = True
        self.GLOBAL_OPTIMA_POSITION = np.zeros( ( 1, ndim ) )
        self.GLOBAL_OPTIMA_VALUE = 0.0
        self.DOMAIN = [ -32, 32 ]

        self.A = 20
        self.B = 0.2
        self.C = 2.0 * np.pi
        self.RANGE = [ 0, 20 ]

class ConfigFcnSchwefel :

    def __init__( self, ndim ) :

        self.ID = 'Schwefel'
        self.DIMENSION = ndim
        self.MINIMIZE = True
        self.GLOBAL_OPTIMA_POSITION = 420.9687 * np.ones( ( 1, ndim ) )
        self.GLOBAL_OPTIMA_VALUE = 0.0
        self.DOMAIN = [ -500, 500 ]
        self.RANGE = [ 0, 2000 ]

class ConfigFcnShafferFcn6 :

    def __init__( self, ndim ) :

        self.ID = 'ShafferFcn6'
        self.DIMENSION = ndim
        self.MINIMIZE = False
        self.GLOBAL_OPTIMA_POSITION = np.zeros( ( 1, ndim ) )
        self.GLOBAL_OPTIMA_VALUE = 1.0
        self.DOMAIN = [ -10, 10 ]
        self.RANGE = [ 0, 1 ]

# ###############################################################

# Optimizers config #############################################

class ConfigOptPSO :

    def __init__( self, ndim ) :

        self.ID = 'PSO'
        self.POPULATION_SIZE = 200
        self.W = 1
        self.C1 = 2
        self.C2 = 2
        self.K = 0.5

# ###############################################################

def GetFcnConfig( cId, ndim ) :

    if cId == 'Sphere' :
        return ConfigFcnSphere( ndim )
    elif cId == 'Ackley' :
        return ConfigFcnAckley( ndim )
    elif cId == 'Schwefel' :
        return ConfigFcnSchwefel( ndim )
    elif cId == 'ShafferFcn6' :
        return ConfigFcnShafferFcn6( ndim )

    else :
        print( 'Obj. Fcn. config requested : ', cId, ' was not found' )
        sys.exit()

def GetFcnObj( cId, ndim ) :

    _cfgFcn = GetFcnConfig( cId, ndim )
    _fcnObj = None

    if cId == 'Sphere' :
        _fcnObj = benchmarks.BMSphere( ndim, 
                                       plottingMode = benchmarks.BenchmarkFunction.PLOTTING_MODE_SURFACE,
                                       xmin = _cfgFcn.DOMAIN[0],
                                       xmax = _cfgFcn.DOMAIN[1] )
    elif cId == 'Ackley' :
        _fcnObj = benchmarks.BMAckley( _cfgFcn.A, _cfgFcn.B, _cfgFcn.C, ndim, 
                                       plottingMode = benchmarks.BenchmarkFunction.PLOTTING_MODE_SURFACE,
                                       xmin = _cfgFcn.DOMAIN[0],
                                       xmax = _cfgFcn.DOMAIN[1] )
    elif cId == 'Schwefel' :
        _fcnObj = benchmarks.BMSchwefel( ndim, 
                                         plottingMode = benchmarks.BenchmarkFunction.PLOTTING_MODE_SURFACE,
                                         xmin = _cfgFcn.DOMAIN[0],
                                         xmax = _cfgFcn.DOMAIN[1] )
    elif cId == 'ShafferFcn6' :
        _fcnObj = benchmarks.BMShafferFcn6( ndim, 
                                            plottingMode = benchmarks.BenchmarkFunction.PLOTTING_MODE_SURFACE,
                                            xmin = _cfgFcn.DOMAIN[0],
                                            xmax = _cfgFcn.DOMAIN[1] )

    else :
        print( 'Obj. Fcn. with id : ', cId, ' was not found' )
        sys.exit()

    _fcnObj.setRange( _cfgFcn.RANGE[0], _cfgFcn.RANGE[1] )

    return _cfgFcn, _fcnObj

def GetOptConfig( cOptId, cfgFcn ) :

    if cOptId == 'PSO' :
        return ConfigOptPSO( cfgFcn.DIMENSION )

    else :
        print( 'Obj. Fcn. config requested : ', cOptId, ' was not found' )
        sys.exit()

def GetOptObj( cOptId, cfgFcn ) :

    _cfgOpt = GetOptConfig( cOptId, cfgFcn )
    _optObj = None

    if cOptId == 'PSO' :
        _optObj = optimizers.PSOoptimizer( cfgFcn.DIMENSION, 
                                           xmin = cfgFcn.DOMAIN[0],
                                           xmax = cfgFcn.DOMAIN[1],
                                           populationSize = _cfgOpt.POPULATION_SIZE,
                                           w = _cfgOpt.W,
                                           c1 = _cfgOpt.C1,
                                           c2 = _cfgOpt.C2,
                                           k = _cfgOpt.K )

    else :
        print( 'Obj. Opt. config requested : ', cOptId, ' was not found' )
        sys.exit()

    return _cfgOpt, _optObj

def InitializeProblem( cId, cOptId, ndim ) :

    _cfgFcn, _fcnObj = GetFcnObj( cId, ndim )
    _cfgOpt, _optObj = GetOptObj( cOptId, _cfgFcn )

    _optObj.setFunctionTarget( _fcnObj, _cfgFcn.MINIMIZE )

    return _fcnObj, _optObj, _cfgFcn, _cfgOpt
