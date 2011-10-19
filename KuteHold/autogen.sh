#!/bin/bash

./remakesourcelist
aclocal
autoheader
touch NEWS AUTHORS ChangeLog README
automake --add-missing --copy
autoconf
./configure "$1"

