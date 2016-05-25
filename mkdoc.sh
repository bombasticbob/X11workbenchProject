#!/bin/sh

rm -rf ./doc
doxygen doxy.txt
tar -cz -f X11workbench-1.0.documentation.tgz doc


