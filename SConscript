Import('RTT_ROOT')
from building import *

cwd = GetCurrentDir()
path_port = 'ports/rtthread/'
src = Glob('src/bacnet/*.c') + \
      Glob('src/bacnet/basic/*.c') + \
      Glob('src/bacnet/basic/binding/*.c') + \
      Glob('src/bacnet/basic/service/*.c') + \
      Glob('src/bacnet/basic/npdu/*.c') + \
      Glob('src/bacnet/basic/sys/*.c') + \
      Glob('src/bacnet/basic/tsm/*.c') + \
      Glob(path_port + 'bacnet.c') + \
      Glob(path_port + 'device.c') + \
      Glob(path_port + 'mstimer-init.c') + \
      Glob(path_port + 'netport.c') + \
      Glob(path_port + 'bi.c') + \
      Glob(path_port + 'bo.c')

src_mstp = Glob(path_port + 'rs485.c') + \
           Glob(path_port + 'dlmstp.c') + \
           Glob('src/bacnet/datalink/mstp.c') + \
           Glob('src/bacnet/datalink/mstptext.c') + \
           Glob('src/bacnet/datalink/crc.c')

src_bip = Glob(path_port + 'bip-init.c') + \
          Glob('src/bacnet/datalink/bvlc.c') + \
          Glob('src/bacnet/basic/bbmd/h_bbmd.c')

path = [cwd + '/src', cwd + '/' + path_port]
defines = ['MAX_TSM_TRANSACTIONS=0']

if GetDepend(['PKG_BACNET_MSTP']):
    src += src_mstp
    defines += ['BACDL_MSTP=1']

if GetDepend(['PKG_BACNET_BIP']):
    src += src_bip
    defines += ['BACDL_BIP=1']

group = DefineGroup('bacnet', src, depend = ['PKG_USING_BACNET'], CPPPATH = path, CPPDEFINES = defines)

Return('group')
