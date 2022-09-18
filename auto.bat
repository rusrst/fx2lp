cd C:\SDCC\bin  
sdcc -mmcs51 --model-small -c C:\fx2lp\fx2lp\main.c -o C:\fx2lp\fx2lp\out\
sdcc -mmcs51 --model-small -c C:\fx2lp\fx2lp\interrupts.c -o C:\fx2lp\fx2lp\out\ 
sdcc -mmcs51 --model-small -c C:\fx2lp\fx2lp\table.c -o C:\fx2lp\fx2lp\out\ 
sdcc -mmcs51 --model-small  -Wl -bUSB_VECTORS=0x1000 C:\fx2lp\fx2lp\out\main.rel C:\fx2lp\fx2lp\out\interrupts.rel C:\fx2lp\fx2lp\out\table.rel -o C:\fx2lp\fx2lp\out\first_step.hex
cd C:\fx2lp\fx2lp\console\fx2lp_fix_interrupt
main -in C:\fx2lp\fx2lp\out\first_step.hex -data 0x021000 -adr 0x43
pause