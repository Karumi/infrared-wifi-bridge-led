#!/usr/bin/env python
from __future__ import print_function

import os
import sys
import click
from pathlib import Path

Import("env")

WEB_DIRECTORY = "html"
CACHE_FILE = ".pio/build/web_cache"


class Color(object):
    BLACK = '\x1b[1;30m'
    RED = '\x1b[1;31m'
    GREEN = '\x1b[1;32m'
    YELLOW = '\x1b[1;33m'
    BLUE = '\x1b[1;34m'
    MAGENTA = '\x1b[1;35m'
    CYAN = '\x1b[1;36m'
    WHITE = '\x1b[1;37m'
    LIGHT_GREY = '\x1b[0;30m'
    LIGHT_RED = '\x1b[0;31m'
    LIGHT_GREEN = '\x1b[0;32m'
    LIGHT_YELLOW = '\x1b[0;33m'
    LIGHT_BLUE = '\x1b[0;34m'
    LIGHT_MAGENTA = '\x1b[0;35m'
    LIGHT_CYAN = '\x1b[0;36m'
    LIGHT_WHITE = '\x1b[0;37m'

def clr(color, text):
    return color + str(text) + '\x1b[0m'

def print_warning(message, color=Color.LIGHT_YELLOW):
    print(clr(color, message), file=sys.stderr)

def print_filler(fill, color=Color.WHITE, err=False):
    width, _ = click.get_terminal_size()
    if len(fill) > 1:
        fill = fill[0]

    out = sys.stderr if err else sys.stdout
    print(clr(color, fill * width), file=out)

def build_web():
    cmd = ["cd", WEB_DIRECTORY, "&&" ,"yarn", "build"]    

    print_warning("Build Website")
    env.Execute(" ".join(cmd))   

def getWebSize():
    root_directory = Path(WEB_DIRECTORY)
    return sum(f.stat().st_size for f in root_directory.glob('**/*') if f.is_file())

def check_web_cache():
    build_web = False
    actualSize = getWebSize()
    print_warning("Actual web directory size: " + str(actualSize))
    
    if os.path.isfile(CACHE_FILE):
        cache = open(CACHE_FILE,"r+")  
        size = cache.readline()    
        print_warning("Cache size: " + size)
        cache.close()
        build_web = int(size) != actualSize
    else:
        writeWebSize(actualSize)        
        build_web = True
        print_warning("Create cache file")

    return build_web

def writeWebSize(webSize):  
    cache = open(CACHE_FILE,"w+")
    cache.write(str(webSize)) 
    cache.close()
    
print_filler("*", color=Color.LIGHT_YELLOW, err=True)

should_build = check_web_cache()

if should_build:
    build_web()
    webSize = getWebSize()
    writeWebSize(webSize)
else:
    print_warning("Compile website from cache")

print_filler("*", color=Color.LIGHT_YELLOW, err=True)