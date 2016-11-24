CALL C:/gbdk/bin/lcc -c -o james_sprite.o res/james_sprite.c
CALL C:/gbdk/bin/lcc -c -o donut_sprite.o res/donut_sprite.c
CALL C:/gbdk/bin/lcc -c -o robot_sprite.o res/robot_sprite.c
CALL C:/gbdk/bin/lcc -c -o Animation16x16.o Animation16x16.c
CALL C:/gbdk/bin/lcc -c -o Sprite16x16.o Sprite16x16.c
CALL C:/gbdk/bin/lcc -o demo.gb Sprite16x16.o Animation16x16.o robot_sprite.o donut_sprite.o james_sprite.o main.c

