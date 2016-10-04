/*
 * PROJECT: GEMMapper
 * FILE: gruntime.h
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#ifndef GRUNTIME_H_
#define GRUNTIME_H_

#include "system/commons.h"

/*
 * GEM Runtime
 */
void gruntime_init(
    const uint64_t num_threads,
    char* const tmp_folder);
void gruntime_destroy(void);

#endif /* GRUNTIME_H_ */
