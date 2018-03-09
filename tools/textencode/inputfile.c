// Copyright(c) 2018 camthesaxman
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "textencode.h"

struct String *gInputStrings = NULL;
int gInputStringsCount = 0;

struct ControlCode
{
    char *id;
    uint8_t *code;
    int codeLength;
};

static struct ControlCode *controlCodes = NULL;
static int controlCodesCount = 0;

static size_t inputFileSize;
static char *inputFileBuffer;
static int lineNum;


static void syntax_error(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    fprintf(stderr, "error: line %i: ", lineNum);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);

    va_end(args);

    exit(1);
}

static char *strdup_(const char *str)
{
    int length = strlen(str);
    char *buffer = malloc(length + 1);

    strcpy(buffer, str);
    return buffer;
}

static void add_input_string(char *id, char *string)
{
    int index = gInputStringsCount;
    int i;

    // check if ID already exists
    for (i = 0; i < index; i++)
        if (strcmp(id, gInputStrings[i].id) == 0)
            syntax_error("string ID %s already exists", id);

    gInputStringsCount++;
    RESIZE_ARRAY(gInputStrings, gInputStringsCount);

    gInputStrings[index].id = strdup_(id);
    gInputStrings[index].text = strdup_(string);
}

static void add_control_code(char *id, uint8_t *code, int codeLength)
{
    int index = controlCodesCount;
    int i;

    // check if ID already exists
    for (i = 0; i < index; i++)
        if (strcmp(id, controlCodes[i].id) == 0)
            syntax_error("control code [%s] is already defined", id);

    controlCodesCount++;
    RESIZE_ARRAY(controlCodes, controlCodesCount);

    controlCodes[index].id = strdup_(id);
    controlCodes[index].code = code;
    controlCodes[index].codeLength = codeLength;
}

static int is_valid_string_id_char(int c)
{
    return (isalnum(c) || c == '_');
}

static int is_valid_ctrlcode_id_char(int c)
{
    // FEditor allows '.' in control codes, so I'm keeping compatibility with that.
    return (isalnum(c) || c == '_' || c == '.');
}

static int hex_digit(int c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 0xA;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 0xA;
    return -1;  // not a hex digit
}

static int is_hex_digit(int c)
{
    return (hex_digit(c) != -1);
}

static int decimal_digit(int c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    return -1;  // not a decimal digit
}

static int is_decimal_digit(int c)
{
    return (decimal_digit(c) != -1);
}

// parses a number and returns a pointer to the end of it
// returns NULL if no valid number could be parsed
static char *parse_number(char *str, int *value)
{
    int n = 0;
    int digit;

    if (str[0] == '0' && tolower(str[1]) == 'x')  // hex number
    {
        str += 2;
        if (!is_hex_digit(*str))
            return NULL;
        while (1)
        {
            digit = hex_digit(*str);
            if (digit == -1)
                break;
            n = n * 16 + digit;
            str++;
        }
    }
    else  // decimal number
    {
        if (!is_decimal_digit(*str))
            return NULL;
        while (1)
        {
            digit = decimal_digit(*str);
            if (digit == -1)
                break;
            n = n * 10 + digit;
            str++;
        }
    }

    *value = n;
    return str;
}

// copies string from src to dest, expanding control codes
static void expand_control_codes(char *src, char *dest)
{
    while (*src != 0)
    {
        switch (*src)
        {
        case '\n':  // silently ignore line breaks
            src++;

            lineNum++;
            break;

        case '[':  // control code
            src++;

            if (isdigit(*src))  // raw number
            {
                int value;

                src = parse_number(src, &value);
                if (src == NULL || *src != ']')
                    syntax_error("invalid number in control code");
                src++;

                *dest++ = value;
            }
            else  // identifier
            {
                char *id;
                const struct ControlCode *ctrlCode = NULL;
                int i;

                // extract identifier
                id = src;
                src = strchr(src, ']');
                if (src == NULL)
                    syntax_error("unterminated [");
                *src = 0;
                src++;

                // find control code
                for (i = 0; i < controlCodesCount; i++)
                {
                    if (strcmp(id, controlCodes[i].id) == 0)
                    {
                        ctrlCode = &controlCodes[i];
                        break;
                    }
                }
                if (ctrlCode == NULL)
                    syntax_error("unknown control code [%s]", id);

                // copy control code to dest
                for (i = 0; i < ctrlCode->codeLength; i++)
                    *dest++ = ctrlCode->code[i];
            }
            break;

        case ']':
            syntax_error("] without matching [");
            break;

        default:
            *dest++ = *src++;
            break;
        }
    }
    *dest = 0;
}

