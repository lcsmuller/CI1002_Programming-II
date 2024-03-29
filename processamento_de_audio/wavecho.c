//GRR20197160 Lucas Müller

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "libwav.h"


int main(int argc, char *argv[])
{
  /*realiza a leitura dos argumentos fornecidos pelo usuário,
    -t: delay entre o eco e o original
    -l: intensidade do eco
    -i: desvio de input  
    -o: desvio de output */  
  FILE *out_stream = stdout, *inp_stream = stdin;
  float echo_rate = 0.5;
  long delay_ms = 1000;
  for (int i=1; i < argc; ++i){
    if ('-' == argv[i][0]){
      switch (argv[i][1]){
      case 't':
          delay_ms = strtol(argv[++i], NULL, 10);
          if ((0 > delay_ms) || (UINT32_MAX < delay_ms)){
            fprintf(stderr,"\nERRO: delay menor que 0 ou overflow\n\n");
            exit(EXIT_FAILURE);
          }
          break;
      case 'l':
          echo_rate = strtof(argv[++i], NULL);
          if ((echo_rate < 0.0) || (echo_rate > 1.0)){
            fprintf(stderr,"\nERRO: nivel de eco invalido\n\n");
            exit(EXIT_FAILURE);
          }
          break;
      case 'i':
          inp_stream = fopen(argv[++i],"rb");
          assert(NULL != inp_stream);
          break;
      case 'o':
          out_stream = fopen(argv[++i],"wb");
          assert(NULL != out_stream);
          break;
      default:
          fprintf(stderr,"\nERRO: Opcao invalida\n\n");
          exit(EXIT_FAILURE);
      }
    }
  }

  wav_st *wav = wav_init(inp_stream);
  
  int8_t *clone = malloc(wav->data.sub_chunk_2size);
  memcpy(clone, wav->audio_data.one_b, wav->data.sub_chunk_2size);
  
  /*percorre o .wav byte a byte independente de sua qtd de bits/canal
    e aplica a fórmula de echo por meio de um clone do audio original,
    à uma variável temporária, a fim de tratar os samples para overflow 
    antes de inserir no audio original*/
  int_fast64_t tmp;
  for (int i = delay_ms + 1; i < wav->data.sub_chunk_2size; ++i){
    tmp = (int8_t)(clone[i] + echo_rate * clone[i - delay_ms]);
    if (tmp > INT8_MAX)
      tmp = INT8_MAX;
    else if (tmp < INT8_MIN)
      tmp = INT8_MIN;

    wav->audio_data.one_b[i] = tmp;
  }

  fwrite(wav, 44, 1, out_stream);
  fwrite(wav->audio_data.one_b, wav->data.sub_chunk_2size, 1, out_stream);

  free(clone);
  wav_clean(wav);

  fclose(out_stream);
  fclose(inp_stream);

  return EXIT_SUCCESS;
}
