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

#ifndef TEXTENCODE_H
#define TEXTENCODE_H

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))

#define RESIZE_ARRAY(ptr, nelems) ptr = realloc(ptr, (nelems) * sizeof(*ptr))

#define FATAL_ERROR(message, ...) \
do { \
    fprintf(stderr, message, ##__VA_ARGS__); \
    exit(1); \
} while (0)


struct String
{
    char *id;
    char *text;
};

extern struct String *gInputStrings;
extern int gInputStringsCount;

void read_input_file(const char *name);

#endif // TEXTENCODE_H
