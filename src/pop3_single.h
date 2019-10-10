#ifndef POP_SINGLE
#define POP_SINGLE

#include "parser.h"


enum pop3_single_type {
    /** N bytes nuevos*/
    POP3_SINGLE_BYTE,
    /** hay que esperar, no podemos decidir */
    POP3_SINGLE_WAIT,
    /** la respuesta está completa */
    POP3_SINGLE_END,
};

/** la definición del parser */
const struct parser_definition * pop3_single_parser(void);

const char *
pop3_single_event(enum pop3_single_type type);

#endif
