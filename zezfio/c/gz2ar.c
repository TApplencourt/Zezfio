#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <errno.h>
#include <string.h>

/***
 *      _                                   
 *     | \  _   _ |  _. ._ _. _|_ o  _  ._  
 *     |_/ (/_ (_ | (_| | (_|  |_ | (_) | | 
 *                                          
 */

char   * extract_gz_into_buffer(const char *, const long unsigned int, const long unsigned int, int *);
int    * ezfio_extract_int     (const char *, const long unsigned int, int *);


/***
 *     ___                   ./                                 
 *      |  ._ _  ._  |  _  ._ _   _  ._ _|_  _. _|_ o  _  ._  
 *     _|_ | | | |_) | (/_ | | | (/_ | | |_ (_|  |_ | (_) | | 
 *               |                                            
 */
char * extract_gz_into_buffer(const char * filename,
                              const long unsigned int nb_int_supposed,
                              const long unsigned int chars_per_value,
                              int *error_code) {

  *error_code = 0;
  /* And the header of ezfio is max of 100 character
     10 Millions  collumn with one element in each*/
  const long unsigned int header_max_size = 200;

  /* Number of bytes expected (with the \n) */
  const long unsigned int bytes_expected = (1 + chars_per_value) * nb_int_supposed + header_max_size;

  /* Allocate the buffer of char uncompresed*/
  char * buffer = (char * ) malloc(bytes_expected);
  if (buffer == NULL) {
    fprintf(stderr, "Allocate memory for uncompressed buffer failed: %s.\n",
                    strerror(errno));
    *error_code = 1;
    return buffer;
  }


  /* Open gzFile */
  const gzFile file = gzopen(filename, "r");

  if (!file) {
    fprintf(stderr,
      "gzopen of '%s' failed: %s.\n",
      filename, strerror(errno));
    *error_code = 1;
    return buffer;
  }

  /* Extract */
  const int bytes_uncompresed_read = gzread(file, buffer, bytes_expected-1);
  if (!gzeof(file)) {
    fprintf(stderr, "Error: gzread have not reach the end of file. Maybe ask for more integer (>%lu).\n",
      nb_int_supposed);
    *error_code = 1;
    return buffer;
  }

  buffer[bytes_uncompresed_read] = '\0';
  return buffer;
}

int * ezfio_extract_int(const char * filename, const long unsigned int nb_int_supposed, int *error_code) {

  *error_code = 0;

  /* Create the buffer */
  int * int_array = (int * ) malloc(sizeof(int) * nb_int_supposed);
  if (int_array == NULL) {
    fprintf(stderr, "Allocate memory failed: %s.\n",
                      strerror(errno));
     *error_code = 1;
     return int_array;
  }

  /* Integer Case    
     Let say we need 10 chars for each integer
     unsigned long  4 bytes 0 to 4,294,967,295 */
  char *buffer = extract_gz_into_buffer(filename, nb_int_supposed, 10, error_code);
  if (*error_code)  {
    return int_array;}

  long unsigned int int_possitions = 0;
  long unsigned int nb_int_read = 0;
  long unsigned int bytes_read = 0;

  int skip_header = 0;

  while (buffer[bytes_read]) {

    if (buffer[bytes_read] == '\n') {

      if (nb_int_read > nb_int_supposed) {
        fprintf(stderr, "Error: nb_int_read (%lu) > nb_int_supposed (%lu).\n",
                        nb_int_read, nb_int_supposed);
         *error_code = 1;
          break;
      }

        buffer[bytes_read] = '\0';

        if (skip_header < 2) {
          skip_header++;
        } else {
          int_array[nb_int_read] = atoi( & buffer[int_possitions]);
          nb_int_read++;
        }
        
        int_possitions = bytes_read + 1;
    }
    bytes_read++;
  }

  if (nb_int_read < nb_int_supposed) {
    fprintf(stderr, "Error: nb_int_read (%lu) < nb_int_supposed (%lu). You overshoot the number of integer.\n",
                    nb_int_read, nb_int_supposed);
    *error_code = 1;
    return int_array;
  }

  free(buffer);
  return int_array;
}

int main( int argc, const char* argv[] )
{
//  char *file =  "../input/bench.ezfio/bench/b1M.gz";
//  long unsigned int nb_of_int = 10000000;

  char *file =  "../input/bench.ezfio/bench/b100.gz";
  long unsigned int nb_of_int = 100;
  int error_code;

  int *data = ezfio_extract_int(file, nb_of_int, &error_code);

  printf("Error code: %d\n", error_code );

//  for(int i=0;i<nb_of_int;++i) {
//    printf("%d\n", data[i]);
//  }
  
  free(data);
  return 0;
}