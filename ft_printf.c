/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktakami <ktakami@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 16:45:15 by ktakami           #+#    #+#             */
/*   Updated: 2020/11/21 16:45:15 by ktakami          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdarg.h>
#define CONV "dxs"

char *ft_strchr(char *str, char c)
{
	if (!str)
		return (NULL);
	while (*str)
	{
		if (*str == c)
			return (str);
		str++;
	}
	return (NULL);
}

int	ft_putchar(char c)
{
	return write(1, &c, 1);
}

int	ft_putstr(char *str)
{
	int	res;
	if (!str)
		return (0);
	res = 0;
	while (*str)
	{
		res += ft_putchar(*str);
		str++;
	}
	return (res);
}

int	ft_putstrl(char *str, int len)
{
	int res;
	int	i;

	if (!str)
		return (0);
	res = 0;
	i = 0;
	while (str[i] && i < len)
	{
		res += ft_putchar(str[i]);
		i++;
	}
	return (res);
}

typedef struct	s_args
{
	int	c;
	int width;
	int has_width;
	int precision;
	int has_precision;
}				t_args;

int		ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int		ft_isspace(char c)
{
	return (c >= 9 && c <= 13) || (c == 32);
}

int		ft_atoi(char *str)
{
	long long res;
	long long max;
	int			sign;

	max = 0x7fffffffffffffff;
	while (ft_isspace(*str))
		str++;
	sign = 1;
	if (*str == '-' || * str == '+')
	{
		sign = (*str == '-') ? -1 : 1;
		str++;
	}
	res = 0;
	while (ft_isdigit(*str))
	{
		if (res < max - (*str - '0') / 10)
			res = 10 * res + (*str - '0');
		else
			res = (sign == -1) ? (max + 1) : max;
		str++;
	}
	return (res);
}

int		ft_get_digits(int d)
{
	int digits;
	if (d == -2147483648)
		return (11);

	digits = 0;
	if (d < 0)
	{
		digits++;
		d *= -1;
	}
	while (d / 10)
	{
		digits++;
		d /= 10;
	}
	digits++;
	return (digits);
}

int		ft_get_digits_x(unsigned int ud)
{
	int digits;

	digits = 0;

	while (ud / 16)
	{
		digits++;
		ud /= 16;
	}
	digits++;
	return (digits);
}

void	initialize_args(t_args *args)
{
	args->c = 0;
	args->has_width = 0;
	args->has_precision = 0;
	args->width = 0;
	args->precision = 0;
}

char	*read_args(t_args *args, char *itr)
{
	if (!itr || *itr != '%')
		return (itr);

	itr++;
	while (*itr)
	{
		initialize_args(args);
		// width
		if (ft_isdigit(*itr))
		{
			args->has_width = 1;
			args->width = ft_atoi(itr);
			itr += ft_get_digits(args->width);
		}
		// .
		if (*itr == '.')
		{
			args->has_precision = 1;
			itr++;
		}
		// precision
		if (ft_isdigit(*itr))
		{
			args->has_precision = 1;
			args->precision = ft_atoi(itr);
			itr += ft_get_digits(args->precision);
		}
		//dxs
		if (ft_strchr(CONV, *itr))
		{
			args->c = *itr;
			itr++;
			return (itr);
		}
		itr++;
	}
	return (itr);
}

int	ft_strlen(char *s)
{
	int	len;

	if (!s)
		return (0);
	len = 0;
	while (*s++)
		len++;
	return (len);
}

int	ft_put_s(t_args *args, va_list ap)
{
	int		res;
	int		width;
	int		precision;
	int		len;
	int		putlen;
	char	*s;

	width = args->has_width ? args->width : 0;
	precision = args->has_precision ? args->precision : 0;
	s = va_arg(ap, char *);
	if (!s)
		s = "(null)";
	len = ft_strlen(s);
	if (args->has_precision)
		putlen = (len > precision) ? precision : len;
	else
		putlen = len;
	res = 0;
	while ((width - putlen) > 0)
	{
		res += ft_putchar(' ');
		width--;
	}
	return (res + ft_putstrl(s, putlen));
}

int	ft_puti(int d, int padding)
{
	int	res;

	res = 0;
	if (d == -2147483648)
	{
		res += ft_putchar('-');
		while (padding-- > 0)
			res += ft_putchar('0');
		res += ft_putstr("2147483648");
		return (res);
	}
	if (d < 0)
	{
		res += ft_putchar('-');
		d *= -1;
	}
	while (padding-- > 0)
		res += ft_putchar('0');
	if (d / 10)
		res += ft_puti(d / 10, padding);
	res += ft_putchar((d % 10) + '0');
	return (res);
}

int	ft_putx(unsigned int ud, int padding)
{
	int	res;

	res = 0;
	while (padding-- > 0)
		res += ft_putchar('0');
	if (ud / 16)
		res += ft_putx(ud / 16, padding);
	if ((ud % 16) >= 10)
		res += ft_putchar((ud % 16) - 10 + 'a');
	else
		res += ft_putchar((ud % 16) + '0');
	return (res);
}

int	ft_put_d(t_args *args, va_list ap)
{
	int	width;
	int	precision;
	int	d;
	int	len;
	int	putlen;
	int	padding;
	int	res;

	width = args->has_width ? args->width : 0;
	precision = args->has_precision ? args->precision : 0;
	d = va_arg(ap, int);
	len = ft_get_digits(d);
	if (args->has_precision && args->precision == 0 && d == 0)
		len = 0;
	if (d < 0)
		padding = ((len - 1) < precision) ? precision - (len - 1) : 0;
	else
		padding = (len < precision) ? precision - len : 0;
	putlen = len + padding;
	res = 0;
	while ((width - putlen) > 0)
	{
		res += ft_putchar(' ');
		width--;
	}
	if (args->has_precision && args->precision == 0 && d == 0)
		return (res);
	return (res + ft_puti(d, padding));
}

int ft_put_x(t_args *args, va_list ap)
{
	int	width;
	int	precision;
	int	d;
	int	len;
	int	putlen;
	int	padding;
	int	res;

	width = args->has_width ? args->width : 0;
	precision = args->has_precision ? args->precision : 0;
	d = va_arg(ap, int);
	len = ft_get_digits_x(d);
	if (args->has_precision && args->precision == 0 && d == 0)
		len = 0;
	padding = (len < precision) ? precision - len : 0;
	putlen = len + padding;
	res = 0;
	while ((width - putlen) > 0)
	{
		res += ft_putchar(' ');
		width--;
	}
	if (args->has_precision && args->precision == 0 && d == 0)
		return (res);
	return (res + ft_putx(d, padding));
}

int	ft_put_conv(t_args *args, va_list ap)
{
	if (args->c == 's')
		return ft_put_s(args, ap);
	else if (args->c == 'd')
		return ft_put_d(args, ap);
	else if (args->c == 'x')
		return ft_put_x(args, ap);
	return (0);
}

int	ft_printf(const char * restrict format, ...)
{
	char	*itr;
	int		res;
	t_args	args;
	va_list	ap;

	itr = (char *)format;
	if (!itr)
		return (0);
	res = 0;
	va_start(ap, format);
	while (*itr)
	{
		if (*itr == '%')
		{
			itr = read_args(&args, itr);
			res += ft_put_conv(&args, ap);
			continue;
		}
		res += ft_putchar(*itr);
		itr++;
	}
	va_end(ap);
	return (res);
}

#ifdef TEST

#include <stdio.h>

#ifdef FT_PRINTF
#define F(...) \
res = ft_printf(__VA_ARGS__); \
printf("%d\n", res);
#else
#define F(...) \
res = printf(__VA_ARGS__); \
printf("%d\n", res);
#endif

int main()
{
	int	res;
	setvbuf(stdout, 0, _IONBF, 0);
	F("hoge\n");
	F("[%s]\n", "hoge");
	F("[%10.5s]\n", "hoge");
	F("[%10.2s]\n", "hoge");
	F("[%3.2s]\n", "hoge");
	F("[%3.10s]\n", "hoge");
	F("[%10.0s]\n", "hoge");
	F("[%s]\n", NULL);
	F("[%10.5s]\n", NULL);
	F("[%10.2s]\n", NULL);
	F("[%3.2s]\n", NULL);
	F("[%3.10s]\n", NULL);
	F("[%10.0s]\n", NULL);
	F("[%d]\n", 123);
	F("[%d]\n", -123);
	F("[%d]\n", -2147483648);
	F("[%d]\n", 2147483647);
	F("[%.0d]\n", -2147483648);
	F("[%.5d]\n", -2147483648);
	F("[%.10d]\n", -2147483648);
	F("[%.20d]\n", -2147483648);
	F("[%.d]\n", -2147483648);
	F("[%10.d]\n", -2147483648);
	F("[%20.d]\n", -2147483648);
	F("[%0.0d]\n", -2147483648);
	F("[%5.0d]\n", -2147483648);
	F("[%10.0d]\n", -2147483648);
	F("[%20.0d]\n", -2147483648);
	F("[%0.0d]\n", -2147483648);
	F("[%0.5d]\n", -2147483648);
	F("[%0.10d]\n", -2147483648);
	F("[%0.20d]\n", -2147483648);
	F("[%5.0d]\n", -2147483648);
	F("[%5.5d]\n", -2147483648);
	F("[%5.10d]\n", -2147483648);
	F("[%5.20d]\n", -2147483648);
	F("[%10.0d]\n", -2147483648);
	F("[%10.5d]\n", -2147483648);
	F("[%10.10d]\n", -2147483648);
	F("[%10.20d]\n", -2147483648);
	F("[%20.0d]\n", -2147483648);
	F("[%20.5d]\n", -2147483648);
	F("[%20.10d]\n", -2147483648);
	F("[%20.20d]\n", -2147483648);
	F("[%10d]\n", 0);
	F("[%10.d]\n", 0);
	F("[%10.0d]\n", 0);
	F("[%.0d]\n", 0);
	F("[%10.0d]\n", 10);
	F("[%.0d]\n", -120);
	F("[%x]\n", 123);
	F("[%x]\n", -123);
	F("[%x]\n", -2147483648);
	F("[%x]\n", 2147483647);
	F("[%.0x]\n", -2147483648);
	F("[%.5x]\n", -2147483648);
	F("[%.10x]\n", -2147483648);
	F("[%.20x]\n", -2147483648);
	F("[%.x]\n", -2147483648);
	F("[%10.x]\n", -2147483648);
	F("[%20.x]\n", -2147483648);
	F("[%0.0x]\n", -2147483648);
	F("[%5.0x]\n", -2147483648);
	F("[%10.0x]\n", -2147483648);
	F("[%20.0x]\n", -2147483648);
	F("[%0.0x]\n", -2147483648);
	F("[%0.5x]\n", -2147483648);
	F("[%0.10x]\n", -2147483648);
	F("[%0.20x]\n", -2147483648);
	F("[%5.0x]\n", -2147483648);
	F("[%5.5x]\n", -2147483648);
	F("[%5.10x]\n", -2147483648);
	F("[%5.20x]\n", -2147483648);
	F("[%10.0x]\n", -2147483648);
	F("[%10.5x]\n", -2147483648);
	F("[%10.10x]\n", -2147483648);
	F("[%10.20x]\n", -2147483648);
	F("[%20.0x]\n", -2147483648);
	F("[%20.5x]\n", -2147483648);
	F("[%20.10x]\n", -2147483648);
	F("[%20.20x]\n", -2147483648);
	F("[%.0x]\n", -1234);
	F("[%.5x]\n", -1234);
	F("[%.10x]\n", -1234);
	F("[%.20x]\n", -1234);
	F("[%.x]\n", -1234);
	F("[%10.x]\n", -1234);
	F("[%20.x]\n", -1234);
	F("[%0.0x]\n", -1234);
	F("[%5.0x]\n", -1234);
	F("[%10.0x]\n", -1234);
	F("[%20.0x]\n", -1234);
	F("[%0.0x]\n", -1234);
	F("[%0.5x]\n", -1234);
	F("[%0.10x]\n", -1234);
	F("[%0.20x]\n", -1234);
	F("[%5.0x]\n", -1234);
	F("[%5.5x]\n", -1234);
	F("[%5.10x]\n", -1234);
	F("[%5.20x]\n", -1234);
	F("[%10.0x]\n", -1234);
	F("[%10.5x]\n", -1234);
	F("[%10.10x]\n", -1234);
	F("[%10.20x]\n", -1234);
	F("[%20.0x]\n", -1234);
	F("[%20.5x]\n", -1234);
	F("[%20.10x]\n", -1234);
	F("[%20.20x]\n", -1234);
	F("[%10x]\n", 0);
	F("[%10.x]\n", 0);
	F("[%10.0x]\n", 0);
	F("[%.0x]\n", 0);
	F("[%10.0x]\n", 10);
	F("[%.0x]\n", -120);
	F("%s, %10.s, %d, %x\n", "hoge", "fuga", 10, -123);
	return (0);
}
#endif
