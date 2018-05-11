import argparse
import os
import datetime
import time
import re
import zipfile
import subprocess

filesList = []
num_of_files_visited = 0
size_of_files_visited = 0
size_of_unique_files = 0
num_of_unique_names = 0
num_of_unique_files = 0
#this function parses the arguments given using argparse and returns a list that contains options and their data
def parseArguments():
    parser = argparse.ArgumentParser(description='file list project')
    parser.add_argument('-before', dest="BeforeDate", action='store')
    parser.add_argument('-after', dest="AfterDate", action='store')
    parser.add_argument('-match', dest="Pattern", action='store')
    parser.add_argument('-bigger', dest='BiggerThan', action='store')
    parser.add_argument('-smaller', dest='SmallerThan', action='store')
    parser.add_argument('-delete', dest='Delete', action='store_true', default=False)
    parser.add_argument('-zip' , dest='ZipFile', action='store')
    parser.add_argument('-duplcont', dest='DuplCont', action='store_true', default=False)
    parser.add_argument('-duplname', dest='DuplName', action='store_true', default=False)
    parser.add_argument('-stats', dest='Stats', action='store_true', default=False)
    parser.add_argument('-nofilelist', dest='NoFileList', action='store_true', default=False)
    parser.add_argument('extras', action='store', nargs='*')
    return parser.parse_args()
#this function takes a path and finds all files in or below this path and puts them into filelist
def traverse(path):
    path = os.path.abspath(path)
    global num_of_files_visited
    global size_of_files_visited
    for (dirpath, dirnames, filenames) in os.walk(path):
        num_of_files_visited += len(filenames)
        for file in filenames:
            size_of_files_visited += os.path.getsize(dirpath + '/' + file)
            filesList.append(dirpath + '/' + file)
#this function takes a time as parameter and returns unix timestamp standart of that time
def convertTime( date_time, default , hours , minutes ):
    if date_time == None:
        return default

    if len(date_time) == 8:
        dt = datetime.datetime(int(date_time[0:4]),int(date_time[4:6]),int(date_time[6:8]) , int(hours) , int(minutes) , 0 )
    else:
        dt = datetime.datetime(int(date_time[0:4]),int(date_time[4:6]),int(date_time[6:8]) , int(date_time[9:11]) , int(date_time[11:13]) , int(date_time[13:15]) )

    return time.mktime(dt.timetuple())
#this function takes a string named size and returns size as bytes
def convertSize( size , default ):
    if size == None:
        return default
    if size[len(size)-1] == 'K':
        return int(size[0:len(size)-1])*1024
    elif size[len(size)-1] == 'M':
        return int(size[0:len(size) - 1]) * 1024 * 1024
    elif size[len(size) - 1] == 'G':
        return int(size[0:len(size) - 1]) * 1024 * 1024 * 1024
    else:
        return int(size[0:len(size)])
#this function handles '-before', '-after', '-bigger', '-smaller' and '-match' options, takes a list of files and returns noneliminated files
def eliminate(files):
    hi_time=convertTime(arguments.BeforeDate , 9999999999 , 23 , 59 )
    lo_time=convertTime(arguments.AfterDate , 0 , 0 , 0 )
    lo_size=convertSize(arguments.BiggerThan , 0 )
    hi_size=convertSize(arguments.SmallerThan , 30*1024*1024*1024 )
    ret = []
    for file in files:
        mtime=os.path.getmtime(file)
        fsize=os.path.getsize(file)
        if arguments.Pattern:
            found=re.search(arguments.Pattern,file.split("/")[len(file.split("/"))-1] )
        else:
            found=True
        if mtime >= lo_time and mtime <= hi_time and fsize >= lo_size and fsize <= hi_size and found:
            ret.append(file)
    return ret
