all: ft_printf

test:
	./test.sh

printf: ft_printf.c
	gcc -Wno-format ft_printf.c -D TEST -o printf

ft_printf: ft_printf.c
	gcc ft_printf.c -D TEST -D FT_PRINTF -o ft_printf

clean:
	rm printf ft_printf
