
import math
import matplotlib.pyplot as plt

import csv

# read points contents to numpy array ################
_xx = []
_yy = []
_ii = []
_fHandle = open( 'circle.csv', 'rb' )
_reader = csv.reader( _fHandle )

_rowCount = 0
for row in _reader :
    if _rowCount == 0 :
        _rowCount += 1
        continue
    
    _ii.append( int( row[0] ) )
    _xx.append( float( row[1] ) )
    _yy.append( float( row[2] ) )
    _rowCount += 1
    
#print _points
#print type( _points[0][0] )

_fHandle.close()

# according to the plot, the points are close to ...
# the unit circle, so we will assume a radius of 1
plt.figure(1)
plt.plot( _xx, _yy )
######################################################

# transform to polar coordinates #####################

def toPolar( xCoords, yCoords ) :
    _rCoords = []
    _tCoords = []
    
    assert( len( xCoords ) == len( yCoords ) )
    for i in range( len( xCoords ) ) :
        _r = math.sqrt( xCoords[i] ** 2 + yCoords[i] ** 2 )
        _t = math.atan2( yCoords[i], xCoords[i] )
        # transform the positive angles to negatives
        # resulting in [0, -2pi]
        if _t > 0 :
            _t = -2 * math.pi + _t

        _rCoords.append( _r )
        _tCoords.append( _t )
        # print the points and check manually the order
        print _t

    return _rCoords, _tCoords

_rr, _tt = toPolar( _xx, _yy )

class PolarPoint :
    
    def __init__( self, r, t, indx ) :
        self.r = r
        self.t = t
        self.indx = indx
    
    @staticmethod
    def getIndicesFromBuffer( points ) :
        return [ p.indx for p in points ]

# Sort the points and check which index is out of order

_pp = [ PolarPoint( _rr[i], _tt[i], i ) for i in range( len( _rr ) ) ]
_ppSorted = sorted( _pp, key = lambda ppoint : ppoint.t, reverse=True )
_ppIndices = PolarPoint.getIndicesFromBuffer( _ppSorted )
print 'indicesSorted'
print _ppIndices

for q in range( len( _ii ) ) :
    # check the first that is out of order
    if _ii[q] != _ppIndices[q] :
        print 'index out of order: ', _ppIndices[q]
        break

######################################################

plt.show()