#this function zips all the files in the given list of paths
def zipThese(files):
    filenames = set()
    with zipfile.ZipFile(arguments.ZipFile,'w') as myzip:
        for file in files:
            filename = file.split("/")[len(file.split("/"))-1]
            if len(filename.split(".")) == 1:
                prev = filename
                next = None
            else:
                next = filename.split(".")[len(filename.split("."))-1]
                prev = filename[0:len(filename)-len(next)-1]
            x = filename
            counter = 1
            while x in filenames:
                if next:
                    x = prev + "(" + str(counter) + ")." + next
                else:
                    x = prev + "(" + str(counter) + ")"
                counter+=1
            myzip.write(file,x)
            filenames.add(x)
#this function deletes all files in the given list of paths
def deleteThese(files):
    for file in files:
        os.remove(file)
#this function finds files that have the same content
def handleDuplCont(files , write ):
    pair_list = []
    global num_of_unique_files
    global size_of_unique_files
    for file in files:
        cmd = 'shasum -a 256 '+ file
        hash = str(subprocess.check_output(cmd,shell=True)).split(" ")[0][2:]
        pair_list.append( (hash , file) )
    pair_list.sort()
    pair_list.append(("some dummy hash", "some dummy path name"))
    prev = "some dummy not equal hash"
    print_list = []
    return_list = []
    firstSet = True
    for hash,path in pair_list:
        if hash != prev:
            if hash != "some dummy hash":
                size_of_unique_files += os.path.getsize(path)
            num_of_unique_files += 1
            if (not firstSet) and write and len(print_list) > 1:
                print("--------------------------------------")
            if write and len(print_list) > 1:
                firstSet = False
                return_list.extend(print_list)
                print_list.sort()
                for prnt in print_list:
                    print( prnt )
            prev = hash
            print_list = []
        print_list.append(path)
    return return_list
#this function finds files that have the same name
def handleDuplName(files , write ):
    pair_list = []
    global num_of_unique_names
    for file in files:
        filename = file.split("/")[len(file.split("/")) - 1]
        pair_list.append( ( filename , file ) )
    pair_list.sort()
    pair_list.append(("some dummy filename" , "some dummy path name"))
    prev = "some dummy not equal name"
    print_list = []
    return_list = []
    firstSet = True
    for name,path in pair_list:
        if name != prev:
            num_of_unique_names += 1
            if (not firstSet) and write and len(print_list) > 1:
                print("--------------------------------------")
            if write and len(print_list) > 1:
                firstSet = False
                return_list.extend(print_list)
                for prnt in print_list:
                    print( prnt )
            prev = name
            print_list = []
        print_list.append(path)
    return return_list

############# MAIN ##############

arguments = parseArguments()
if len(arguments.extras) == 0:
    traverse(os.getcwd())
else:
    for path in arguments.extras:
        traverse(path)
#for taking only unique paths of files
files = set(filesList)

result = eliminate(files)

if arguments.ZipFile:
    zipThese(result)

if arguments.Delete:
    deleteThese(result)

if arguments.DuplCont:
    files = handleDuplCont(result , not arguments.NoFileList )
elif arguments.DuplName:
    files = handleDuplName(result , not arguments.NoFileList )
else:
    if not arguments.NoFileList:
        files = list(result)
        files.sort()
        for file in files:
            print(file)

size_of_listed_files = 0
num_of_listed_files = len(files)
for file in files:
    size_of_listed_files += os.path.getsize(file)
#print stats if the option is given 
if arguments.Stats:
    print("total number of files visited: "+str(num_of_files_visited) )
    print("total size of files visited in bytes: "+str(size_of_files_visited) )
    print("number of listed files: "+str(num_of_listed_files))
    print("total size of listed files in bytes: "+str(size_of_listed_files))
    if arguments.DuplName:
        print("number of files with unique names: "+str(num_of_unique_names-1))
    if arguments.DuplCont:
        print("number of unique files: " + str(num_of_unique_files - 1))
        print("total size of unique files in bytes: "+str(size_of_unique_files) )