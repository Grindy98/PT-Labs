#include "mystring.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>

void *my_memccpy(void *dest, const void *src, int c, size_t n)
{
    char* d = dest;
    const char* s = src;
    for(int i = 0; i < n; i++)
    {
        if((d[i] = s[i]) == c)
        {
            return (void*)(d + i + 1);
        }
    }
    return NULL;
}

void *my_memchr(const void *s, int c, size_t n)
{
    const char* a = s;
    for(int i = 0; i < n; i++)
    {
        if(a[i] == c)
        {
            return (void*)(a + i);
        }
    }
    return NULL;
}

int my_memcmp(const void *s1, const void *s2, size_t n)
{
    const char* a1 = s1;
    const char* a2 = s2;
    for(int i = 0; i < n; i++)
    {
        if(a1[i] != a2[i])
        {
            return (int)(a1[i]) - (int)(a2[i]);
        }
    }
    return 0;
}

void *my_memcpy(void *dest, const void *src, size_t n)
{
    char* d = dest;
    const char* s = src;
    for(int i = 0; i < n; i++)
    {
        d[i] = s[i];
    }
    return dest;
}

void *my_memmove(void *dest, const void *src, size_t n)
{
    char* d = dest;
    const char* s = src;
    char temp[n];
    int i;
    for(i = 0; i < n; i++)
    {
        temp[i] = s[i];
    }
    for(i = 0; i < n; i++)
    {
        d[i] = temp[i];
    }
    return dest;
}

void *my_memset(void *s, int c, size_t n)
{
    char* a = s;
    char byte = (char)c;
    for(int i = 0; i < n; i++)
    {
        a[i] = byte;
    }
    return s;
}

char *my_stpcpy(char *dest, const char *src)
{
    int i = 0;
    while(1)
    {
        dest[i] = src[i];
        if(src[i] == '\0')
        {
            return dest + i;
        }
        i++;
    }
}

char *my_stpncpy(char *dest, const char *src, size_t n)
{
    size_t n_min = my_strlen(src) + 1;
    if(n_min > n)
    {
        n_min = n;
    }
    for(int i = 0; i < n_min; i++)
    {
        dest[i] = src[i];
    }
    if(n < my_strlen(src) + 1)
    {
        // Case where length of src is greater or equal with n
        return dest + n;
    }
    for(int i = n_min; i < n; i++)
    {
        dest[i] = '\0';
    }
    return dest + n_min - 1;
}

char *my_strcat(char *dest, const char *src)
{
    size_t n_src = my_strlen(src) + 1;
    char* append = dest + my_strlen(dest);
    for(int i = 0; i < n_src; i++)
    {
        append[i] = src[i];
    }
    return dest;
}

char *my_strchr(const char *s, int c)
{
    int i = 0;
    while(s[i] != '\0')
    {
        if(s[i] == c)
        {
            return (char*)s + i;
        }
        i++;
    }
    return NULL;
}

int my_strcmp(const char *s1, const char *s2)
{
    int i = 0;
    while((s1[i] != '\0') && (s2[i] != '\0'))
    {
        if(s1[i] != s2[i])
        {
            break;
        }
        i++;
    }
    return (int)(s1[i]) - (int)(s2[i]);
}

char *my_strcpy(char *dest, const char *src)
{
    int i = 0;
    while(1)
    {
        if((dest[i] = src[i]) == '\0')
        {
            return dest;
        }
        i++;
    }
}

size_t my_strcspn(const char *s, const char *reject)
{
    size_t n = 0;
    while(s[n] != '\0')
    {
        if(my_strchr(reject, s[n]) != NULL)
        {
            // Found a char from reject
            return n;
        }
        n++;
    }
    return n;
}

char *my_strdup(const char *s)
{
    int n = my_strlen(s) + 1;
    char* dest = malloc(n);
    my_strcpy(dest, s);
    return dest;
}

size_t my_strlen(const char *s)
{
    size_t n = 0;
    // Check if little or big endian
    unsigned int endian = 1;
    if(*((char*)&endian) == 0)
    {
        // Big endian, return the length inefficiently
        while(s[n++] != '\0');
        return n - 1;
    }
    uint64_t byte, *casted_s;
    casted_s = (uint64_t*)s;
    while(1)
    {
        byte = casted_s[n];
        if((byte &       0x00000000000000ff) == 0)
        {
            return n * 8;
        }else if((byte & 0x000000000000ff00) == 0)
        {
            return n * 8 + 1;
        }else if((byte & 0x0000000000ff0000) == 0)
        {
            return n * 8 + 2;
        }else if((byte & 0x00000000ff000000) == 0)
        {
            return n * 8 + 3;
        }else if((byte & 0x000000ff00000000) == 0)
        {
            return n * 8 + 4;
        }else if((byte & 0x0000ff0000000000) == 0)
        {
            return n * 8 + 5;
        }else if((byte & 0x00ff000000000000) == 0)
        {
            return n * 8 + 6;
        }else if((byte & 0xff00000000000000) == 0)
        {
            return n * 8 + 7;
        }
        n++;
    }
}

