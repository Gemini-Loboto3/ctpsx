int Sound_get_pan(int x);

void Sound_set_vol(const char* name, int vol);
void Sound_set_pan(const char* name, int pan);
void Sound_play(const char* name, int loops, int pan, int vol);
void Sound_stop(const char* name);

int Sound_clear(int index);
