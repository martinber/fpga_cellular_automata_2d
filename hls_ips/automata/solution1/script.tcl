############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project automata
set_top automata_hw
add_files automata/src/automata.cpp
add_files automata/src/automata.h
add_files -tb automata/src/automata_tb.cpp
open_solution "solution1"
set_part {xc7z020-clg484-1}
create_clock -period 10 -name default
#source "./automata/solution1/directives.tcl"
csim_design
csynth_design
cosim_design -trace_level port -rtl vhdl
export_design -format ip_catalog
