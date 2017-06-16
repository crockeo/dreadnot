#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum mode_t { MALLOC, FREE };

const unsigned char *afl_postprocess(const unsigned char *in_buf, unsigned int *len)
{
    unsigned char buf[*len];
    memcpy(buf, in_buf, *len);

    char *line = strtok((char *)buf, "\n");
    while (line)
    {
        // Checking that we have correct modes.
        enum mode_t mode;
        char *tok = strtok(line, " ");
        if (!tok)
            return NULL;
        if (strcmp(tok, "malloc") == 0)
            mode = MALLOC;
        else if (strcmp(tok, "free") == 0)
            mode = FREE;
        else
            return NULL;

        // Checking that we have a valid name.
        tok = strtok(NULL, " ");
        if (!tok)
            return NULL;
        int tok_len = strlen(tok);
        for (int i = 0; i < tok_len; i++)
            if (tok[i] < '0' || tok[i] > '9')
                return NULL;

        // Checking that we have a valid size (if we're using malloc).
        if (mode == MALLOC)
        {
            tok = strtok(NULL, " ");
            if (!tok)
                return NULL;
            tok_len = strlen(tok);
            for (int i = 0; i < tok_len; i++)
                if (tok[i] < '0' || tok[i] > '9')
                    return NULL;

            line = strtok(NULL, "\n");
        }
    }

    return in_buf;
}
