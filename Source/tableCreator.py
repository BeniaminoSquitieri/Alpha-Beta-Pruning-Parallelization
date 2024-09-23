import matplotlib.pyplot as plt
import matplotlib
import os
import shutil

def makeTable(sourceFile, outputPath, outputName):

    f = open(sourceFile,"r")
    col = f.readline().strip()[:-1].split(";")
    col.pop(0)
    col.append("Speedup")

    row = []
    cell = []

    lines = f.readlines()

    seqTime = 0 
    parallelTime = 0

    for line in lines: 
        elem = line.strip()[:-1].split(";")
        modality = elem.pop(0)
        row.append(modality)
        new = []
        new.append(elem.pop(0))
        for val in elem:
            new.append(str(format(float(val), ".7f")))
        if modality == "Sequential":
            seqTime = float(elem[0])
            new.append("1")
        else:
            parallelTime = float(elem[0])
            new.append(str(format((seqTime/parallelTime),".7f")))
        cell.append(new)
    
    fig, ax = plt.subplots() 
    ax.set_axis_off() 
    table = ax.table(
        cellText = cell,  
        rowLabels = row,  
        colLabels = col, 
        rowColours = ["lightgrey"] * len(row),  
        colColours = ["lightgrey"] * len(col), 
        cellLoc = 'center',  
        loc = 'upper center',
        rowLoc = "center",
        colLoc = "center")

    table.scale(1,1)
    
    plt.savefig(os.path.join(outputPath, outputName)+".png",dpi=300, bbox_inches='tight')
    plt.close()
    f.close()

if os.path.exists("Tables"): # if Tables directory exist, delete it and all its elements
    shutil.rmtree('Tables')

os.mkdir("Tables") # create the tables directory

for path, currentDirectory, files in os.walk("Results"):
    for file in files:
        resultPath=path.replace("Results","Tables")
        if not(os.path.exists(resultPath)):
            os.makedirs(resultPath)
        makeTable(os.path.join(path, file),resultPath,file.replace(".csv",""))