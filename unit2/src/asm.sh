# intel 指令格式和 att 指令格式都是同一个指令集，只是不同的书写风格、语法风格
gcc function.c -S -o function.s -masm=intel
gcc main.c -S -o main.s