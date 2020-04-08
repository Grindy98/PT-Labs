#include "mystring.h"
#include <stdio.h>

int main()
{
    {
        printf("\nCheck strlen functions:\n");
        printf("%ld\n", my_strlen("cadcahkasdvkhvkdfsads"));
    }
    {
        printf("\nCheck stpcpy functions:\n");
        char a1[1000]="sadfas";
        char* p = my_stpncpy(a1, "bool", 5  );
        for(int i = 0; i < 5; i++)
        {
            putchar(a1[i]);
        }
        printf("\n%ld\n", p - a1);
    }
    {
        printf("\nCheck strcat functions:\n");
        char a1[1000] = "fjsadbjlfdsa";
        my_strncat(a1, "jasfnna", 3);
        puts(a1);
    }
    {
        printf("\nCheck strchr functions:\n");
        char a1[1000] = "saddafsacaxasd";
        puts(my_strchr(a1, 'x' ));
        puts(my_strrchr(a1, 'd'));
    }
    {
        printf("\nCheck strcmp functions:\n");
        printf("%d %d %d\n", my_strcmp("","das"), 
        my_strncmp("qazax", "qaz", 3), my_strcmp("aaaaas", "aaaaa"));
    }
    {
        printf("\nCheck strspn functions:\n");
        printf("%ld\n", my_strcspn("fdascfafwe", "w"));
        printf("%ld\n", my_strspn("fdascfafwe", "fdasc"));
    }
    {
        printf("\nCheck strpbrk function:\n");
        printf("%s\n", my_strpbrk("fdascfafwe", "cw"));
    }
    {
        printf("\nCheck strstr function:\n");
        puts(my_strstr("rrralabralaporrrtocarlaaaaaalaaa", "rt"));
    }
    {
        printf("\nCheck strtoupr/lwr functions:\n");
        char a1[1000] = "";
        puts(my_strtoupr(a1, "lalaLALAdsavac12313"));
        puts(my_strtolwr(a1, "lalaLALAdsavac12313"));
    }
    {
        printf("\nCheck strtok functions:\n");
        char s[1000] = "rrralabralaporrrtocarlaaaaaalaaazz";
        printf("1: %s\n", my_strtok(s, "r"));
        char* p;
        int i = 2;
        while((p = my_strtok(NULL, "ra")) != NULL)
        {
            printf("%d: %s\n", i++, p);
        }

        my_strcpy(s, "jjjaajvabjobaojcdwjmxqwjonbof");
        printf("1: %s\n", my_strtok(s, "ja"));
        i = 2;
        while((p = my_strtok(NULL, "ao")) != NULL)
        {
            printf("%d: %s\n", i++, p);
        }

        my_strcpy(s, "");
        printf("1: %s\n", my_strtok(s, "ja"));
        i = 2;
        while((p = my_strtok(NULL, "ao")) != NULL)
        {
            printf("%d: %s\n", i++, p);
        }
    }
    {
        char str[]="m.y,o.w.n,,,s.t.r.t.o.k...";
        const char *p=",",*q=".";
        char *a,*b,*c,*d;
        putchar('\n');
        for( a=strtok_r(str,p,&c) ; a!=NULL ; a=strtok_r(NULL,p,&c) ) {
                printf("%s\n",a);

                for( b=strtok_r(a,q,&d) ; b!=NULL ; b=strtok_r(NULL,q,&d) )
                        printf("%s\n",b);
        }

    }
    return 0;
}