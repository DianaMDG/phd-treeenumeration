uint16_t syndrome(uint16_t n, uint16_t k, uint16_t g, uint16_t w);
uint16_t encode(uint16_t n, uint16_t k, uint16_t g, uint16_t w);
uint16_t decode(uint16_t n, uint16_t k, uint16_t g, uint16_t *z, uint16_t w);
void make_codewords(uint16_t n, uint16_t k, uint16_t g, uint16_t *c, uint16_t *z, uint16_t s);
void make_syndromes(uint16_t n, uint16_t k, uint16_t g, uint16_t *s);
