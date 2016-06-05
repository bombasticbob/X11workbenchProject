X11workbench - X11 programmer's 'work bench' application and toolkit
Copyright (c) 2010-2016 by Bob Frazier (aka 'Big Bad Bombastic Bob')
all rights reserved

DISCLAIMER:  The X11workbench application and toolkit software are supplied 'as-is', with no waranties, either implied or explicit.

There is no restriction as to what you can do with this software, so long as you include the above copyright notice and DISCLAIMER for any distributed work that is equal to or derived from this one, along with this paragraph that explains the terms of the license if the source is also being made available.  A "derived work" describes a work that uses a significant portion of the source files or algorithms that are included with this one.  Specifically excluded from this are files that were generated by the software, or anything that is included with the software that is part of another package (such as files that were created or added during the 'configure' process).  Specifically included is the use of part or all of any of the X11 workbench toolkit source or header files in your distributed application.  If you do not ship the source, the above copyright statement is still required to be placed in a reasonably prominent place, such as documentation, splash screens, and/or 'about the application' dialog boxes.

(for full license text, see COPYING)

This software has been tested on Linux and FreeBSD (version 6 and later)
If you have trouble building and/or running this software, and cannot
determine an appropriate workaround, please contact the author.


  sourceforge:  
    https://x11workbench.sourceforge.net/
    https://sourceforge.net/project/x11workbench
  HEAD:
    https://downloads.sourceforge.net/project/x11workbench/HEAD/X11workbench-1.0.tar.gz
  RELEASE:  TBD
    https://downloads.sourceforge.net/project/x11workbench/RELEASE/X11workbench-1.0.tar.gz
  ARCHIVE:  TBD
    https://downloads.sourceforge.net/project/x11workbench/ARCHIVE/X11workbench-1.0.tar.gz

  github:
    https://github.com/bombasticbob/X11workbenchProject

  documentation:
    https://bombasticbob.github.io/X11workbench

  website: 
    http://www.mrp3.com/

For Debian Linux and derivatives (like Ubuntu, Mint):
  Install a standard X11 distribution, along with the following packages
      build-essential
      xorg-dev
      autoconf
      automake
      autotools-dev

For FreeBSD (using the 'ports' system)
  run depends:   x11/libXpm and x11/xorg-clients
  build depends: devel/autotools  (meta-port)



BUILDING
--------

Prior to building, you may have to use the 'autotools' to create
or re-create some necessary files.  Specifically, you need the aclocal,
autoheader, autoconf, and automake programs.

To do so, enter the following commands:

  aclocal
  autoheader
  autoconf
  automake --add-missing --copy

(on some operating systems the program names may differ slightly, such
 as on FreeBSD, since the program names may reflect the version)

You should also run 'autoscan' at least once and do what it says.

Following this step, run

  ./configure

and then

  make


DEBUG VS RELEASE
----------------

The 'DEBUG' build is the default configuration when you use 'configure'
without any arguments.  To make a RELEASE build, do the following:

  ./configure --enable-release

  make clean
  make

You can then re-build the debug version using the following:

  ./configure

  make clean
  make


CONFIGURE OPTIONS
-----------------

Other than enabling a release build, there are additional 'configure' options
that you might consider using.

  --enable-release   enables a RELEASE build (i.e. no debug symbols)
  --enable-noopt     enables a 'no optimization' build; useful for debugging
  --enable-libXpm    uses libXpm rather than the internal equivalents.
                     NOTE:  libXpm is MUCH slower, observably so.  Many things
                            don't use libXpm any more because of this.

And also, the '--prefix' option, for installing everything in a place that
is NOT /usr/local (the default prefix):

  --prefix=pathname  sets the install prefix to 'pathname'

As an example, you might want to build and install a LOCAL copy for yourself
in '$HOME', or put it into a separate tree (particularly for a cross-compiled
version) such as '/usr/local/myversion' or '/usr/local/x86'

NOTE:  the install option will place a copy of the configuration output header
       'X11workbenchToolkit_config.h' in the appropriate 'include' directory.
       This will specify whatever options you built the toolkit with,
       excluding any debug or release options.



INSTALLATION
------------

See the 'INSTALL' file included with this distribution.

Once you have built a 'release' version, you can install it provided
that you have appropriate privileges on your operating system.

To install the various files 'as-is' (after being built):

  make install



RE-DISTRIBUTION
---------------

Re-distribution of this application is permitted according to the
license requirements specified in COPYING.

Also, please include the entire contents of THIS file with any
re-distribution of the X11workbench application and/or toolkit.



CODE SUBMISSIONS
----------------

Basic style guide for code submissions are as follows:

a) BSD style (aka Allman style) indentation.  Other styles are simply
   irritating and/or are more difficult to read.
   (see http://en.wikipedia.org/wiki/Indent_style)

b) 2 spaces per indent, no hard tabs (this eliminates the possibility
   that 'other editors' display the code differently).  The exception
   to the 'hard tab' rule is (of course) any 'make' file or autotool
   files that generate 'make' files.  These files may require hard tabs
   in the first column.

c) 'hungarian' notation when possible.  This may be a little nauseating
   to some anti-microsoft purists, but it helps make the code more
   readable (and maintainable by others, particularly me).

d) global variables should ONLY be used when ABSOLUTELY NECESSARY
   (the code is SUPPOSED to be somewhat object-oriented, ok?).  If you
   must use a global variable, give it a name that's self-descriptive.

e) ANSI C and POSIX compatibility whenever possible.

f) maximum cross-platform compatibility (use autotool features to make
   sure this happens).  The current target platforms are:

      Linux (all flavors)
      FreeBSD (6-STABLE and later)
      Interix (Microsoft's Services for Unix, aka 'SFU')
      Cygwin
      Apple (X11 compatibility)

   X server interoperatibility should be as wide-ranged as possible.
   The current targeted X servers and/or client libraries are:

      Xorg
      Xfree86
      XMing (on Windows)

   The code should also work with just about any window manager.  The
   following window managers (at a minumum) must be supported:

      gnome 2.x and 3.x
      Mate
      KDE
      lxde
      fluxbox (and other 'blackbox' derived)
      twm (and clones)

g) minimal 'run dependencies'.  If you MUST add a dependency, make sure it
   compiles on every target platform AND provide some kind of alternate
   code [via autotools] for when the library/package isn't installed or
   available.  A good example of this is 'libXpm' which is NOT present in
   Interix, and there is specific code in place to deal with this.


Author testing has been done on the following platforms:
  FreeBSD 7.x, 8.x, 9.x (gnome 2, fluxbox)
  Debian (squeeze,wheezy) linux with 2.6 kernel (mate,gnome,KDE)
  Interix (on Windows XP) with XMing
  Cygwin (on Windows XP) with XMing


