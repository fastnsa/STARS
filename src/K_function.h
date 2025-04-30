#pragma once
#ifndef K_FUNCTION_H
#define K_FUNCTION_H

#include "Network.h"
#include "SP.h"
#include "count.h"
#include "sam_count.h"

void call_RQS(model& our_model);
void call_SPS(model& our_model);
void call_ASPS(model& our_model);
void call_STARS(model& our_model);
void compute_K_function(model& our_model);

#endif