
import numpy as np
import random
import matplotlib.pyplot as plt
import matplotlib.image as mpimg


_img = mpimg.imread( 'mystery_number_scrambled.jpg' )


random.seed( 2 )

print 'shape: ', _img.shape

H = _img.shape[0]
W = _img.shape[1]
N = W * H

_indices = range( N )
random.shuffle( _indices )

# Check that the indices provided ...
# are the same as the generator
# print _indices[0:20]

def scrambleImage( imgIn, scrambleIndices, isInverse = False ) :
    
    _imgOut = np.zeros( imgIn.shape )

    _h, _w = _imgOut.shape[0], _imgOut.shape[1]
    _n = _w * _h

    assert( _n == len( scrambleIndices ) )

    for i in range( _n ) :
        _srcIndx = i
        _dstIndx = scrambleIndices[i]

        _xSrc = _srcIndx % _w
        _ySrc = _srcIndx / _w
        
        _xDst = _dstIndx % _w
        _yDst = _dstIndx / _w
       
        #print '(x, y) dst: ', _xDst, _yDst
        #print '(x, y) src: ', _xSrc, _ySrc
 
        if isInverse :
            _imgOut[_yDst, _xDst] = imgIn[_ySrc, _xSrc]
        else :
            _imgOut[_ySrc, _xSrc] = imgIn[_yDst, _xDst]

    return _imgOut

_imgRecovered = scrambleImage( _img, _indices, True )

plt.figure(1)
plt.imshow( _img, cmap='gray' )
plt.figure(2)
plt.imshow( _imgRecovered, cmap='gray' )
plt.show()


