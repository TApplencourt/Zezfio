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

void gzip2buffer            (const char *, const size_t, char * const);
size_t skip_lines           (const char * buffer, const int);
void buffer2int_impur       (char * const, const size_t, int    * const);
void buffer2long_impur      (char * const, const size_t, long   * const);
void buffer2double_impur    (char * const, const size_t, double * const);
void buffer2float_impur     (char * const, const size_t, float  * const);
void buffer2bool_impur      (char * const, const size_t, int    * const);
void buffer2char            (char * const, const size_t, const size_t, char * const);

/***
 *     ___                   ./                                 
 *      |  ._ _  ._  |  _  ._ _   _  ._ _|_  _. _|_ o  _  ._  
 *     _|_ | | | |_) | (/_ | | | (/_ | | |_ (_|  |_ | (_) | | 
 *               |                                            
 */

void gzip2buffer(const char * filename,
              const size_t bytes_expected,
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

size_t skip_lines(const char * buffer, const int number_of_line){
  int ligne_read = 0;
  size_t bytes_read = 0;

  while (buffer[bytes_read]) {
    if (buffer[bytes_read] == '\n') {
        
        if (ligne_read == number_of_line-1) break;
        ligne_read ++;
    }
    bytes_read ++;
    }

    return bytes_read + 1;
}

//Start of copy pasta for int/long/double/float/"bool"
void buffer2int_impur(char * const buffer, const size_t lines_supposed, int * const scalar_array){

  size_t bytes_read = skip_lines(buffer, 2);

  size_t scalar_position = bytes_read;
  size_t scalar_read = 0;

  while (buffer[bytes_read]) {

    if (buffer[bytes_read] == '\n') {

      if (scalar_read > lines_supposed) {
        fprintf(stderr, "Error: They are more value than (%lu) to read.\n",
                        scalar_read);
        errno = -1;
        return;
      }

        buffer[bytes_read] = '\0';
        scalar_array[scalar_read] = atoi( & buffer[scalar_position]);
        scalar_read++;

        scalar_position = bytes_read + 1;
    }
    bytes_read++;
  }

  if (scalar_read < lines_supposed) {
    fprintf(stderr, "Error: I read less value (%lu) than asked (%lu).\n",
                    scalar_read, lines_supposed);
    errno = -1;
  }

  return;
}

void buffer2long_impur(char * const buffer, const size_t lines_supposed, long * const scalar_array){

  size_t bytes_read = skip_lines(buffer, 2);

  size_t scalar_position = bytes_read;
  size_t scalar_read = 0;

  while (buffer[bytes_read]) {

    if (buffer[bytes_read] == '\n') {

      if (scalar_read > lines_supposed) {
        fprintf(stderr, "Error: They are more value than (%lu) to read.\n",
                        scalar_read);
        errno = -1;
        return;
      }

        buffer[bytes_read] = '\0';
        scalar_array[scalar_read] = atol( & buffer[scalar_position]);
        scalar_read++;

        scalar_position = bytes_read + 1;
    }
    bytes_read++;
  }

  if (scalar_read < lines_supposed) {
    fprintf(stderr, "Error: I read less value (%lu) than asked (%lu).\n",
                    scalar_read, lines_supposed);
    errno = -1;
  }

  return;
}

void buffer2double_impur(char * const buffer, const size_t lines_supposed, double * const scalar_array){

  size_t bytes_read = skip_lines(buffer, 2);

  size_t scalar_position = bytes_read;
  size_t scalar_read = 0;

  while (buffer[bytes_read]) {

    if (buffer[bytes_read] == '\n') {

      if (scalar_read > lines_supposed) {
        fprintf(stderr, "Error: They are more value than (%lu) to read.\n",
                        scalar_read);
        errno = -1;
        return;
      }

        buffer[bytes_read] = '\0';
        scalar_array[scalar_read] = atof( & buffer[scalar_position]);
        scalar_read++;

        scalar_position = bytes_read + 1;
    }
    bytes_read++;
  }

  if (scalar_read < lines_supposed) {
    fprintf(stderr, "Error: I read less value (%lu) than asked (%lu).\n",
                    scalar_read, lines_supposed);
    errno = -1;
  }

  return;
}

void buffer2float_impur(char * const buffer, const size_t lines_supposed, float * const scalar_array){

  size_t bytes_read = skip_lines(buffer, 2);

  size_t scalar_position = bytes_read;
  size_t scalar_read = 0;

  while (buffer[bytes_read]) {

    if (buffer[bytes_read] == '\n') {

      if (scalar_read > lines_supposed) {
        fprintf(stderr, "Error: They are more value than (%lu) to read.\n",
                        scalar_read);
        errno = -1;
        return;
      }

        buffer[bytes_read] = '\0';
        scalar_array[scalar_read] = (float) atof( & buffer[scalar_position]);
        scalar_read++;

        scalar_position = bytes_read + 1;
    }
    bytes_read++;
  }

  if (scalar_read < lines_supposed) {
    fprintf(stderr, "Error: I read less value (%lu) than asked (%lu).\n",
                    scalar_read, lines_supposed);
    errno = -1;
  }

  return;
}

void buffer2bool_impur(char * const buffer, const size_t lines_supposed, int * const scalar_array){

  size_t bytes_read = skip_lines(buffer, 2);

  size_t scalar_position = bytes_read;
  size_t scalar_read = 0;

  while (buffer[bytes_read]) {

    if (buffer[bytes_read] == '\n') {

      if (scalar_read > lines_supposed) {
        fprintf(stderr, "Error: They are more value than (%lu) to read.\n",
                        scalar_read);
        errno = -1;
        return;
      }

        buffer[bytes_read] = '\0';

        const int bool_read = atoi( & buffer[scalar_position]);
        if ( !(bool_read == 0 || bool_read == 1) ) {
          fprintf(stderr, "Error: Bool value need to be eather 1 or 0 not (%d).\n",
                          bool_read);
          errno = -1;
          return;
        }

        scalar_array[scalar_read] = bool_read;
        scalar_read++;

        scalar_position = bytes_read + 1;
    }
    bytes_read++;
  }

  if (scalar_read < lines_supposed) {
    fprintf(stderr, "Error: I read less value (%lu) than asked (%lu).\n",
                    scalar_read, lines_supposed);
    errno = -1;
  }

  return;
}

void buffer2char(char * const buffer, const size_t lines_supposed,const size_t padding, char * const char_array){

  size_t bytes_read = skip_lines(buffer, 2);

  size_t size_string = 0;
  size_t offset = 0;
  size_t char_array_position = 0;

  while (buffer[bytes_read]) {

    if (!(buffer[bytes_read] == '\n')){

      char_array[char_array_position] = buffer[bytes_read];
      size_string++;
    }

    else{

      for (size_t i=0; i<padding-size_string; i++, char_array_position++){
        char_array[char_array_position] = ' '; 
      }
      char_array[char_array_position]='\0';

      size_string = 0;
    }

    bytes_read++;
    char_array_position++;
  }

  char_array[char_array_position] = '\0';

  return;
}