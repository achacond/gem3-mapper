/*
 * PROJECT: GEMMapper
 * FILE: approximate_search.h
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#ifndef APPROXIMATE_SEARCH_H_
#define APPROXIMATE_SEARCH_H_

#include "search_parameters.h"
#include "archive.h"
#include "pattern.h"
#include "region_profile.h"
#include "interval_set.h"
#include "filtering_candidates.h"
#include "mapper_profile.h"

/*
 * Debug
 */
#define DEBUG_SEARCH_STATE          GEM_DEEP_DEBUG

/*
 * Approximate Search State
 */
typedef enum {
  asearch_begin = 0,                     // Beginning of the search
  asearch_no_regions = 1,                // While doing the region profile no regions were found
  asearch_exact_matches = 2,             // One maximum region was found (exact results)
  asearch_exact_filtering_adaptive = 3,  // Region-Minimal Profile (Adaptive) + Exact candidate generation
  asearch_verify_candidates = 4,         // Verify candidates
  asearch_candidates_verified = 5,       // Candidates verified
  asearch_exact_filtering_boost = 6,     // Boost Region-Profile + Exact candidate generation
  asearch_inexact_filtering = 7,         // Region-Delimit Profile (Adaptive) + Approximate candidate generation
  asearch_neighborhood = 8,              // Neighborhood search
  asearch_end = 9,                       // End of the current workflow
  asearch_read_recovery = 10,            // Read recovery
  asearch_unbounded_alignment = 11,      // Unbounded Alignment Search
  asearch_probe_candidates = 12          // Probe candidates (try to lower max-differences) // TODO
} approximate_search_state_t;
extern const char* approximate_search_state_label[13];

/*
 * Approximate Search
 */
typedef struct {
  /* Index Structures, Pattern & Parameters */
  archive_t* archive;                                   // Archive
  pattern_t pattern;                                    // Search Pattern
  as_parameters_t* as_parameters; // Search Parameters (Evaluated to read-length)
  /* Search State */
  bool emulated_rc_search;                              // Currently searching on the RC (emulated on the forward strand)
  bool do_quality_search;                               // Quality search
  approximate_search_state_t search_state;              // Current State of the search
  bool stop_before_neighborhood_search;                 // Stop before Neighborhood Search
  uint64_t max_complete_error;
  uint64_t max_complete_stratum;                        // Maximum complete stratum reached by the search
  uint64_t max_matches_reached;                         // Quick abandon due to maximum matches found
  uint64_t lo_exact_matches;                            // Interval Lo (Exact matching)
  uint64_t hi_exact_matches;                            // Interval Hi (Exact matching)
  /* Search Structures */
  region_profile_t region_profile;                      // Region Profile
  filtering_candidates_t* filtering_candidates;         // Filtering Candidates
  /* BPM Buffer */
  uint64_t bpm_buffer_offset;
  uint64_t bpm_buffer_candidates;
  /* Search Auxiliary Structures (external) */
  text_collection_t* text_collection;                   // Stores text-traces
  interval_set_t* interval_set;                         // Interval Set
  /* MM */
  mm_stack_t* mm_stack;                                 // MM-Stack
} approximate_search_t;

/*
 * Setup
 */
void approximate_search_init(
    approximate_search_t* const search,archive_t* const archive,
    as_parameters_t* const as_parameters,const bool emulated_rc_search);
void approximate_search_configure(
    approximate_search_t* const search,filtering_candidates_t* const filtering_candidates,
    text_collection_t* text_collection,interval_set_t* const interval_set,mm_stack_t* const mm_stack);
void approximate_search_reset(approximate_search_t* const search);
void approximate_search_destroy(approximate_search_t* const search);

/*
 * Accessors
 */
uint64_t approximate_search_get_num_filtering_candidates(const approximate_search_t* const search);
uint64_t approximate_search_get_num_exact_filtering_candidates(const approximate_search_t* const search);
void approximate_search_update_mcs(approximate_search_t* const search,const uint64_t max_complete_stratum);

/*
 * Modifiers
 */
void approximate_search_hold_verification_candidates(approximate_search_t* const search);
void approximate_search_release_verification_candidates(approximate_search_t* const search);

/*
 * Aproximate String Search
 */
void approximate_search(approximate_search_t* const search,matches_t* const matches);

/*
 * Display
 */
void approximate_search_print(FILE* const stream,approximate_search_t* const search);

#endif /* APPROXIMATE_SEARCH_H_ */
