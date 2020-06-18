This is a program that I wrote about 31 years ago and was my first "Shareware" type of program.

The really cool thing about this program is that this was the one I learned C with - and subsequently
learned every other language that I've learned by taking pieces of this one and rewriting it because
I knew it so well.

I hate the bracing style I used in this.   This is probably the 14th reformat overall, and was when I
was in my "Petzold, Programming Windows 3.0" stage.   The bracing style is awful.   I should have 
stuck with K&R style bracing, which is what I do now.

I wound up donating this to the [FreeDOS](http://www.freedos.org) project in July of 2001.   Unfortunately, the only 
record I can find of this now is this [spanish version of the FreeDOS log](http://www.fdos.info/es/Jul2001.htm).

Aside from the bracing style - I hope you enjoy looking at this.   I found it while rummaging through the house
over the last week and thought I would publish it on Github - not because its useful, but because it is historical.

The time this was written was a _great_ time, when someone like me could just start publishing software on bulletin boards.

Trust me, at the time it was cool.

See the [DOCS](https://github.com/rbieber/quickfind/tree/master/DOC) directory for some other cool historical documentation, including a software review.

Cool thing is, I made $10 off of all the hours that I put into this.   Yes, $10.

UPDATE
======
This now builds with the [OpenWatcom C Compiler](http://www.openwatcom.org/index.php/Downloads) for DOS.   The default
makefile (Makefile) will build the DOS executable.

Building this software also requires the [A86 assembler](http://www.eji.com/a86/) by Eric Isaacson.  Install the software 
in a directory that appears in your path.
 
After installing the above and the the Watcom C compiler setting up your environment, just type <CODE>wmake</CODE>





