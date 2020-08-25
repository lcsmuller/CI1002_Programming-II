#include <inttypes.h>

typedef struct {
  char chunk_id[4];     //identifica tipo de arquivo
  uint32_t chunk_size;  //tamanho do arquivo em bytes (s/id e size)
  char format[4];       //define formato do restante do conteúdo
} wav_riff_t;

typedef struct {
  char sub_chunk_1id[4];        //cabeçalho do chunk
  uint32_t sub_chunk_1size;     //tamanho deste chunk
  uint16_t audio_format;        //codificação utilizada
  uint16_t num_channels;         //qtd canais de áudio
  uint32_t sample_rate;         //taxa de amostragem (amostras/seg)
  uint32_t byte_rate;           //taxa de bytes por segundo
  uint16_t block_align;         //bytes por amostra (soma dos canais)
  uint16_t bits_sample;         //bits por amostra, por canal
} wav_fmt_t;

typedef struct {
  char sub_chunk_2id[4];        //cabeçalho do chunk;
  uint32_t sub_chunk_2size;     //espaço ocupado pelas amostras(bytes)
} wav_data_t;

typedef struct {
  wav_riff_t riff;              //(12 bytes)
  wav_fmt_t fmt;                //(24 bytes)
  wav_data_t data;              //(8 bytes)
  uint16_t *audio_data;         //amostras de áudio
  size_t samples_channel;       //amostras por canal
} wav_t;

wav_t* wav_init(FILE* wav_sample);
void wav_clean(wav_t* wav);
