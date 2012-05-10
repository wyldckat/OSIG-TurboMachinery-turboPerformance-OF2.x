#!/usr/bin/env python

''' 
A tool for visualizing turboPerformance functionObject data.
Author: Mikko Auvinen, Aalto University
Usage: Place the program in your ~HOME/bin directory and make it executable.
In the OpenFOAM case directory, run:
analyzeTurbo.py 
or 
analyzeTurbo.py  [rpm] [time at 0-angle] 
'''

import os, sys
import subprocess
import math
import operator   # for tuple sorting
import pylab as pl

# - - - Function definitions - - - -

def addToPlots( t, H, Tw, eta, Frx, Fry, Frz):

    pl.figure(1);
    pl.plot( t , H )  # fig1 active
    pl.figure(2); 
    pl.plot( t, Tw )   # fig2 active    
    pl.figure(3); 
    pl.plot( t, eta )   # fig3 active	    
    pl.figure(4); 
    pl.plot( t, Frx, t, Fry ) # fig4 active, (Fz omitted here!)     

# - - - - - - - - - - - - - - - 

def appendDataArrays( a1, a2, a3, a4, a5, a6, a7, timeDir ):
    # Read data
    fileName = timeDir+'/turboPerformance.dat'
    #a1b, a2b, a3b, a4b, a5b, a6b, a7b = pl.load(fileName,skiprows=1,usecols=(0,1,2,3,4,5,6),unpack=True)
    a1b, a2b, a3b, a4b, a5b, a6b, a7b = pl.mlab.load(fileName,skiprows=1,usecols=(0,1,2,3,4,5,6),unpack=True)
    # Append data    
    a1 = pl.append(a1 , a1b)
    a2 = pl.append(a2 , a2b)
    a3 = pl.append(a3 , a3b)
    a4 = pl.append(a4 , a4b)
    a5 = pl.append(a5 , a5b)
    a6 = pl.append(a6 , a6b)
    a7 = pl.append(a7 , a7b)
    return a1, a2, a3, a4, a5, a6, a7

# - - - - - - - - - - - - - - - 

def writeXY( xStart , x , y , fileName ):
    f = open( fileName ,'w')             #'w' = for writing
    for i in xrange(len(x)):
        if( x[i] >= xStart ):
            f.write("%f \t %f \n" %((x[i]-xStart), y[i]) )
    print 'Writing file '+fileName
    f.close()

# - - - - - - - - - - - - - - - 

def writeXYV( xStart , x , y1 , y2, y3, fileName ):
    f = open( fileName ,'w')             #'w' = for writing
    for i in xrange(len(x)):
        if( x[i] >= xStart ):
            f.write("%f \t %f \t %f \t %f \n" %((x[i]-xStart), y1[i], y2[i], y3[i] ) )
    print 'Writing file '+fileName
    f.close()

# - - - - - - - - - - - - - - - 
def averageValue( t, var, rpm ):
    timePerRev = 60./rpm
    t2 = t[-1]
    t1 = t2 - timePerRev
    if(t1 < t[0]):
        return 'NaN'

    minDiff = 1.E6
    iStop   = len(t)
    iStart  = 0
    for i in xrange(iStop):
        diff = abs( t[i]-t1 )
        if( diff < minDiff ):
            iStart = i; minDiff = diff

    return (sum(var[iStart:iStop])/float(iStop-iStart))
        

# - - - Main program - - - - -

try:
    rpm      = sys.argv[1]
    zeroTime =  sys.argv[2]
except:
    rpm      = 1470.   # My default
    zeroTime = 0.


# string to float
rpm      = float(rpm)
zeroTime = float(zeroTime) 

# Create plots
pl.figure(1); 
pl.ylabel(' Head (m) ');   pl.xlabel(' Time (s) '); pl.title(' Head vs. Time ')
pl.grid(True) 
pl.figure(2); 
pl.ylabel(' TOmega (W) '); pl.xlabel(' Time (s) '); pl.title(' Shaft Power vs. Time ')
pl.grid(True) 
pl.figure(3); 
pl.ylabel(' Eff (%) '); pl.xlabel(' Time (s) '); pl.title(' Efficiency vs. Time ')
pl.grid(True) 
pl.figure(4); 
pl.ylabel(' Forces (N) '); pl.xlabel(' Time (s) '); pl.title(' Axial Forces vs. Time ')
pl.grid(True) 


# Initialize the arrays
time = pl.array([]);
head = pl.array([]); TOmega = pl.array([]); eff = pl.array([]);
Fx   = pl.array([]); Fy     = pl.array([]); Fz  = pl.array([]);


os.chdir("./turboPerformance")    # cd ./turboPerformance
dirList = os.listdir(".")         # Obtain the list of directories 
#dirList.sort()                   # Sort the list - this won't work properly for floats (in string format) 

# We need a more robust sorting which uses numerical values
# This is accomplished by using tuples.
dirTuple = []
for dirStr in dirList:
    dirTuple.append((dirStr, float(dirStr)) ) # (str,float)

# Sort according to the numerical values: key=operator.itemgetter(1)
sortedTuple = sorted(dirTuple, key=operator.itemgetter(1))

# Use the sorted tuple list and pass the string value (tpl[0]) to the function
for tpl in sortedTuple:
    time, head, TOmega, eff, Fx, Fy, Fz = \
    appendDataArrays( time, head, TOmega, eff, Fx, Fy, Fz, tpl[0])	

# Plot the data arrays
addToPlots(time, head, TOmega, eff, Fx, Fy, Fz)

# If zeroTime is provided, go back to starting directory and write files
os.chdir("../.")   
if(zeroTime != 0.):
    writeXY(zeroTime, time, head , 'head.dat')
    writeXY(zeroTime, time, TOmega , 'TOmega.dat')
    writeXY(zeroTime, time, eff , 'eff.dat')
    writeXYV(zeroTime, time, Fx, Fy, Fz , 'Forces.dat')

# Write Average values to the screen
print " Head_avg (m)   = ", averageValue( time, head , rpm )
print " TOmega_avg (W) = ", averageValue( time, TOmega , rpm )
print " Eff_avg (%)    = ", averageValue( time, eff , rpm )

pl.show()
