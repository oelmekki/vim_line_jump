#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char ** options;
  char ** files;
  int     options_count;
  int     files_count;
} Data;


char * compute_command( char * command, Data * data )
{
  int size = strlen( command ) + 1;
  int i;

  for ( i = 0; i < (*data).options_count; i++ )
  {
    size += strlen( (*data).options[i] ) + 2;
    command = realloc( command, ( size * sizeof( char ) ) );
    sprintf( command, "%s %s ", command, (*data).options[i] );
  }

  for ( i = 0; i < (*data).files_count; i++ )
  {
    size += strlen( (*data).files[i] ) + 2;
    command = realloc( command, ( size * sizeof( char ) ) );
    sprintf( command, "%s %s ", command, (*data).files[i] );
  }

  return command;
}


void too_bad( Data * data )
{
  char * vim      = "/usr/bin/vim -p ";
  char * command  = malloc( strlen( vim ) * sizeof( char ) );
  strcpy( command, vim );
  system( compute_command( command, data ) );
}



void roxor_linator( Data * data )
{
  char * file;
  char * line_no;
  char * vim      = "/usr/bin/vim -p ";
  char * command  = malloc( strlen( vim ) * sizeof( char ) );

  strcpy( command, vim );

  if ( strstr( (*data).files[0], ":" ) )
  {
    file    = malloc( strlen( (*data).files[0] ) * sizeof( char ) );
    line_no = malloc( strlen( (*data).files[0] ) * sizeof( char ) );
    file    = strtok( (*data).files[0], ":" );
    line_no = strtok( NULL, ":" );
    command = compute_command( command, data );
    command = realloc( command, ( ( strlen( line_no ) + 2 ) * sizeof( char ) ) );
    sprintf( command, "%s %s%s", command, " +", line_no );

    system( command );
  }

  else
  {
    too_bad( data );
  }
}



int main( int argc, char **argv )
{
  int     i;
  char *  p;
  Data    data;

  data.files          = malloc( argc * sizeof( char * ) );
  data.options        = malloc( argc * sizeof( char * ) );
  data.files_count    = 0;
  data.options_count  = 0;

  for ( i = 1; i < argc; i++ )
  {
    if ( ( p = strstr( argv[i], "-" ) ) && p - argv[i] == 0 )
    {
      data.options[ data.options_count ] = malloc( strlen( argv[i] ) * sizeof( char ) );
      strcpy( data.options[ data.options_count ], argv[i] );
      data.options_count++;
    }

    else
    {
      data.files[ data.files_count ] = malloc( strlen( argv[i] ) * sizeof( char ) );
      strcpy( data.files[ data.files_count ], argv[i] );
      data.files_count++;
    }
  }

  if ( data.files_count == 1 )
  {
    roxor_linator( &data );
  }

  else
  {
    too_bad( &data );
  }

  return 0;
}
