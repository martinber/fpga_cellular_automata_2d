2D cellular automata
====================

Martin Bernardi

-------

Deadline: Last lecture

Do not use the Xilinx provided opencv. Otherwise use intaleed opencv. Maybe
create python script to save image as raw file

-------

Make it work at least in cosimulation. We can ask professor to use one of his
boards via teamviewer


-------

If we are using a nested loop to read elements, there might be an error because
the compiler does not know if an element is used only once.

If using linebuffer is used, we can use Vivado HLS

-------

If you pipeline a loop with subloops, all the subloops will be unrolled.

If you have 9 multiplications, it will be executed in parallel


-------

How to generate the 3x3 part from a pixel.

There are two loops, for rows and columns and will process one frame. There is
another loop if processing several frames

For the median it is better to have a 1 by 9 array

If you have the 3x3 array. Write two loops where you put pixel by pixel the
elements of the 3x3 into a 1x9 array

Two nexted lopps for the image

The to right structure is a 2x8 array. First copy lower to upper line, and then
copy element to the place. And finally go to next column. Upper line is line 0
and lower is 1.
First copy the two elements to the 3x3 array

Notes
=====

Directories
---------

- hls_ips
  - hls_ip_1
    - src
    - solution1
    - solution2
- vivado_ipi_src
- vivado_ipi
- vitis_sdk
```


Creating projects
-----------------

```

Abierto hls
Creada carepeta "./hls_ips/"
Creado proyecto
Nombre: "automata", carpeta "./hls_ips"
No agregado ningun archivo
En part selection apretar ...
Seleccionar zedboard zynq evaluation development kit
COn explorador archivos crear carpata "src" en "automata"
Click derecho en Sources > New file
   Agregar "automata.cpp", "automata.h"
Click derecho en Test Bench > New file
   Agregar "automata_tb.cpp"
Escribir codigo
Project\Project properties > Synthesis
Set Top Function: automata_hw
Run C Synthesis (boton de play)
Run C/RTL cosimulation (boton de ok)
    USAR Verilog en vez de VHDL
    Dumpl trace: port
Ver waveform (boton negro)
Export RTL (boton de paquete)


Abierto vivado
Creado proyecto
En carpeta ipi
RTL project, do not include sources
Board: zedboard
Create block design
Nombre: tcpip_main, en carpeta ipi_src
Agregar un bloque Zinq apretando boton +
Run block automation

Tools > Settings > IP > Repository
Apretar +
Agregar hls_ips/solution1/impl

Si hay errores con clock:
    Click derecho en bloque > Cusrtomize > PS/PL configuration > AXI non-secure > GP Master
    Desactivar interfaz

Generate bitstream (boton al lado de play)
Sources > click derecho en tcpip_main > create hdl wrapper
File > Export > Export Hardware
Seleccionar Include bitstream


Tools > Launch Vitis
Workspace poner en carpeta "sdk"
Create platform project, nombre "tcpip_platform"
Create from hardware specificationn (XSA)
Seleccionar ipi/tcpip_main_wrapper.xsa

Board Support Package. Apretar boton "Modify BSP Settings..."
En Overview, activar "lwip211" si se va a usar TCPIP
Apretar ok
Board Support Package. Apretar boton "Modify BSP Settings..."
Ir a opciones de lwip y desactivar las dos opciones en DHCP. Y poner API mode en SOCKET
En el Assistant, apretar martillo para hacer build

File > New > Application Project, nombre "tcpip_application"
Seleccionar plataforma "tcpip_platform"
Seleccionar C++
```

Running project from git
------------------------

```
cd a la carpeta principal del proyecto (no a la _hw)

vivado_hls -i

Pegar lineas de los script.tcl de cada solution

quit

berma11@VLSI-01:~/Desktop/hls/projects/vector_mult/vector_mult_hls/src$ vivado_hls

Abrir proyecto

Run synthesis on solutions
C/RTL cosiumlation
   Dump trace: Port
Open wave view

Compare solutions


Solution > Export Solution...
abrir "vivado"

Crear proyecto
Seleccionar board
Create block design
Add zynq
Automate implementation?
Doble click zynq. PS-PL interface
   ACP port es coherente, accede cache. Vamos a usar los otros pero vamos a tener que flushear el cache (con FlushRange) e invalidarlo en el procesador (con Xil_DInvalidateRange). Solamente flushear e invalidar datos memaligneados
Tools > Settings > IP > Repository > Add, seleccionar carpeta solucion
Agregar block
Run connection automation
Save

Sources
Block design
Surces > bloque > Create HDL Wrapper. Valores por defecto
Generate bitstream, 4 cores
Open implemented design
Close implemented design
File > Export > Hardware Design. Include bistream, adentro de _ipi dir
File > Launch Vitis
Crear directorio _sdk
Create platform project _platform
Archivoo .xsa en _ipi. Standalone por ahora, pero en el proyecto vamos a usar otro

Build platform
Carpeta export > hw? y esta el .h y .c de nuestro
Carpeta ps7_cortex > include > xil_cache.h

Create application project, _application
Usar mi propia plataforma
Seleccionar C o C++
Importar codigo del test_bench desde carpeta src > import sources > ir a _hls/src . OK, seleccionar los .cpp del test_bench

Debug > Build debug application
Prender FPGA
Abrir serial ttyACM0, 115200
Debug > Launch on Hardware
```

Error in debian
---------------

Done:

```
sudo ln -s /usr/lib/x86_64-linux-gnu/crt1.o /media/mbernardi/datos/extra/async/ipcv/fpga/xilinx/Vivado/2019.2/lnx64/tools/gcc/lib/gcc/x86_64-unknown-linux-gnu/4.6.3/
sudo ln -s /usr/lib/x86_64-linux-gnu/crti.o /media/mbernardi/datos/extra/async/ipcv/fpga/xilinx/Vivado/2019.2/lnx64/tools/gcc/lib/gcc/x86_64-unknown-linux-gnu/4.6.3/
sudo ln -s /usr/lib/x86_64-linux-gnu/crtn.o /media/mbernardi/datos/extra/async/ipcv/fpga/xilinx/Vivado/2019.2/lnx64/tools/gcc/lib/gcc/x86_64-unknown-linux-gnu/4.6.3/

» ls /media/mbernardi/datos/extra/async/ipcv/fpga/xilinx/Vivado/2019.2/lnx64/tools/gcc/lib/gcc/x86_64-unknown-linux-gnu/4.6.3/
32/      crtbegin.o   crtbeginT.o  crtendS.o      crti.o@  crtprec32.o  crtprec80.o  include-fixed/  libgcc.a     libgcov.a
crt1.o@  crtbeginS.o  crtend.o     crtfastmath.o  crtn.o@  crtprec64.o  include/     install-tools/  libgcc_eh.a  plugin/
```

In the end the fix was:

```
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LIBRARY_PATH
```

