#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

int print_pointer(void* p)
{
  int chars_w = 2;
  fprintf(stdout, "0x");
  char aux_str[50];
  int i = 0;
  // Assume 64 bit pointer and if smaller there are no problems
  uint64_t p_64 = (uint64_t)p;
  while(p_64)
  {
    if(p_64 % 16 < 10)
    {
      aux_str[i++] = '0' + p_64 % 16;
    }else
    {
      aux_str[i++] = 'a' + p_64 % 16 - 10;
    }
    p_64 /= 16;
  }
  // Print pointer in rev order
  while(i)
  {
    putchar(aux_str[--i]);
    chars_w++;
  }
  return chars_w;
}

int print_int_in_base(int n, int is_unsigned, int base)
{
  int chars_w = 0;
  unsigned u_n = (unsigned)n;
  if(!is_unsigned && n < 0)
  {
    // Number is negative
    u_n = (unsigned)(-n);
    putchar('-');
    chars_w++;
  }
  if(u_n == 0)
  {
    putchar('0');
    chars_w++;
    return chars_w;
  }
  char aux_str[50];
  int i = 0;
  while(u_n)
  {
    if(u_n % base < 10)
    {
      aux_str[i++] = '0' + u_n % base;
    }else
    {
      aux_str[i++] = 'a' + u_n % base - 10;
    }
    u_n /= base;
  }
  while(i)
  {
    putchar(aux_str[--i]);
    chars_w++;
  }
  return chars_w;
}

int print_float(double x, int is_scientific)
{
  int chars_w = 0;
  if(x == 0.0)
  {
    fputs("0.000000", stdout);
    return 8;
  }
  if(is_scientific)
  {
    int e = 0;
    // Have f only be of a single digit before the decimal point
    while(x >= 10.0)
    {
      x = x / 10.0;
      e++;
    }
    while(x < 1.0)
    {
      x = x * 10.0;
      e--;
    }
    chars_w += print_float(x, 0);
    putchar('e');
    chars_w++;
    chars_w += print_int_in_base(e, 0, 10);
  }else
  {
    int whole = (int)x;
    chars_w += print_int_in_base(whole, 0, 10);
    // Make sure x is positive
    if(x < 0.0)
    {
      x = -x;
    }
    x = x - (float)((int)x);
    putchar('.');
    chars_w++;
    // Display the next 6 digits after the decimal place
    for(int i = 0; i < 6; i++)
    {
      x = x * 10.0;
      chars_w += putchar((int)x + '0');
      x = x - (int)x;
    }
  }
  return chars_w;
}

int custom_printf(const char* format, ...)
{
  int chars_w = 0;
  int i = 0;
  va_list valist;
  va_start(valist, format);
  while(format[i] != '\0')
  {
    if(format[i] == '%')
    {
      // We have a format specifier
      // %[d,i,u,f,e,,p,o,x,c,s,n,%]
      i++;
      switch(format[i])
      {
        case 'd':
          chars_w += print_int_in_base(va_arg(valist, int), 0, 10);
          break;
        case 'i':
          chars_w += print_int_in_base(va_arg(valist, int), 0, 10);
          break;
        case 'u':
          chars_w += print_int_in_base(va_arg(valist, unsigned), 1, 10);
          break;
        case 'f':
          chars_w += print_float(va_arg(valist, double), 0);
          break;
        case 'e':
          chars_w += print_float(va_arg(valist, double), 1);
          break;
        case 'p':
          chars_w += print_pointer(va_arg(valist, void*));
          break;
        case 'o':
          chars_w += print_int_in_base(va_arg(valist, unsigned), 1, 8);
          break;
        case 'x':
          chars_w += print_int_in_base(va_arg(valist, int), 1, 16);
          break;
        case 'c':
          putchar(va_arg(valist, int));
          chars_w++;
          break;
        case 's':
        {
          char* exterior_string = va_arg(valist, char*);
          fputs(exterior_string, stdout);
          chars_w += strlen(exterior_string);
          break;
        }
        case 'n':
        {
          int* exterior_intp = va_arg(valist, int*);
          *exterior_intp = chars_w;
          break;
        }
        case '%':
          putchar('%');
          chars_w++;
          break;
        default:
          fputs("Unknown modifier %", stdout);
          putchar(format[i]);
          exit(1);
      }
    }else
    {
      putchar(format[i]);
      chars_w++;
    }
    i++;
  }
  va_end(valist);
  return chars_w;
}

int main()
{
  int* rand_arr = malloc(100*sizeof(int));
  custom_printf("%p\n", rand_arr);
  free(rand_arr);
  char* s = "I am a string!";
  int written_c = 0;
  int d = -104;
  custom_printf("String print:%s\nWe also have a number written in 3 different bases:%d %o %x\n%n",
    s, d, d, d, &written_c);
  custom_printf("Percent sign:%%\nNumber of chars written previously:%d\n", written_c);
  float x = 756.23457f;
  custom_printf("Printing floats in 2 formats: %f %e\n", x, x);
  return 0;
}
