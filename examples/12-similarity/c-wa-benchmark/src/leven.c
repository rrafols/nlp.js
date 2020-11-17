/*
 * Copyright (c) AXA Group Operations Spain S.A.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include "utf8_decode.h"

#define TMP_SIZE 1000
#define TEST_STRINGS 18
#define MAX_STRING_SIZE 50

char array[TMP_SIZE];
char charCodeCache[TMP_SIZE];
int leftWide[TMP_SIZE];
int rightWide[TMP_SIZE];

int *leftW = leftWide;
int *rightW = rightWide;

int leven(char *left, char *right, int leftLength, int rightLength) {
  int *tmp;
  int swp, start, i, j, c;
  int bCharCode, result, temp, temp2;
  char done;

  i = 0;
  done = 0;
  utf8_decode_init(left, leftLength);
  while(!done) {
    c = utf8_decode_next();
    
    if (c == UTF8_END || c == UTF8_ERROR) {
      if (c == UTF8_ERROR) printf("utf8 error");
      done = 1;
    } else {
      printf("left %d %d\n", i, c);
      leftW[i++] = c;
    }
  }
  // printf("left: %d, %d\n", leftLength, i);
  leftLength = i;

  i = 0;
  done = 0;
  utf8_decode_init(right, rightLength);
  while(!done) {
    c = utf8_decode_next();
    
    if (c == UTF8_END || c == UTF8_ERROR) {
      if (c == UTF8_ERROR) printf("utf8 error");
      done = 1;
    } else {
      printf("right %d %d\n", i, c);
      rightW[i++] = c;
    }
  }
  
  // printf("right: %d, %d\n", rightLength, i);
  rightLength = i;

  if (leftLength > rightLength) {
    tmp = rightW;
    rightW = leftW;
    leftW = tmp;

    swp = rightLength;
    rightLength = leftLength;
    leftLength = swp;
  }
  
  leftLength--;
  rightLength--;
  
  while (leftLength > 0 && leftW[leftLength] == rightW[rightLength]) {
    leftLength--;
    rightLength--;
  }

  leftLength++;
  rightLength++;

  start = 0;
  while (start < leftLength && leftW[start] == rightW[start]) start++;

  leftLength -= start;
  rightLength -= start;

  if (!leftLength) return rightLength;

  for (i = 0; i < leftLength; i++) {
    charCodeCache[i] = leftW[start + i];
    array[i] = i + 1;
  }

  j = 0;
  while (j < rightLength) {
    bCharCode = rightW[start + j];
    temp = j;
    j += 1;
    result = j;
    for (i = 0; i < leftLength; i += 1) {

      temp2 = temp + (bCharCode != charCodeCache[i]);

      temp = array[i];
      if (temp > result) {
        array[i] = temp2 > result ? result + 1 : temp2;
      } else {
        array[i] = temp2 > temp ? temp + 1 : temp2;
      }
      result = array[i];
    }
  }
  return result;
}

int main() {

  char left[TEST_STRINGS][MAX_STRING_SIZE] = {
    "potatoe",
    "",
    "123",
    "a",
    "ab",
    "abc",
    "xabxcdxxefxgx",
    "xabxcdxxefxgx",
    "javawasneat",
    "example",
    "forward",
    "sturgeon",
    "levenshtein",
    "distance",
    "distance",
    "你好世界",
    "因為我是中國人所以我會說中文",
    "mikailovitch"
  };

  char right[TEST_STRINGS][MAX_STRING_SIZE] = {
    "potatoe",
    "123",
    "",
    "b",
    "ac",
    "axc",
    "1ab2cd34ef5g6",
    "abcdefg",
    "scalaisgreat",
    "samples",
    "drawrof",
    "urgently",
    "frankenstein",
    "difference",
    "eistancd",
    "你好",
    "因為我是英國人所以我會說英文",
    "Mikhaïlovitch"
  };

  int expected[TEST_STRINGS] = {0, 3, 3, 1, 1, 1, 6, 6, 7, 3, 6, 6, 6, 5, 2, 2, 2, 3};
  int i;

  for(i = 0; i < TEST_STRINGS; i++) {  
    printf("leven test %s vs %s result (should be %d): %d\n", left[i], right[i], expected[i], leven(left[i], right[i], strlen(left[i]), strlen(right[i])));
  }
}

