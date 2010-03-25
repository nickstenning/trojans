#!/usr/bin/env python

import os, commands, re
from fabricate import *

compiler = commands.getoutput('which llvm-g++ || echo -n "g++"')
         
target  = 'trojans'
sources = ['trojans', 'simulator', 'particle', 'point', 'constants']
pkgs    = ['gsl']
        
cflags  = '''-g -O2 -ansi -pedantic -Wall -Wextra -Wconversion -Wshadow 
              -Wpointer-arith -Wcast-qual -Wwrite-strings
              -D__USE_FIXED_PROTOTYPES__ '''
libs    = ''
              
if any(pkgs):
    cflags += commands.getoutput('pkg-config --cflags ' + ''.join(pkgs))
    libs   += commands.getoutput('pkg-config --libs ' + ''.join(pkgs))

def build():
    compile()
    link()

def oname(filename):
    return os.path.join('build', os.path.basename(filename))

def compile(flags=None):
    for source in sources:
        run(compiler, '-c', source+'.cc', '-o', oname(source+'.o'), cflags.split(), flags)

def link(flags=None):
    objects = [oname(s+'.o') for s in sources]
    run(compiler, objects, '-o', oname(target), libs.split(), flags)
    
def clean():
    autoclean()

def rebuild():
    clean()
    build()

class QuieterBuilder(Builder):
    def echo_command(self, command):
        compiling = re.compile(r'-c (\S+\.cc)\b', re.IGNORECASE).search(command)
        linking   = re.compile(r'((?:\S+\.o\s+)+)-o (\S+)\b', re.IGNORECASE).search(command)
        if compiling:
            self.echo('CC ' + compiling.group(1))
        elif linking:
            # self.echo('LD ' + linking.group(1) + '-> ' + linking.group(2))
            self.echo('LD ' + linking.group(2))
        else:
            self.echo(command)

setup(builder=QuieterBuilder)
main()
