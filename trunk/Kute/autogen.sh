#!/bin/bash

./remakesourcelist
aclocal
autoheader
touch NEWS AUTHORS ChangeLog
automake --add-missing --copy
autoconf
./configure "$1"

