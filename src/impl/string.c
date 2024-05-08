#include "../include/string.h"
#include "../include/kheap.h"

size_t strlen (char *str) {
    //return (*str) ? strlen(++str) + 1 : 0;
    for (size_t len = 0; ; ++str, ++len) { if (!*str) return len; }
}

char* strcat(char* destination, const char* source) {
    char* ptr = destination + strlen(destination);
 
    while (*source != '\0') {
        *ptr++ = *source++;
    }

    *ptr = '\0';

    return destination;
}

char* strcpy(char* destination, const char* source) {
    if (destination == NULL) {
        return NULL;
    }
 
    char *ptr = destination;
 
    while (*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }
 
    *destination = '\0';
 
    return ptr;
}

int strcmp(const char *X, const char *Y) {
    while (*X)
    {
        if (*X != *Y) {
            break;
        }
 
        X++;
        Y++;
    }
 
    return *(const unsigned char*)X - *(const unsigned char*)Y;
}

unsigned int digit_count(int num){
  unsigned int count = 0;
  if(num == 0)
    return 1;
  while(num > 0){
    count++;
    num = num/10;
  }
  return count;
}

char *hextostr(uint32_t num)
{
  unsigned char *s = "0000\0";

  s[0] = (num >> 24) & 0xff;
  s[1] = (num >> 16) & 0xff;
  s[2] = (num >>  8) & 0xff;
  s[3] = num         & 0xff;

  return s;
}

void itoa(int num, char *number) {
  int dgcount = digit_count(num);
  int index = dgcount - 1;
  char x;
  if(num == 0 && dgcount == 1){
    number[0] = '0';
    number[1] = '\0';
  }else{
    while(num != 0){
      x = num % 10;
      number[index] = x + '0';
      index--;
      num = num / 10;
    }
    number[dgcount] = '\0';
  }
}

bool is_delim(char c, char *delim) {
  while(*delim != '\0')
  {
    if(c == *delim)
      return true;
    delim++;
  }
  return false;
}

char *strtok(char *s, char *delim) {
  static char *p;
  if(!s)
  {
    s = p;
  }
  if(!s)
  {
    return NULL;
  }

  while(1)
  {
    if(is_delim(*s, delim))
    {
      s++;
      continue;
    }
    if(*s == '\0')
    {
      return NULL;
    }
    break;
  }

  char *ret = s;
  while(1)
  {
    if(*s == '\0')
    {
      p = s;
      return ret;
    }
    if(is_delim(*s, delim))
    {
      *s = '\0';
      p = s + 1;
      return ret;
    }
    s++;
  }
}

char *strchr(char *s, char c) {
   while(*s != c && *s != '\0') {
      s++;
   }
   if(*s == c) {
      return s;
   }else {
      return NULL;
   }
}

long atoi(const char* S) {
    long num = 0;
 
    int i = 0;

    while (S[i] && (S[i] >= '0' && S[i] <= '9'))
    {
        num = num * 10 + (S[i] - '0');
        i++;
    }
 
    return num;
}