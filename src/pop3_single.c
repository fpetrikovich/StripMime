#include "pop3_multi.h"

const char *
pop3_single_event(enum pop3_single_type type) {
    const char *ret;
    switch(type) {
        case POP3_SINGLE__BYTE:
            ret = "byte(c)";
            break;
        case POP3_SINGLE_WAIT:
            ret = "wait()";
            break;
        case POP3_SINGL_END:
            ret = "end()";
            break;
    }
    return ret;
}

enum {
    NEWLINE,
    BYTE,
    CR,
    END,
};

static void
byte(struct parser_event *ret, const uint8_t c) {
    ret->type    = POP3_SINGLE_BYTE;
    ret->n       = 1;
    ret->data[0] = c;
}

static void
byte_cr(struct parser_event *ret, const uint8_t c) {
    byte(ret, '\r');
}

static void
wait(struct parser_event *ret, const uint8_t c) {
    ret->type    = POP3_SINGLE_WAIT;
    ret->n       = 0;
}

static void
end(struct parser_event *ret, const uint8_t c) {
    ret->type    = POP3_SINGLE_END;
    ret->n       = 0;
}

static const struct parser_state_transition ST_NEWLINE[] =  {
    {.when = '.',        .dest = DOT,         .act1 = wait,},
    {.when = ANY,        .dest = BYTE,        .act1 = byte,},
};

static const struct parser_state_transition ST_BYTE[] =  {
    {.when = '\r',       .dest = CR,          .act1 = wait,},
    {.when = ANY,        .dest = BYTE,        .act1 = byte,},
};

static const struct parser_state_transition ST_CR[] =  {
    {.when = '\n',       .dest = NEWLINE,     .act1 = byte_cr, .act2 = byte},
    {.when = ANY,        .dest = BYTE,        .act1 = byte_cr, .act2 = byte},
};

static const struct parser_state_transition ST_END[] =  {
    {.when = ANY,        .dest = BYTE,        .act1 = byte_cr, .act2 = byte},
};

///////////////////////////////////////////////////////////////////////////////
// Declaración formal

static const struct parser_state_transition *states [] = {
    ST_NEWLINE,
    ST_BYTE,
    ST_CR,
    ST_END,
};

#define N(x) (sizeof(x)/sizeof((x)[0]))

static const size_t states_n [] = {
    N(ST_NEWLINE),
    N(ST_BYTE),
    N(ST_CR),
    N(ST_END),
};

static struct parser_definition definition = {
    .states_count = N(states),
    .states       = states,
    .states_n     = states_n,
    .start_state  = NEWLINE,
};

const struct parser_definition *
pop3_single_parser(void) {
    return &definition;
}
