import matplotlib.pyplot as plt
import numpy as np
import os
import shutil

def makePlot(sourceFile,outputPath,outputName):
    f=open(sourceFile,"r")

    col=f.readline().strip()[:-1].split(";")
    col.pop(0)

    lines=f.readlines()

    sequential=0
    openMP=dict()
    
    for line in lines:
        elem=line.strip()[:-1].split(";")
        elem.pop(0)
        omp=int(elem.pop(0))
 
        if(omp==0 ):
            sequential=float(elem[0])
        else:
            openMP[omp]=sequential/float(elem[0])
        
    #openMP
    x=[]
    y=[]
    xIdeal=[1,2,3,4,5,6,7,8,9]
    ompList=list(openMP.keys())
    ompList.sort()

    for omp in ompList:
        x.append(omp)
        y.append(openMP[omp])

    xpoints = np.array(x)
    ypoints = np.array(y)
    plt.xlabel('Threads') 
    plt.ylabel('Speedup') 
    plt.title('OpenMP')
    plt.plot(xpoints, ypoints, marker="o", label="real",  linewidth='1', ms=3)
    plt.plot(xIdeal, xIdeal, marker="o", label="ideal",  linewidth='1', ms=3)
    plt.grid()
    plt.tick_params()
    plt.legend()
    plt.savefig(os.path.join(outputPath, outputName)+".png", bbox_inches='tight')
    plt.close()
    f.close()

if os.path.exists("Plots"): # if Plots directory exist, delete it and all its elements
    shutil.rmtree('Plots')

os.mkdir("Plots") # create the plots directory

for path, currentDirectory, files in os.walk("Results"):
    for file in files:
        resultPath=path.replace("Results","Plots")
        if not(os.path.exists(resultPath)):
            os.makedirs(resultPath)
        makePlot(os.path.join(path, file),resultPath,file.replace(".csv",""))