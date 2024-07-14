/*
 **********************************************************************
 ** md5driver.c -- sample routines to test                           **
 ** RSA Data Security, Inc. MD5 message digest algorithm.            **
 ** Created: 2/16/90 RLR                                             **
 ** Updated: 1/91 SRD                                                **
 **********************************************************************
 */

/*
 **********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved. **
 **                                                                  **
 ** RSA Data Security, Inc. makes no representations concerning      **
 ** either the merchantability of this software or the suitability   **
 ** of this software for any particular purpose.  It is provided "as **
 ** is" without express or implied warranty of any kind.             **
 **                                                                  **
 ** These notices must be retained in any copies of any part of this **
 ** documentation and/or software.                                   **
 **********************************************************************
 */

#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <mpi.h>
#include <string.h>
/* -- include the following file if the file md5.h is separate -- */
#include "md5.h" 

/* Prints message digest buffer in mdContext as 32 hexadecimal digits.
   Order is from low-order byte to high-order byte of digest.
   Each byte is printed with high-order hexadecimal digit first.
 */
static void MDPrint (mdContext)
MD5_CTX *mdContext;
{
  int i;

  for (i = 0; i < 16; i++)
    printf ("%02x", mdContext->digest[i]);
}

/* size of test block */
#define TEST_BLOCK_SIZE 1000

/* number of blocks to process */
#define TEST_BLOCKS 10000

/* number of test bytes = TEST_BLOCK_SIZE * TEST_BLOCKS */
static long TEST_BYTES = (long)TEST_BLOCK_SIZE * (long)TEST_BLOCKS;

/* A time trial routine, to measure the speed of MD5.
   Measures wall time required to digest TEST_BLOCKS * TEST_BLOCK_SIZE
   characters.
 */
static void MDTimeTrial ()
{
  MD5_CTX mdContext;
  time_t endTime, startTime;
  unsigned char data[TEST_BLOCK_SIZE];
  unsigned int i;

  /* initialize test data */
  for (i = 0; i < TEST_BLOCK_SIZE; i++)
    data[i] = (unsigned char)(i & 0xFF);

  /* start timer */
  printf ("MD5 time trial. Processing %ld characters...\n", TEST_BYTES);
  time (&startTime);

  /* digest data in TEST_BLOCK_SIZE byte blocks */
  MD5Init (&mdContext);
  for (i = TEST_BLOCKS; i > 0; i--)
    MD5Update (&mdContext, data, TEST_BLOCK_SIZE);
  MD5Final (&mdContext);

  /* stop timer, get time difference */
  time (&endTime);
  MDPrint (&mdContext);
  printf (" is digest of test input.\n");
  printf
    ("Seconds to process test input: %ld\n", (long)(endTime-startTime));
  printf
    ("Characters processed per second: %ld\n",
     TEST_BYTES/(endTime-startTime));
}

/* Computes the message digest for string inString.
   Prints out message digest, a space, the string (in quotes) and a
   carriage return.
 */

 // 静态缓冲区，足够存储结果字符串（32 字符 + 1 个终止符 '\0'）

static char* MDPrint_1(MD5_CTX* mdContext) {
  int i;
    static char result[33];
  for (i = 0; i < 16; i++) {
    sprintf(result + (i * 2), "%02x", mdContext->digest[i]);
  }

  return result;
}
static char* MDString (inString)
char *inString;
{
  MD5_CTX mdContext;
  unsigned int len = strlen (inString);
  MD5Init (&mdContext);
  MD5Update (&mdContext, inString, len);
  MD5Final (&mdContext);
  char* result = MDPrint_1 (&mdContext);

  return result;
}


int main(int argc, char* argv[]) {
    char* output;
    char input[50];
    char letter[50];
    char text[450];
    int numP, myId;
    int foundMatch = 0;  // 终止标志

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);
    MPI_Comm_size(MPI_COMM_WORLD, &numP);

    if (myId == 0) {
        scanf("%s", input);
    }

    MPI_Bcast(&input, 50, MPI_CHAR, 0, MPI_COMM_WORLD);

    for (int i = 0; i < 26; i++) {
        letter[i] = 'a' + i;
    }

    for (int i = 0; i < 10; i++) {
        letter[i + 26] = '0' + i;
    }

    letter[36] = '\0';
    int size = 36 / numP;
    int start = size * myId;
    int end = (myId == numP - 1) ? 36 : (myId + 1) * size;  

    for (int i = start; i < end; i++) {
        if (foundMatch) {  // 检查终止标志
            break;
        }

        text[0] = letter[i];

        for (int j = 0; j < 36; j++) {
            text[1] = letter[j];

            for (int k = 0; k < 36; k++) {
                text[2] = letter[k];

                for (int t = 0; t < 36; t++) {
                    text[3] = letter[t];
                    output = MDString(text);

                    if (strcmp(output, input) == 0) {
                        foundMatch = 1;  // 设置终止标志
                        printf("%s\n",text);
                        break;
                    }
                }

                if (foundMatch) {  // 检查终止标志
                    break;
                }
            }

            if (foundMatch) {  // 检查终止标志
                break;
            }
        }
    }

    // 广播终止标志
    MPI_Bcast(&foundMatch, 1, MPI_INT, 0, MPI_COMM_WORLD);


    MPI_Finalize();
}


