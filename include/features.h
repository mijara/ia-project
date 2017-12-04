#ifndef FEATURES_H
#define FEATURES_H

#include "board.h"

int landing_height(struct board * board);

int eroded_piece_cells(struct board * board);

int holes(struct board * board);

int row_transitions(struct board * board);

int column_transitions(struct board * board);

int cumulative_wells(struct board * board);

#endif // FEATURES_H
