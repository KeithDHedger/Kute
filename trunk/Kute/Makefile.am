
SUBDIRS = Kute/app Kute/debug#***MMM***

scriptfilesdir = $(pkgdatadir)/scripts
scriptfiles_SCRIPTS = Kute/resources/scripts/*

man1_MANS = Kute/resources/man/kute.1

#$(shell mkdir -p ~/.gnome2/nautilus-scripts/Kute)
#$(shell cp -rp $(pkgdatadir)/scripts/ConvertALLtoMP3 ~/.gnome2/nautilus-scripts/Kute/ConvertALLtoMP3)
#$(shell /usr/bin/install -dt Kute/resources/scripts/* ~/.gnome2/nautilus-scripts/Kute/)
#$(shell ln -s $(pkgdatadir)/scripts/* ~/.gnome2/nautilus-scripts/Kute/)

$(shell /usr/bin/install -cpd ~/.gnome2/nautilus-scripts/Kute)
$(shell /usr/bin/install -cp Kute/resources/scripts/ConvertALLtoMP3  ~/.gnome2/nautilus-scripts/Kute/ConvertALLtoMP3)
$(shell /usr/bin/install -cp Kute/resources/scripts/ConvertFLACtoAAC  ~/.gnome2/nautilus-scripts/Kute/ConvertFLACtoAAC)


#$(shell ../resources/myscript "@TESTVAR@")
