#pragma once

#include <stddef.h>

const void* guifont_ibmplexmono_data();
size_t guifont_ibmplexmono_size();

const void* guifont_chicago_data();
size_t guifont_chicago_size();

void guifont_add_chicago_font(int size_in_pixels);
void guifont_add_ibmplexmono_font(int size_in_pixels);
