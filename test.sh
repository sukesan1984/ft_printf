make ft_printf
make printf
./ft_printf > ft_printf.txt
./printf > printf.txt
diff -a --suppress-common-lines ft_printf.txt printf.txt
make clean
#rm ft_printf.txt printf.txt
