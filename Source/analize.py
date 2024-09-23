import os
import shutil

def analizeFile(sourceFile,resultFile): # analize the source file and save the information in the result file
    f=open(sourceFile,"r") # open file and read all its lines
    lines=f.readlines()
    data={}
    num={}
    for line in lines: # for each line get the informations
        
        type,omp,totalTime=line.strip()[:-1].split(";")
        if type not in data:
            data[type]={} 
            num[type]={}
        if omp not in data[type]:
            data[type][omp]=float(totalTime)
            num[type][omp]= 1
        else:
            data[type][omp]+=float(totalTime)
            num[type][omp] += 1
    f.close() # close the source file and open the result file
    f=open(resultFile,"w")
    f.write("Modality; Threads; Time(s);\n")
    for typeKey in data.keys(): # for each element in the dictionary calculate the mean and save it in the result file
        ompList=list(data[typeKey].keys())
        ompList.sort()
        for ompKey in ompList:
            res=data[typeKey][ompKey]/num[type][omp]
            f.write(typeKey+"; "+str(ompKey)+"; "+str(res)+";\n")
    f.close() # close the result file

if os.path.exists("Results"): # if Result directory exist, delete it and all its elements
    shutil.rmtree('Results')

os.mkdir("Results") # create the result directory

for path, currentDirectory, files in os.walk("Informations"): # for each file in the directory Informations, analize them and save the results in the relative path in Results directory 
    for file in files:
        resultPath=path.replace("Informations","Results")
        if not(os.path.exists(resultPath)):
            os.makedirs(resultPath)
        analizeFile(os.path.join(path, file),os.path.join(resultPath,file))