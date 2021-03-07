# I²C-Lib

Make 8-Bit AVR devices more versatile. Build your own special
sensor or actuation controller, connect multiple master devices
to common I/O or even communicate by I²C instead of UART. Check
out the I²C operation modes supported:

* Slave
* Single Master
* Multiple-Master
* Multiple-Master-plus-Slave

The Multiple-Master modes all feature bus arbitration in case of
simultaneous accesses.

Use hardware modules on the micro, dedicated to I²C support:

* USI
* TWI

Make hardware support transparent to the application. One
application could compile for different micros without changes to
the source code - at least for the I²C parts!
For example a simple master application currently compiled for an
ATtiny2313 can be compiled for an ATmega48 by just changing some
Makefile settings. Of course, environment changes (e. g. LED
or pushbutton connections) need to be reflected, too.

Currently verified with:

* ATtiny2313   (USI)
* ATmega168    (TWI)

Prepared already for:

* ATmega8      (TWI)
* ATmega48     (TWI)
* ATmega88     (TWI)
* ATmega328    (TWI)
* ATtiny26     (USI)
* ATmega169    (USI)
* ATmeag16U4   (TWI)
* ATmega32U4   (TWI)

In theory those should immediately work, but this is still not
verified. Micros not listed just lack their appropriate 
definitions and can get added in `lib-i2c/includes/i2c_hw.h`.

_The ATmega8 is somewhat special since it lacks a TWIMASK register.
Due to this it just cannot be made to respond to an entire address
range._


## What To Find Where

The Library itself is contained below the path `lib-i2c/`.
You find there its `includes` and `sources` in case you want to
take a look under the hood. Normally you do not need to bother
with its details.
The only file of interest is `lib-i2c/includes/i2c_hw.h` giving
all the API interface details.

Demo applications are prepared for the variety of operation modes
the I²C is capable of. There are quite a couple and each is put
into a dedicated folder:

*  `single-master-demo/` - one single master accesses two slaves
*  `multi-master-demo/` - two master devices access one common
   slave
*  `multi-master-slave-demo/` - two master devices access one
   common slave and also each other as slave

Each demo contains documentation on the circuitry intended, the
application source code and the makefiles used to build this
particular demo. Be aware, the demos are made to compile once for
different settings on different micros. For this reason most demos
are way more complicated to build than your later application code.
But no fear, the complicated building is done by make. You simply
call `make` on the command line to rebuild the examples. Ready to
flash `.hex`-files are pre-made for you. Just flash them to the
micros and hook the hardware up according to the schematic to try
it out.

In most cases your project will compile just for one certain
setting using one micro. For this you just need one makefile and 
one main.c file.

It is recommended to use the folder structure as it is given here.
It clearly distinguishes between lib parts, application and the
top level used for some management and documentation purposes.


## How To Use

Well, this is quite simple. Take all files found in `/lib-i2c/`, add
them to your project and compile them together with all the other
things. Of course, your makefile needs to get all files referenced for
compilation.

See the demos for some examples. Be aware, that most demos provide code
for _two different_ AVRs. Because if this you will find one `...usi...`
and one `...twi...` set of files that need to get flashed into the right
controller.