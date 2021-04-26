import os
from building import *

objs = []
cwd  = GetCurrentDir()

bluetooth_dir = 'zephyr/subsys/bluetooth'

# bluetooth core
bluetooth_core = []


src = bluetooth_core

CPPPATH = [cwd]

group = DefineGroup('ZephyrBLE', src, depend = [''], CPPPATH = CPPPATH)
# depend = ['PKG_USING_ZEPHYRBLE'], 

group = group + SConscript(os.path.join('rtthread-port', 'SConscript'))

Return('group')
