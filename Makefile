pbl3: pbl3.c pbl3-dev.c
gcc -o pbl3 pbl3.c pbl3-dev.c -lwiringPi -lwiringPiDev -lpthread -lmosquitto
clean:
rm pbl3