# python

import os
from SCons.Script.SConscript import SConsEnvironment

# Sample: inherit Environment. ( Not necessary in most cases. )
#
class MyEnvironment(SConsEnvironment):
  def __init__(self, **kw):
    SConsEnvironment.__init__(self, **kw)
    self.Append(CXXFLAGS='-std=c++11')
  def newMethod():
    pass

env = MyEnvironment()

# Sample: add customization items into environment
env['X_RUNTIME_PATH'] = os.path.join( os.path.abspath('.'), 'runtime' )   # absolute path
env['X_RUNTIME_DSO_DIR'] = 'dso'
env['X_RUNTIME_DSO_PATH'] = os.path.join( env['X_RUNTIME_PATH'], env['X_RUNTIME_DSO_DIR'] )

# Sample: add method into environment
#
def MyPrint(env):
  print("Hello, MyPrint.\nCXXFLAGS=%s" % env['CXXFLAGS'])
  print('runtime path = [%s], dso path = [%s]' % ( env['X_RUNTIME_PATH'], env['X_RUNTIME_DSO_PATH'] ))
env.AddMethod(MyPrint,'MyPrint')

# Sample: add option
#
AddOption("--colorcc",
          action="store_true",
          dest="colorcc",
          default=False,
          help="Outout error/warning with color")
# Note: gcc 4.9 will add color support : -fdiagnostics-color=auto
if GetOption("colorcc"):
  env.Replace(CC='colorgcc')
  env.Replace(CXX='colorgcc')
  env.Append(LIBS='stdc++') 
  # The following is required as SCons wiki.
  env['ENV'] = {'PATH' : os.environ['PATH'],
                'TERM' : os.environ['TERM'],
                'HOME' : os.environ['HOME']}

# Sample: export method
#
Export('env')

VariantDir('build', 'src', duplicate=0)
SConscript('build/SConscript')
# Note: could also be the following:
# SConscript('src/SConscript', variant_dir='build', duplicate=0)

