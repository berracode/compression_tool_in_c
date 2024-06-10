@echo off
setlocal enabledelayedexpansion

REM compiler name
set CC=gcc

REM build options for development
set CFLAGS_DEV=-Wall -Wextra -g

REM build options for prod
set CFLAGS_PROD=-O3 -flto -DNDEBUG -march=native

REM headers
set HEADERS=ktmem.h heap_priority_queue.h kthash_table.h compress.h simple_hash_table.h binary_utils.h decompress.h

REM source
set SRCS=ktmem.c heap_priority_queue.c kthash_table.c compress.c simple_hash_table.c binary_utils.c decompress.c main.c

REM objects
set OBJS=ktmem.o heap_priority_queue.o kthash_table.o compress.o simple_hash_table.o binary_utils.o decompress.o main.o

REM executable name
set EXEC=cpress.exe

REM main
if "%1"=="dev" goto dev
if "%1"=="prod" goto prod
if "%1"=="clean" goto clean
if "%1"=="rebuild" goto rebuild

REM default build
call :prod

REM compile sources
:compile_objs
echo building...
for %%f in (%SRCS%) do (
    %CC% %CFLAGS% -c %%f -o %%~nf.o
)
exit /b 0

REM clean all generated files
:clean_objs
echo cleaning objs...
for %%f in (%OBJS%) do (
    if exist %%f del %%f
)
exit /b 0

REM clean all generated files include .exe
:clean
echo cleaning all...
call :clean_objs
if exist %EXEC% (
    echo deleting .exe %EXEC%...
    del %EXEC%
)
exit /b 0


REM clean all generated files except .exe
:clean_not_exe
echo cleaning all objects only...
call :clean_objs
exit /b 0

REM rebuild
:rebuild
echo rebuild...
call :clean
call :compile_objs
echo building executable...
%CC% %CFLAGS% -o %EXEC% %OBJS%
call :clean_not_exe
exit /b 0

REM build in dev 
:dev
echo building in dev...
set CFLAGS=%CFLAGS_DEV%
call :rebuild
call :clean_not_exe
exit /b 0

REM building in prod
:prod
echo building in prod...
set CFLAGS=%CFLAGS_PROD%
call :rebuild
exit /b 0

