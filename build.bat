echo on
set TOOLS=tools
set OBJ=obj

if "%1"=="clean" (
	if exist %OBJ% rd /s/q %OBJ%
	if exist demo.gb del demo.gb
	goto end
)

if not exist %OBJ% mkdir %OBJ%

call %TOOLS%\gbdk-n-compile.bat res\donut_sprite.c -o %OBJ%\donut_sprite.rel
call %TOOLS%\gbdk-n-compile.bat res\james_sprite.c -o %OBJ%\james_sprite.rel
call %TOOLS%\gbdk-n-compile.bat res\robot_sprite.c -o %OBJ%\robot_sprite.rel
call %TOOLS%\gbdk-n-compile.bat res\numbers.c -o %OBJ%\numbers.rel
call %TOOLS%\gbdk-n-compile.bat Animation16x16.c -o %OBJ%\Animation16x16.rel
call %TOOLS%\gbdk-n-compile.bat Sprite16x16.c -o %OBJ%\Sprite16x16.rel
call %TOOLS%\gbdk-n-compile.bat GameScene.c -o %OBJ%\GameScene.rel
call %TOOLS%\gbdk-n-compile.bat Entity.c -o %OBJ%\Entity.rel
call %TOOLS%\gbdk-n-compile.bat Maths.c -o %OBJ%\Maths.rel
call %TOOLS%\gbdk-n-compile.bat main.c -o %OBJ%\main.rel

call %TOOLS%\gbdk-n-link.bat %OBJ%\donut_sprite.rel ^
                             %OBJ%\james_sprite.rel ^
                             %OBJ%\robot_sprite.rel ^
                             %OBJ%\numbers.rel ^
                             %OBJ%\Animation16x16.rel ^
                             %OBJ%\Sprite16x16.rel ^
                             %OBJ%\Entity.rel ^
                             %OBJ%\GameScene.rel ^
                             %OBJ%\Maths.rel ^
                             %OBJ%\main.rel ^
                              -o %OBJ%\demo.ihx

call %TOOLS%\gbdk-n-make-rom.bat %OBJ%\demo.ihx demo.gb

:end
