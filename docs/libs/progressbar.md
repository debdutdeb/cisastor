# Progress Bar

A drop-in replacement for `std::ostream` like functionality to display progress in the terminal.

## API

### Types

#### `ProgressBarOptions`
- `int bytes_length`: Total size to reach 100%.
- `char *label`: Label displayed next to the bar.

### Functions

#### `FILE *progressbar_open(FILE *fp, ProgressBarOptions *options)`
Returns a `FILE` pointer that, when written to, updates the progress bar on `stderr` while passing data through to the original `fp`.
