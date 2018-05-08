#!/bin/sh

sed -i '/#include\s*<SDL2\/SDL.h>/s/SDL2\///' *.c *.h
