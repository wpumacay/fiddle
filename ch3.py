
import math
import time


N = 123456
#N = 100008 # checking with this https://primes.utm.edu/lists/small/100000.txt
#N = 10

# in 20 millions we get the quota of primes required



# brute force the primes

def isPrime( n ) :

    if n == 2 :
        return True
    
    _maxCheck = int( math.sqrt( n ) )
    _check = 2

    for _ in range( _maxCheck ) :

        if n % _check == 0 :
            return False

        _check += 1

    return True

# brute force the primes with kind of Dynamic programming

def dpIsPrime( n, primesSoFar ) :

    if n == 2 :
        return True

    _maxCheck = math.sqrt( n )

    for prime in primesSoFar :

        if prime > _maxCheck :
            break

        if n % prime == 0 :
            return False

    return True


_numPrimesFound = 0
_sumPrimesFound = 0
_primes = []

_current = 2

_tStart = time.time()

while _numPrimesFound < N :

    if dpIsPrime( _current, _primes ) :
#    if isPrime( _current ) :
        _sumPrimesFound += _current
        _numPrimesFound += 1
        _primes.append( _current )

        print 'prime ', _numPrimesFound, " : ",  _current

    _current += 1

_tEnd = time.time()

print 'dt: ', ( _tEnd - _tStart )

print 'sum: ', _sumPrimesFound
