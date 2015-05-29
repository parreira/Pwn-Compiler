#!/bin/bash
file="test.pwn"

file_asm=${file//pwn/asm}
file_o=${file//pwn/o}
file_exec=${file//.pwn/}

rm -f $file_asm
#make clean -s
make

cat $file
echo " "
echo " "
./pwn -g $file

yasm -felf32 $file_asm

ld -m elf_i386 -o $file_exec $file_o -lrts
echo " "
echo " " 

sed '/\t/d' $file_asm
echo " "
echo " "
./$file_exec
rm -f $file_o $file_exec
