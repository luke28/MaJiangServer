CC = arm-none-linux-gnueabi-gcc

OBJ = camera.o  encode_jpeg_zc301.o

test_lcd: 
	arm-none-linux-gnueabi-gcc  -O2   -c -o lcd.o lcd.c
	arm-none-linux-gnueabi-gcc     -c  camera.c    -o   camera.o    -I./ -L./
	arm-none-linux-gnueabi-gcc  -O2   -c -o test.o test.c
	arm-none-linux-gnueabi-gcc	lcd.o camera.o test.o  -o   test_lcd

%.o: %.c
	$(CROSS_COMPILE)$(CC)     -c  $<    -o   $@    -I./ -L./

clean:
	rm *.o test_lcd
