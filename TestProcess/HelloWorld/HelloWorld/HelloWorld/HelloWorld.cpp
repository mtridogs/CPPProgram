#include <stdio.h>

int main()
{
printf("Hello World\n");
#if DEBUG
getchar();
#endif
return 0;
}