char *skip_whitespace_and_comments(char *str)
{
    int isLineStart = 0;

    // needed to allow comments on first line of file
    if (str == inputFileBuffer)
        isLineStart = 1;

    while (1)
    {
        if (isspace(*str))
        {
            if (*str == '\n')
            {
                isLineStart = 1;
                lineNum++;
            }

            str++;
        }
        else
        {
            // Handle comments
            // # starts a comment if it is the first non-whitespace character on a line
            if (*str == '#' && isLineStart)
            {
                // Skip to the end of the line
                while (*str != '\n' && *str != 0)
                    str++;
            }
            else
            {
                // not whitespace and not a comment
                return str;
            }
        }
    }
}

// skips all whitespace until a newline char or non-whitespace char is encountered
char *skip_whitespace_no_newline(char *str)
{
    while (isspace(*str))
    {
        if (*str == '\n')
            break;
        str++;
    }
    return str;
}

// parses a control code definition and returns a pointer to the end of it
char *parse_ctrlcode_def(char *str)
{
    char *identStart;
    char *identEnd;
    uint8_t *code = NULL;
    int codeLength = 0;

    str++;  // skip opening [

    // identifier
    identStart = str;
    if (isdigit(*str))
        syntax_error("identifier cannot start with digit");
    while (is_valid_ctrlcode_id_char(*str))
        str++;

    // closing bracket
    identEnd = str;
    if (*str != ']')
        syntax_error("invalid identifier character '%c'", *str);
    str++;

    str = skip_whitespace_no_newline(str);

    // equal sign
    if (*str != '=')
        syntax_error("expected = after control code identifier");

    // value list
    do
    {
        int value;

        str++;  // skip equal sign or comma

        str = skip_whitespace_no_newline(str);

        // number
        str = parse_number(str, &value);
        if (str == NULL)
            syntax_error("expected number as right-hand value");
        codeLength++;
        RESIZE_ARRAY(code, codeLength);
        code[codeLength - 1] = value;

        str = skip_whitespace_no_newline(str);

    } while (*str == ',');  // values separated by commas

    if (*str != '\n' && *str != 0)
        syntax_error("junk at end of line");

    *identEnd = 0;

    add_control_code(identStart, code, codeLength);

    return str;
}

// parses a string entry and returns a pointer to the end of it
char *parse_string_entry(char *str)
{
    char *identStart;
    char *identEnd;
    char *stringStart;
    char *stringEnd;
    char *buffer = malloc(5000);  // TODO: size this buffer dynamically

    // identifier
    identStart = str;
    if (isdigit(*str))
        syntax_error("identifier cannot start with digit");
    while (is_valid_string_id_char(*str))
        str++;
    identEnd = str;

    str = skip_whitespace_and_comments(str);

    // opening bracket
    if (*str != '{')
        syntax_error("expected { after identifier");
    str++;

    // string
    stringStart = str;

    // closing bracket
    str = strchr(str, '}');
    stringEnd = str;
    if (str == NULL)
        syntax_error("unterminated {");
    str++;

    *identEnd = 0;
    *stringEnd = 0;

    expand_control_codes(stringStart, buffer);
    RESIZE_ARRAY(buffer, strlen(buffer) + 1);  // shrink buffer to avoid wasting memory for now
    add_input_string(identStart, buffer);

    return str;
}

void parse_contents(void)
{
    char *pos = inputFileBuffer;

    lineNum = 1;
    while (1)
    {
        // move to next non-whitespace character
        pos = skip_whitespace_and_comments(pos);

        if (*pos == 0)  // end of file
            break;

        // handle statement
        else if (*pos == '[')
            pos = parse_ctrlcode_def(pos);
        else if (is_valid_string_id_char(*pos))
            pos = parse_string_entry(pos);
        else
            syntax_error("unexpected character '%c'", *pos);
    }
}

void read_input_file(const char *filename)
{
    FILE *file;

    // open file
    file = fopen(filename, "rb");
    if (file == NULL)
        FATAL_ERROR("could not open file '%s' for reading\n", filename);

    // get size
    fseek(file, 0, SEEK_END);
    inputFileSize = ftell(file);

    // read file contents
    inputFileBuffer = malloc(inputFileSize + 1);
    fseek(file, 0, SEEK_SET);
    if (fread(inputFileBuffer, inputFileSize, 1, file) != 1)
        FATAL_ERROR("error reading data from file '%s'\n", filename);
    inputFileBuffer[inputFileSize] = 0;

    // close file
    fclose(file);

    // get string list
    parse_contents();
}
