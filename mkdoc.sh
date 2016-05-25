#!/bin/sh

rm -rf ./doc
doxygen doxy.txt
tar -cz -f SourceForge/DOC/X11workbench-1.0.documentation.tgz doc


