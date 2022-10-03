NAME=ambilib~
CSYM=ambilib~

current: pd_darwin

# ----------------------- NT -----------------------

pd_nt: ambilib~.dll

.SUFFIXES: .dll

PDNTCFLAGS = /W3 /WX /DNT /DPD /nologo
VC="C:\Program Files\Microsoft Visual Studio\VC98"

PDNTINCLUDE = /I. /I\tcl\include /I\pd\src /I$(VC)\include

PDNTLDIR = $(VC)\lib
PDNTLIB = $(PDNTLDIR)\libc.lib \
	$(PDNTLDIR)\oldnames.lib \
	$(PDNTLDIR)\kernel32.lib \
	C:\pd\pd_37\bin\pd.lib 

.c.dll:
	cl $(PDNTCFLAGS) $(PDNTINCLUDE) /c $*.c
	link /dll /export:ambilib_tilde_setup $*.obj $(PDNTLIB)

# ----------------------- IRIX 5.x -----------------------

pd_irix5: $(NAME).pd_irix5

.SUFFIXES: .pd_irix5

SGICFLAGS5 = -o32 -DPD -DUNIX -DIRIX -O2

SGIINCLUDE =  -I../../src

.c.pd_irix5:
	$(CC) $(SGICFLAGS5) $(SGIINCLUDE) -o $*.o -c $*.c
	ld -elf -shared -rdata_shared -o $*.pd_irix5 $*.o
	rm $*.o

# ----------------------- IRIX 6.x -----------------------

pd_irix6: $(NAME).pd_irix6

.SUFFIXES: .pd_irix6

SGICFLAGS6 = -n32 -DPD -DUNIX -DIRIX -DN32 -woff 1080,1064,1185 \
	-OPT:roundoff=3 -OPT:IEEE_arithmetic=3 -OPT:cray_ivdep=true \
	-Ofast=ip32

.c.pd_irix6:
	$(CC) $(SGICFLAGS6) $(SGIINCLUDE) -o $*.o -c $*.c
	ld -n32 -IPA -shared -rdata_shared -o $*.pd_irix6 $*.o
	rm $*.o

# ----------------------- LINUX i386 -----------------------

pd_linux: $(NAME).pd_linux

.SUFFIXES: .pd_linux

LINUXCFLAGS = -DPD -O2 -funroll-loops -fomit-frame-pointer -fPIC \
    -Wall -W -Wshadow -Wstrict-prototypes \
    -Wno-unused -Wno-parentheses -Wno-switch $(CFLAGS)

LINUXINCLUDE =  -I../../src

.c.pd_linux:
	$(CC) $(LINUXCFLAGS) $(LINUXINCLUDE) -o $*.o -c $*.c
	ld -export_dynamic  -shared -o $*.pd_linux $*.o -lc -lm
	strip --strip-unneeded $*.pd_linux
	rm -f $*.o

# ----------------------- Mac OSX -----------------------

pd_darwin: $(NAME).pd_darwin

.SUFFIXES: .pd_darwin


DARWINCFLAGS = -DPD -O2  -Wall -W -Wshadow -Wstrict-prototypes \
    -Wno-unused -Wno-parentheses -Wno-switch -isysroot /Developer/SDKs/MacOSX10.4u.sdk

.c.pd_darwin:
	#./tk2c.bash < $*.tk >$*.tk2c
	cc $(DARWINCFLAGS) $(LINUXINCLUDE) -arch i686 -o $*386.o -c $*.c
	cc -bundle -arch i686 -undefined suppress  -isysroot /Developer/SDKs/MacOSX10.4u.sdk -flat_namespace -o $*386.pd_darwin $*386.o 
	rm -f ../$*386.pd_darwin
#	ln -s  $*/$*386.pd_darwin ..

#.c.pd_darwin:
	#./tk2c.bash < $*.tk >$*.tk2c
	cc $(DARWINCFLAGS) $(LINUXINCLUDE) -arch ppc -o $*ppc.o -c $*.c
	cc -bundle -arch ppc -undefined suppress  -isysroot /Developer/SDKs/MacOSX10.4u.sdk -flat_namespace -o $*ppc.pd_darwin $*ppc.o 
	rm -f ../$*ppc.pd_darwin
#	ln -s  $*/$*ppc.pd_darwin ..
	lipo -create ambilib~ppc.pd_darwin ambilib~386.pd_darwin -output bin/ambilib~.pd_darwin
	rm -f ../$*.pd_darwin
	rm -f *386.pd_darwin
	rm -f *ppc.pd_darwin
	
#	ln -s  $*/$*.pd_darwin ..

# ----------------------------------------------------------

clean:
	rm -f *.o *.pd_* so_locations
