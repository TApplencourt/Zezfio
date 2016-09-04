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

void gzip2buffer            (const char *, const long unsigned int, char * const);
long unsigned int skip_lines(const char * buffer, const int);
void buffer2int_impur       (char * const, const long unsigned int, int    * const);
void buffer2long_impur      (char * const, const long unsigned int, long   * const);
void buffer2double_impur    (char * const, const long unsigned int, double * const);
void buffer2float_impur     (char * const, const long unsigned int, float  * const);

/***
 *     ___                   ./                                 
 *      |  ._ _  ._  |  _  ._ _   _  ._ _|_  _. _|_ o  _  ._  
 *     _|_ | | | |_) | (/_ | | | (/_ | | |_ (_|  |_ | (_) | | 
 *               |                                            
 */

void gzip2buffer(const char * filename,
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

//Start of copy pasta for int/long/double/float
void buffer2int_impur(char * const buffer, const long unsigned int scalars_supposed, int * const scalar_array){

  long unsigned int bytes_read = skip_lines(buffer, 2);

  long unsigned int scalar_possitions = bytes_read;
  long unsigned int scalar_read = 0;

  while (buffer[bytes_read]) {

    if (buffer[bytes_read] == '\n') {

      if (scalar_read > scalars_supposed) {
        fprintf(stderr, "Error: They are more value than (%lu) to read.\n",
                        scalar_read);
        errno = -1;
        return;
      }

        buffer[bytes_read] = '\0';
        scalar_array[scalar_read] = atoi( & buffer[scalar_possitions]);
        scalar_read++;

        scalar_possitions = bytes_read + 1;
    }
    bytes_read++;
  }

  if (scalar_read < scalars_supposed) {
    fprintf(stderr, "Error: I read less value (%lu) than asked (%lu).\n",
                    scalar_read, scalars_supposed);
    errno = -1;
  }

  return;
}

void buffer2long_impur(char * const buffer, const long unsigned int scalars_supposed, long * const scalar_array){

  long unsigned int bytes_read = skip_lines(buffer, 2);

  long unsigned int scalar_possitions = bytes_read;
  long unsigned int scalar_read = 0;

  while (buffer[bytes_read]) {

    if (buffer[bytes_read] == '\n') {

      if (scalar_read > scalars_supposed) {
        fprintf(stderr, "Error: They are more value than (%lu) to read.\n",
                        scalar_read);
        errno = -1;
        return;
      }

        buffer[bytes_read] = '\0';
        scalar_array[scalar_read] = atol( & buffer[scalar_possitions]);
        scalar_read++;

        scalar_possitions = bytes_read + 1;
    }
    bytes_read++;
  }

  if (scalar_read < scalars_supposed) {
    fprintf(stderr, "Error: I read less value (%lu) than asked (%lu).\n",
                    scalar_read, scalars_supposed);
    errno = -1;
  }

  return;
}

void buffer2double_impur(char * const buffer, const long unsigned int scalars_supposed, double * const scalar_array){

  long unsigned int bytes_read = skip_lines(buffer, 2);

  long unsigned int scalar_possitions = bytes_read;
  long unsigned int scalar_read = 0;

  while (buffer[bytes_read]) {

    if (buffer[bytes_read] == '\n') {

      if (scalar_read > scalars_supposed) {
        fprintf(stderr, "Error: They are more value than (%lu) to read.\n",
                        scalar_read);
        errno = -1;
        return;
      }

        buffer[bytes_read] = '\0';
        scalar_array[scalar_read] = atof( & buffer[scalar_possitions]);
        scalar_read++;

        scalar_possitions = bytes_read + 1;
    }
    bytes_read++;
  }

  if (scalar_read < scalars_supposed) {
    fprintf(stderr, "Error: I read less value (%lu) than asked (%lu).\n",
                    scalar_read, scalars_supposed);
    errno = -1;
  }

  return;
}

void buffer2float_impur(char * const buffer, const long unsigned int scalars_supposed, float * const scalar_array){

  long unsigned int bytes_read = skip_lines(buffer, 2);

  long unsigned int scalar_possitions = bytes_read;
  long unsigned int scalar_read = 0;

  while (buffer[bytes_read]) {

    if (buffer[bytes_read] == '\n') {

      if (scalar_read > scalars_supposed) {
        fprintf(stderr, "Error: They are more value than (%lu) to read.\n",
                        scalar_read);
        errno = -1;
        return;
      }

        buffer[bytes_read] = '\0';
        scalar_array[scalar_read] = (float) atof( & buffer[scalar_possitions]);
        scalar_read++;

        scalar_possitions = bytes_read + 1;
    }
    bytes_read++;
  }

  if (scalar_read < scalars_supposed) {
    fprintf(stderr, "Error: I read less value (%lu) than asked (%lu).\n",
                    scalar_read, scalars_supposed);
    errno = -1;
  }

  return;
}
