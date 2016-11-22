CALL C:/gbdk/bin/lcc -c -o james_sprite.o res/james_sprite.c
CALL C:/gbdk/bin/lcc -c -o donut_sprite.o res/donut_sprite.c
CALL C:/gbdk/bin/lcc -o demo.gb donut_sprite.o james_sprite.o main.c

