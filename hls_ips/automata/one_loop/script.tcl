############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project automata
set_top automata_hw
add_files automata/src/automata.h
add_files automata/src/automata.cpp
add_files -tb automata/src/automata_tb.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "one_loop"
set_part {xc7z020-clg484-1}
create_clock -period 10 -name default
config_sdx -target none
config_export -vivado_optimization_level 2 -vivado_phys_opt place -vivado_report_level 0
set_clock_uncertainty 12.5%
#source "./automata/one_loop/directives.tcl"
csim_design
csynth_design
cosim_design -trace_level port -rtl vhdl
export_design -rtl verilog -format ip_catalog
