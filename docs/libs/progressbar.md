# Progress Bar

The Progress Bar library provides a simple way to display visual feedback for long-running tasks in the terminal. It acts as a wrapper around a `FILE` stream (like `stdout`), automatically updating a progress bar on `stderr` as you write data.

## Usage

```c
#include "progressbar.h"

void download_file(FILE *dest, size_t total_size) {
    ProgressBarOptions options = {
        .bytes_length = total_size,
        .label = "Downloading"
    };

    // Open a progress-aware stream
    FILE *pb_stream = progressbar_open(dest, &options);
    
    char buffer[1024];
    size_t read;
    while ((read = get_data(buffer, 1024)) > 0) {
        // Writing to pb_stream automatically updates the bar on stderr
        fwrite(buffer, 1, read, pb_stream);
    }

    // Closing the stream finishes the progress bar
    fclose(pb_stream);
}
```

## API Reference

### `ProgressBarOptions`
| Field | Type | Description |
| --- | --- | --- |
| `bytes_length` | `int` | The total number of bytes to be written to reach 100%. |
| `label` | `char *` | The text label to display next to the bar. |

### Functions

#### `FILE *progressbar_open(FILE *fp, ProgressBarOptions *options)`
Creates a new `FILE` stream that wraps `fp`.
- **Returns**: A new `FILE` pointer, or `NULL` on error.
- **Side Effect**: Every write to the returned stream updates a progress bar on `stderr`.
- **Cleanup**: You must call `fclose()` on the returned pointer when finished.
