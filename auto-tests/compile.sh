#!/bin/bash
# 
#  ./pwn $file_pwn
#  ld -m elf_i386 -o $file_exec $file_o -lrts
for file_pwn in *.pwn
do


file_exec=${file_pwn//.pwn/}
file_out=${file_pwn//.pwn/.outout}
file_exp=${file_pwn//.pwn/.out}
file_asm=${file_pwn//.pwn/.asm}
file_o=${file_pwn//.pwn/.o}

echo $file_exec
echo $file_o
echo $file_out
echo $file_asm
echo $file_exp

./../pwn $file_pwn
yasm -felf32 $file_asm
ld -m elf_i386 -o $file_exec $file_o -lrts
./$file_exec > $file_out 
diff --ignore-all-space -u -r $file_out expected/$file_exp

done
  

