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
* [ATmega32U4   (TWI) - soon to come]

Prepared already for:

* ATmega8      (TWI)
* ATmega48     (TWI)
* ATmega88     (TWI)
* ATmega328    (TWI)
* ATtiny26     (USI)
* ATmega169    (USI)

In theory those should immediately work, but this is still not
verified. Micros not listed just lack their appropriate 
definitions and can get added.

----------------
Status 3.5.2020:
*  `single-master-demo/` - in Arbeit, SM sieht gut aus, SL fehlt
*  `multi-master-demo/` - benötigt debugging und Verifikation
*  `multi-master-slave-demo/` - könnte auf Anhieb laufen
*  `example-usage-demo` - leer

//i2c_hw.h (multimaster+slave)
//               [ ] R0 = first draft
//               [ ] R1 = able to compile and link
//               [ ] R2 = running stable
//               [x] R3 = working as intended
//               [ ] R4 = documentation complete
//               [ ] R5 = final release

----------------

The Library itself is contained below the path `lib-i2c/`.
You find there its `includes` and `sources` in case you want to
take a look under the hood. Normally you do not need to bother
with its details.
The only file of interest is `lib-i2c/includes/i2c_hw.h` giving
all the API interface details. A full fledged
[Doxygen]()
documentation is available and found in the `doxygen` folder.

Demo applications are prepared for the variety of operation modes
the I²C is capable of. There are quite a couple and each is put
into a dedicated folder:

*  `single-master-demo/` - one single master accesses two slaves
*  `multi-master-demo/` - two master devices access one common
   slave
*  `multi-master-slave-demo/` - two master devices access one
   common slave and also each other as slave
*  `example-usage-demo` - how to setup a typical project for just
   one micro

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
setting using one micro. This is way less complicated when
considering the `example-usage demo`. The `Makefile` given there
you could use as foundation for your own purposes.

It is recommended to use the folder structure as it is given here.
It clearly distinguishes between lib parts, application and the
top level used for some management and documentation purposes.

----------------

Anwendung:
Man bindet die Dateien aus dem lib-Verzeichnis ein, lässt das Make
laufen und hat was man für seinen Anwendungsfall benötigt.

In jeder .c-Datei die richtige Auswahl an bedingter Compilierung
setzen, dann werden unnötige Dateien (denen Definitionen im Header
fehlen) nicht gebaut. Falls etwas nicht stimmt, meckert der Linker.

Und genau so ist es aktuell für die "twi0_..." Sektion gelöst.
Jede Datei hat ifdefs und wird nur compiliert wenn die Defs passend
stehen. Sonst macht der Compiler nix (aber das Make macht
Vorarbeiten).

Testlauf mit dem alten MMS-Testcase: Es wird gebaut wenn die Lib-
Einstellungen passen, es verweigert wenn die Einstellungen der Lib
falsch sind.

Wir hätten dann noch drei Modi ...

Je Modus ein Verzeichnis mit Demo. Die Lib liegt "nebenan".

...
+- Projekt-root
   +- lib-i2c
   |  +- include
   |  +- source
   |
   +- multi-master-slave-demo (jeder µC 8 MHz interner OSC)
   |
   +- multi-master-demo
   |
   +- single-master-demo
   |
   +- slave-demo


Für eine Lib zu einer Application ist ein Makefile ausreichend.
In dem Makefile werden immer *alle* Lib-Routinen für die gewünschte
Einstellung angegeben.
Baut man allerdings zwei unterschiedliche Applikationen, ist für jede
Applikation ein eigenes Makefile mit nur dem jeweiligen Teil für
twi oder usi sinnvoller. Dann lassen sich beide Programme mit einem
Master-Makefile bauen. Alternativ werden die demos noch weiter
aufgeteilt. Der Aufwand ist aber nur für die demos mit mehreren µCs
erforderlich. Für den üblichen Fall (ein µC-Projekt) geht es einfacher.

Wenn das alles soweit fertig ist, muss zu jedem der 4 Anwendungsfälle
eine demo bereitgestellt werden. Dazu ist die komplette Dokumentation
zu pflegen und als Doxygen-Dateisatz mit abzuliefern. Es sollen keine
Fragen bleiben. Ausser vielleicht die Frage nach: "Warum gibt es die
Lib erst jetzt im Netz?"

Ein schneller Steifzug eben brachte sowohl reine SW-Libs als auch
einige TWI-Libs. Alle nur als Master brauchbar. USI ist gar nicht
zu finden. Hm, richtig beworben und platziert, könnte das ein Hammer
werden!

Ein Sparkfun ProMicro als USB-Userport der den PC als I²C-Slave
nutzbar macht ...
