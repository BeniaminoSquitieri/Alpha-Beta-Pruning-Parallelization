import os
import shutil

if os.path.exists("Informations"): # if Informations directory exist, delete it and all its elements
    shutil.rmtree('Informations')

os.mkdir("Informations") # create the Informations directory

if os.path.exists("Results"): # if Results directory exist, delete it and all its elements
    shutil.rmtree('Results')
 
os.mkdir("Results") # create the Results directory

os.chdir("Informations") 

for i in range(4):
    opt="opt"+str(i)
    os.mkdir(opt)
    os.chdir(opt)
     

    os.chdir("..")

os.chdir("../Results") 

for i in range(4):
    opt="opt"+str(i) 
    os.mkdir(opt)
    os.chdir(opt)
    os.chdir("..")
    
 