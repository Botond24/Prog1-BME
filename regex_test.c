#include <regex.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    regex_t regex;
    int ret;
    char buffer[100];

    if (argc < 3) {
        printf("Usage: %s <regex> <string>\n", argv[0]);
        return 1;
    }
    printf("Testing regex '%s' with string '%s'\n", argv[1], argv[2]);
 
    if (0 != (ret = regcomp(&regex, argv[1], REG_EXTENDED))) {
        regerror(ret, &regex, buffer, 100);
        printf("regcomp() failed with '%s'\n", buffer);
        return ret;
    }
    printf("OK regex\n");
    if (0 != (ret = regexec(&regex, argv[2], 0, NULL, 0))) {
      regerror(ret, &regex, buffer, 100);
      printf("regcomp() failed with '%s'\n", buffer);
      return ret;
   }
    return 0;
}