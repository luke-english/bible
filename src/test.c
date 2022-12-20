#include <unistr.h>
#include <uniwidth.h>
#include <uniwbrk.h>
#include <uninorm.h>
#include <stdio.h>

void ok() {
  printf("OK\n");
}

#define TEST(x, e) if (x) { printf("OK\n"); } else { perror(e); exit(1); } 

int main(int argc, char** argv) {

  TEST(4 == u8_width("hello", 4, "UTF-8"),
    "u8_width first 4 bytes of hello");

  int i = u8_width("ačiū", sizeof("ačiū"), "utf-8");
  printf("u8_width of 'ačiū' = %d\n", i);
  TEST(4 == u8_width("ačiū", sizeof("ačiū"), "utf-8"),
    "full length of ačiū width must be 4" );
  
  printf("u8_strlen(ačiū) = %ld\n", u8_strlen("ačiū"));
  TEST(6 == u8_strlen("ačiū"),
    "ačiū length must be 6" );
  

  printf("u8_strnlen(ačiū jums labai, 4) = %ld\n",
    u8_strnlen("ačiū jums labai", 4));
  // TEST(6 == u8_strmblen("ačiū jums labai", 4),
  //   "ačiū jums labai first 4 characters length must be 6" );
  



  // 4 == u8_width("ačiū", 5, "")
  //   elseways("v8_width failed with ačiū");

  // 33 == u8_width("ačiū", 4, "")
  //   elseways("v8_width failed with ačiū");
}