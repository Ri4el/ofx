#ifndef AudioGlobalVariables_h
#define AudioGlobalVariables_h

extern vector<float> l_buff;
extern vector<float> r_buff;
extern ofSoundStream sound_stream;
extern float rms;
extern float lpeak;
extern float rpeak;
extern int buffer_counter;
extern int sample_rate;
extern int fft_size;
extern int n_bands;
extern int buffer_size;

#endif /* AudioGlobalVariables_h */
