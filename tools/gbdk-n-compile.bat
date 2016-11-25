IF "%GBDK_DIR%" == "" GOTO NOGBDK

sdcc -mgbz80 --no-std-crt0 -I "%GBDK_DIR%\include" -I "%GBDK_DIR%\include\asm" -c %*
GOTO END

:NOGBDK
@ECHO GBDK_DIR is not set

:END