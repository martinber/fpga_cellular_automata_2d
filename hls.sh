#!/usr/bin/env bash

export XILINXD_LICENSE_FILE=2100@atlas.itk.ppke.hu
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LIBRARY_PATH
source /media/mbernardi/datos/extra/async/ipcv/fpga/xilinx/Vivado/2019.2/settings64.sh
vivado_hls
