import argparse
from collections import OrderedDict
import os
import subprocess
import sys

# argparse

def dir_path(string):
    if os.path.isdir(string):
        return string
    else:
        raise NotADirectoryError(string)

parser = argparse.ArgumentParser(
                    prog='bldd',
                    description='Show all EXECUTABLE files that use specified shared library files.',
                    epilog='Example: python3 bldd.py -d /bin -l libc.so.6 -o prettyreport.txt')

parser.add_argument('-l', '--lib', metavar='./mylib.so',
                    type=str, help='Library file to check when searching for executables. By default, every lib is taken into account.',
                    required=False)
parser.add_argument('-d', '--directory', metavar='/my/superb/dir',
                    type=dir_path, help='Scan directory. Default is \'./test\'.', default='./test', required=False)
parser.add_argument('-o', '--output', metavar='myreport.txt',
                    type=str, help='Output report file name. Default is \'report.txt\'.', default='report.txt', required=False)
args = parser.parse_args()

#
# main

import sys
sys.stdout = open(args.output, 'w')

dictArchLibExecs = {}

def getArch(exePath):
    return subprocess.getoutput(f'readelf {exePath} -h 2>/dev/null | grep Machine: | awk \'{{for (i=2; i<NF; i++) printf $i " "; print $NF}}\'')

def getLibs(exePath):
    return subprocess.getoutput(f'readelf {exePath} -d 2>/dev/null | grep NEEDED | awk \'{{for (i=5; i<NF; i++) printf $i " "; print $NF}}\'').split(sep='\n')

for path, subdirs, files in os.walk(args.directory):
    for name in files:
        exePath = os.path.abspath(os.path.join(path, name))
        exeArch = getArch(exePath)
        exeLibs = [x[1:-1] for x in getLibs(exePath)]
        if exeArch not in dictArchLibExecs.keys():
            dictArchLibExecs[exeArch] = OrderedDict()
        for l in exeLibs:
            if l not in dictArchLibExecs[exeArch].keys():
                dictArchLibExecs[exeArch][l] = []
            dictArchLibExecs[exeArch][l].append(exePath)

for arch in dictArchLibExecs.keys():
    for l in sorted(dictArchLibExecs[arch], key=lambda k: len(dictArchLibExecs[arch][k]), reverse=True):
        dictArchLibExecs[arch].move_to_end(l)

for arch in dictArchLibExecs.keys():
    if args.lib is None or len([x for x in dictArchLibExecs[arch] if x == args.lib]) > 0:
        print(arch)
        for l in dictArchLibExecs[arch]:
            if args.lib is None or l == args.lib:
                print(f'\t{l} ({len(dictArchLibExecs[arch][l])} execs)')
                for e in dictArchLibExecs[arch][l]:
                    print(f'\t\t{e}')