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

void extract_gz_into_buffer(const char *, const long unsigned int, char * const);
long unsigned int skip_lines(const char * buffer, const int);
void ezfio_extract_int     (char *, const long unsigned int, int *);
int fast_atoi (const char * str);

/***
 *     ___                   ./                                 
 *      |  ._ _  ._  |  _  ._ _   _  ._ _|_  _. _|_ o  _  ._  
 *     _|_ | | | |_) | (/_ | | | (/_ | | |_ (_|  |_ | (_) | | 
 *               |                                            
 */

/* The requirements are:
1. Input string contains only numeric characters, or is empty
2. Input string represents a number from 0 up to INT_MAX
*/

 int fast_atoi( const char * str )
{
    int val = 0;
    while( *str ) {
        val = val*10 + (*str++ - '0');
    }
    return val;
}

void extract_gz_into_buffer(const char * filename,
              const long unsigned int bytes_expected,
              char * const buffer) {

  /* Open gzFile */
  const gzFile file = gzopen(filename, "r");

  if (!file) {
    fprintf(stderr, "gzopen of '%s' failed: %s.\n",
            filename, strerror(errno));
    errno = -1;
    return;
  }

  /* Extract */
  const int bytes_uncompresed_read = gzread(file, buffer, bytes_expected-1);
  if (!gzeof(file)) {
    fprintf(stderr, "Error: Your buffer is to small for the uncompressed data. \n");
    errno = -1;
    return ;
  }

  buffer[bytes_uncompresed_read] = '\0';
}

long unsigned int skip_lines(const char * buffer, const int number_of_line){
  int ligne_read = 0;
  long unsigned int bytes_read = 0;

  while (buffer[bytes_read]) {
    if (buffer[bytes_read] == '\n') {
        
        if (ligne_read == number_of_line-1) break;
        ligne_read ++;
    }
    bytes_read ++;
    }

    return bytes_read + 1;
}

void ezfio_extract_int(char * const buffer, const long unsigned int scalars_supposed, int * const int_array){

  long unsigned int bytes_read = skip_lines(buffer, 2);


  long unsigned int int_possitions = bytes_read;

  long unsigned int scalars_read = 0;
  while (buffer[bytes_read]) {

    if (buffer[bytes_read] == '\n') {

      if (scalars_read > scalars_supposed) {
        fprintf(stderr, "Error: They are more value than (%lu) to read.\n",
                        scalars_read);
        errno = -1;
        return;
      }

        buffer[bytes_read] = '\0';
        int_array[scalars_read] = atoi( & buffer[int_possitions]);
        scalars_read++;

        int_possitions = bytes_read + 1;
    }
    bytes_read++;
  }

  if (scalars_read < scalars_supposed) {
    fprintf(stderr, "Error: I read less value (%lu) than asked (%lu).\n",
                    scalars_read, scalars_supposed);
    errno = -1;
  }

  return;
}

int main( int argc, const char* argv[] )
{
//  char *file =  "../input/bench.ezfio/bench/b1M.gz";
//  long unsigned int nb_of_int = 10000000;

  const long unsigned int header_max_size = 200;
  const long unsigned int nb_value_supposed = 10000000;
  const long unsigned int chars_per_value = 10;

  /* Number of bytes expected (with the \n) */
  const long unsigned int bytes_expected = (1 + chars_per_value) * nb_value_supposed + header_max_size;

  /* Allocate the buffer of char uncompresed*/
  char * buffer = (char * ) malloc(bytes_expected);
  if (buffer == NULL) {
    fprintf(stderr, "Allocate memory for uncompressed buffer failed: %s.\n",
                    strerror(errno));
  }

  char *file =  "../input/bench.ezfio/bench/b100M.gz";
  extract_gz_into_buffer(file,bytes_expected,buffer);
  

  if (errno != 0) {
    printf("%s\n", "failed");
  }
  else {
    printf("%s\n", "ok");
  }

  /* Create the buffer */
  int * int_array = (int * ) malloc(sizeof(int) * nb_value_supposed);
  if (int_array == NULL) {
    fprintf(stderr, "Allocate memory failed: %s.\n",
                      strerror(errno));
  }


   ezfio_extract_int(buffer,nb_value_supposed,int_array);

  if (errno != 0) {
    printf("%s\n", "failed");
  }
  else {
    printf("%s\n", "ok");
  }


  for(int i=0;i<10;++i) {
      printf("%d\n", int_array[i]);
  }
  
  free(int_array);
  free(buffer);

  return 0;
}