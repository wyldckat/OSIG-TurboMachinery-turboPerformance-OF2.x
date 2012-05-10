#!/usr/bin/env python

''' 
A really simple viewing script for turboPerformance functionObject.
Author: Mikko Auvinen, Aalto University
Usage: Place the program in your ~HOME/bin directory and make it executable.
In the OpenFOAM case directory, run:
plotTurbo.py  
'''

import os, sys
import subprocess
import math
import pylab as pl

# - - - Function definitions - - - -

def addToPlots( timeName ):
	fileName = timeName+'/turboPerformance.dat'
	#time, head, TOmega, eff, Fx, Fy, Fz = pl.load( fileName ,skiprows=1,usecols=(0,1,2,3,4,5,6),unpack=True )
	time, head, TOmega, eff, Fx, Fy, Fz = pl.mlab.load( fileName ,skiprows=1,usecols=(0,1,2,3,4,5,6),unpack=True )
	pl.figure(1);
	pl.plot( time, head )  # fig1 active
	pl.figure(2); 
	pl.plot( time, TOmega )# fig2 active
	pl.figure(3); 
	pl.plot( time, eff )   # fig3 active	
	pl.figure(4); 
	pl.plot( time, Fx, time, Fy )# fig4 active, Note, Fz is not included (throws the scale off)    


# - - - Main program - - - - -

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


os.chdir("./turboPerformance")    # cd ./turboPerformance
#print os.listdir(".")

for dirStr in os.listdir("."):
	print dirStr
	addToPlots( dirStr )
	
pl.show()
