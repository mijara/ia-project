static int _last_id = -1;

static int _holes[512] = { -1 };
static int _column_transitions[512] = { -1 };
static int _heights[512] = { -1 };
static int _row_transitions[512] = { -1 };
static int _rows_filled[512] = { -1 };

void _reset_caches_if_new(struct board * board) {
    if (board->uid == _last_id) {
        return;
    }

    _last_id = board->uid;
    for (int x = 0; x < board->width; x++) {
        _holes[x] = -1;
        _column_transitions[x] = -1;
        _heights[x] = -1;
        _row_transitions[x] = -1;
        _rows_filled[x] = -1;
    }
}

int landing_height(struct board * board)
{
    int height = board->height + 1;

    for (int i = 0; i < 4; i++) {
        int y = board->ghosts[i * 2 + 1];

        if (y < height) {
            height = y;
        }
    }

    // NOTE: the return value when there's no GHOST pieces will be
    // height + 1. This should never happend though.
    return height;
}

int eroded_piece_cells(struct board * board)
{
    _reset_caches_if_new(board);

    // invalidate cache.
    for (int x = board->ghost_min_y; x <= board->ghost_max_y; x++) {
        _rows_filled[x] = -1;
    }
    for (int x = board->ghost_min_y_prev; x <= board->ghost_max_y_prev; x++) {
        _rows_filled[x] = -1;
    }

    int eroded_lines = 0;
    int eroded_cells = 0;

    for (int y = 0; y < board->height; y++) {
        int eroded;

        #ifdef OPTIMIZATIONS
        if (_rows_filled[y] != -1) {
            eroded = _rows_filled[y];
        } else {
            eroded = board_is_row_filled(board, y);
        }
        #else
        eroded = board_is_row_filled(board, y);
        #endif

        eroded_lines += eroded;

        if (eroded) {
            for (int i = 0; i < 4; i++) {
                int ghost_x = board->ghosts[i * 2];
                int ghost_y = board->ghosts[i * 2 + 1];

                if (ghost_y == y) {
                    eroded_cells += board_get(board, ghost_x, ghost_y) == BOARD_BLOCK_GHOST;
                }
            }
        }

        _rows_filled[y] = eroded;
    }

    return eroded_lines * eroded_cells;
}

/**
 * Counts all holes present on a board.
 * 
 * This function uses a cache to avoid counting columns that didn't changed.
 */
int holes(struct board * board)
{
    _reset_caches_if_new(board);

    // invalidate cache.
    for (int x = board->ghost_min_x; x <= board->ghost_max_x; x++) {
        _holes[x] = -1;
    }
    for (int x = board->ghost_min_x_prev; x <= board->ghost_max_x_prev; x++) {
        _holes[x] = -1;
    }

    int sum = 0;

    for (int x = 0; x < board->width; x++) {
        // check if the cache has the value.
        #ifdef OPTIMIZATIONS
        if (_holes[x] != -1) {
            sum += _holes[x];
            continue;
        }
        #endif

        int count = 0;
        int height = board_column_height(board, x);

        for (int y = 0; y < height - 1; y++) {
            if (!board_is_occupied(board, x, y) && y < height - 1) {
                count++;
            }
        }

        _holes[x] = count;
        sum += count;
    }

    return sum;
}

int row_transitions(struct board * board)
{
    _reset_caches_if_new(board);

    // invalidate cache.
    for (int y = board->ghost_min_y; y <= board->ghost_max_y; y++) {
        _row_transitions[y] = -1;
    }
    for (int y = board->ghost_min_y_prev; y <= board->ghost_max_y_prev; y++) {
        _row_transitions[y] = -1;
    }

    int sum = 0;

    for (int j = 0; j < board->height; j++) {
        #ifdef OPTIMIZATIONS
        if (_row_transitions[j] != -1) {
            sum += _row_transitions[j];
            continue;
        }
        #endif

        int count = 0;
        
        // the bottom outside of the board is considered as occupied.
        int state = BOARD_BLOCK_OCCUPIED;

        for (int i = 0; i < board->width; i++) {
            int cell = board_is_occupied(board, i, j);
            if (cell != state) {
                count++;
            }

            state = cell;
        }
        
        // the right outside of the board is considered as occupied.
        if (state != BOARD_BLOCK_OCCUPIED) {
            count++;
        }

        _row_transitions[j] = count;
        sum += count;
    }

    return sum;
}

/**
 * This function uses a cache to avoid counting columns that didn't changed.
 */
int column_transitions(struct board * board)
{
    _reset_caches_if_new(board);

    // invalidate cache.
    for (int x = board->ghost_min_x; x <= board->ghost_max_x; x++) {
        _column_transitions[x] = -1;
    }
    for (int x = board->ghost_min_x_prev; x <= board->ghost_max_x_prev; x++) {
        _column_transitions[x] = -1;
    }

    int sum = 0;

    for (int i = 0; i < board->width; i++) {
        #ifdef OPTIMIZATIONS
        // check if the cache has the value.
        if (_column_transitions[i] != -1) {
            sum += _column_transitions[i];
            continue;
        }
        #endif

        int count = 0;

        // the bottom outside of the board is considered as occupied.
        int state = BOARD_BLOCK_OCCUPIED;

        for (int j = 0; j < board->height; j++) {
            int cell = board_is_occupied(board, i, j);

            if (cell != state) {
                count++;
            }

            state = cell;
        }

        _column_transitions[i] = count;
        sum += count;
    }

    return sum;
}

int _arithmetic_sum(int n)
{
    return n < 0 ? 0 : n * (n + 1) / 2;
}

int _get_height(struct board * board, int x) {
    #ifdef OPTIMIZATIONS
    if (_heights[x] != -1) {
        return _heights[x];
    }
    #endif

    int height = board_column_height(board, x);

    _heights[x] = height;
    return _heights[x];
}

int cumulative_wells(struct board * board)
{
    _reset_caches_if_new(board);

    for (int x = board->ghost_min_x; x <= board->ghost_max_x; x++) {
        _heights[x] = -1;
    }
    for (int x = board->ghost_min_x_prev; x <= board->ghost_max_x_prev; x++) {
        _heights[x] = -1;
    }

    int sum = 0;

    // check the left-most column.
    int diff_left = _get_height(board, 1) - _get_height(board, 0);
    sum += _arithmetic_sum(diff_left);

    // check the middle of the board.
    for (int x = 1; x < board->width - 1; x++) {
        int height = _get_height(board, x);

        int diff_left = _get_height(board, x - 1) - height;
        int diff_right = _get_height(board, x + 1) - height;

        int minimum = diff_left < diff_right ? diff_left : diff_right;
        sum += _arithmetic_sum(minimum);
    }

    // check the right-most column.
    int diff_right = _get_height(board, board->width - 2) - _get_height(board, board->width - 1);
    sum += _arithmetic_sum(diff_right);

    return sum;
}