char *my_strncat(char *dest, const char *src, size_t n)
{
    size_t n_src = my_strlen(src);
    if(n < n_src)
    {
        n_src = n;
    }
    char* append = dest + my_strlen(dest);
    for(int i = 0; i < n_src; i++)
    {
        append[i] = src[i];
    }
    append[n_src] = '\0';
    return dest;
}

int my_strncmp(const char *s1, const char *s2, size_t n)
{
    int i = 0;
    while((s1[i] != '\0') && (s2[i] != '\0') && (i < n - 1))
    {
        if(s1[i] != s2[i])
        {
            break;
        }
        i++;
    }
    return (int)(s1[i]) - (int)(s2[i]);
}

char *my_strncpy(char *dest, const char *src, size_t n)
{
    int i = 0;
    while(i < n)
    {
        if((dest[i] = src[i]) == '\0')
        {
            break;
        }
        i++;
    }
    return dest;
}

char *my_strndup(const char *s, size_t n)
{
    size_t len = my_strnlen(s, n);
    char* dest = malloc(len + 1);
    my_strncpy(dest, s, len);
    dest[len] = '\n';
    return dest;
}

size_t my_strnlen(const char *s, size_t maxlen)
{
    size_t n = 0;
    for(; n < maxlen; n++)
    {
        if(s[n] == '\0')
        {
            break;
        }
    }
    return n;
}

char *my_strpbrk(const char *s, const char *accept)
{
    int i = 0;
    while(s[i] != '\0')
    {
        if(my_strchr(accept, s[i]) != NULL)
        {
            return (char*)s + i;
        }
        i++;
    }
    return NULL;
}

char *my_strrchr(const char *s, int c)
{
    // Iterating the loop only once for efficiency
    char* last_c_ptr = NULL;
    int i = 0;
    while(s[i] != '\0')
    {
        if(s[i] == c)
        {
            last_c_ptr = (char*)s + i;
        }
        i++;
    }
    return last_c_ptr;
}

size_t my_strspn(const char *s, const char *accept)
{
    size_t n = 0;
    while(s[n] != '\0')
    {
        if(my_strchr(accept, s[n]) == NULL)
        {
            // Found a char not included in accept
            return n;
        }
        n++;
    }
    return n;
}

char *my_strstr(const char *haystack, const char *needle)
{
    size_t len_n = my_strlen(needle);
    if(len_n == 0)
    {
        // If needle has no chars
        return (char*)haystack;
    }
    int i = 0;
    while(haystack[i] != '\0')
    {
        if(haystack[i] == needle[0])
        {
            int j = 1;
            while(1)
            {
                if(haystack[i+j] != needle[j])
                {
                    break;
                }
                if(j + 1 == len_n)
                {
                    return (char*)haystack + i;
                }
                j++;
            }
        }
        i++;
    }
    return NULL;    
}

char *my_strtok(char *str, const char *delim)
{
    static char* prev_str = NULL;
    // When this is null, the function is not initialized
    if(prev_str == NULL && str == NULL)
    {
        // Function uninitialized and no new string given
        return NULL;
    } else if(str != NULL)
    {
        // Initialization step
        prev_str = str;
    }
    
    // Jump over all initial delims
    while(my_strchr(delim, *prev_str) != NULL)
    {
        prev_str++;
    }
    if(*prev_str == '\0')
    {
        // Reached the end of the string
        prev_str = NULL;
        return NULL;
    }

    char* start_str = prev_str;
    while(*prev_str != '\0')
    {
        if(my_strchr(delim, *prev_str) != NULL)
        {
            // Reached the next token
            *prev_str = '\0';
            prev_str++;
            return start_str;
        }
        prev_str++;
    }
    // If we reach this point, prev_str points to the end of the string =>
    // Reinitialize the function and return the last token
    prev_str = NULL;
    return start_str;
}

char *my_strtok_r(char *str, const char *delim, char **saveptr)
{
    if(str != NULL)
    {
        // Initialization step
        *saveptr = str;
    }   
    // Jump over all initial delims
    while(my_strchr(delim, **saveptr) != NULL)
    {
        *saveptr += 1;
    }
    if(**saveptr == '\0')
    {
        // Reached the end of the string
        *saveptr = NULL;
        return NULL;
    }

    char* start_str = *saveptr;
    while(**saveptr != '\0')
    {
        if(my_strchr(delim, **saveptr) != NULL)
        {
            // Reached the next token
            **saveptr = '\0';
            *saveptr += 1;
            return start_str;
        }
        *saveptr += 1;
    }
    // If we reach this point, saveptr points to the end of the string =>
    // Reinitialize the function and return the last token
    return start_str;
}

char *my_strtoupr(char *dest, const char *src)
{
    int i = 0;
    while(1)
    {
        if(src[i] >= 'a' && src[i] <= 'z')
        {
            dest[i] = src[i] - 'a' + 'A';
        }
        else
        {
            dest[i] = src[i];
        }  
        if(src[i] == '\0')
        {
            return dest;
        }
        i++;
    }
}

char *my_strtolwr(char *dest, const char *src)
{
    int i = 0;
    while(1)
    {
        if(src[i] >= 'A' && src[i] <= 'Z')
        {
            dest[i] = src[i] - 'A' + 'a';
        }
        else
        {
            dest[i] = src[i];
        }      
        if(src[i] == '\0')
        {
            return dest;
        }
        i++;
    }
}