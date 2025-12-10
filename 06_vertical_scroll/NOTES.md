# Vertical Scrolling

*THE* page to learn about scrolling is [Player Missile](https://playermissile.com/scrolling_tutorial/index.html#topic-1-vertical-coarse-scrolling)


Vertical scrolling works out pretty easily because the way A8s lay out video
memory. Each line in a playfield to to bottom is just a straight run of bytes,
one line following the other. 

Since Display Lists are provided with a start-address (LMS), we can just change 
the adress following the LMS to move up and down the field.



