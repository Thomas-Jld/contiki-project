#git clone https://github.com/contiki-os/contiki.git
unzip contiki
patch -t -p0 -i patch.diff
cp -r src/examples/messenger contiki_save/examples/
cp -r src/apps/messenger-app contiki_save/apps
cd contiki_save/examples/messenger/ 
make all
#make all TARGET=avr-atmega128rfa1

