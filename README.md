MinionFirmware
==============

Arduino program for a plush minion, that will make sounds, when squezed and glow 
when shaken.
It uses the lilipad mp3 board for the sound and an FSR sensor to sense 
squeezing. A pololu MiniIMU 9DOF is used to determine if the plush is shaken, 
althought that is an overkill, but I had this sensor for a long time laying 
around without any use.
In the future it could be used to play sounds if minion is put in a certain 
position or if moved in a certain way.
Other electronic parts are small and thin 8 Ohm speaker and a 10kOhm resistor 
for the FSR as well as bright led.
I used the following tutorial for the plush:
http://youtu.be/WCWdZE0xVI4

Licensed under GPLv3


            ____________
           /            \
          /    _    _    \
    \|/  |----|_|--|_|----|
    | |  |                |
     \ \ |                |
      \ \|    \______/    |_
       \_|\              /| \
         | \____________/ |\ \
         |       _        | \ \
          \     |_|      /   | |
           \____________/    /|\
             |  |  |  |
             |  |  |  |
          ___|  |  |  |___
         /______|  |______\
           
