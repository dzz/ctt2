#######################3
# BUILD NOTES
#
# needs sdl2, sdl_mixer, sdl_image, python-devel
#

import fnmatch
import os
import sys


build_debug_mode = True

def populate_c_matches(matches, dir):
    for root, dirnames, filenames in os.walk(dir):
        for filename in fnmatch.filter(filenames, '*.c'):
            matches.append(os.path.join(root, filename))

def populate_h_matches(matches, dir):
    for root, dirnames, filenames in os.walk(dir):
        for filename in fnmatch.filter(filenames, '*.h'):
            path = os.dirname(os.path.join( root, filename))
            if not path in matches:
                matches.append(path)

def configure_portaudio(includes):
    includes.append("/usr/include/portaudio/")

def configure_python(includes, libs):
    includes.append("/usr/include/python3.5/")
    libs.extend( [ 'python3.5m', 'pthread', 'dl', 'util', 'm' ] )
    
def configure_sdl(includes,libs):
    includes.append("/usr/include/SDL2/")
    libs.extend( ['SDL2' ] )

def configure_opengl(includes):
    includes.append("./gl_includes/")

Libs            = []
SourceFiles     = []
IncludePaths    = []

populate_c_matches(SourceFiles, "src")
populate_h_matches(IncludePaths, "DSPFilters/source")

configure_python(IncludePaths, Libs)
configure_sdl(IncludePaths, Libs)
configure_opengl(IncludePaths)
configure_portaudio(IncludePaths)

if build_debug_mode:
    print SourceFiles
    print IncludePaths

env = Environment()

if sys.platform == "linux2":
    CCFLAGS="-Wno-incompatible-pointer-types"
if sys.platform == "win32":
    ##CXXFLAGS="/EHsc /O2 /Ot /arch:AVX"
    CCFLAGS+""
    #env.Append(CXXFLAGS = ['/DEBUG'])

env.Program(
        'cli', 
        SourceFiles, 
        CPPPATH= IncludePaths,
        LIBPATH='.',
        LIBS=Libs,
        CCFLAGS=CCFLAGS
)

