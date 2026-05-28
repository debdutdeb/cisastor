# Cisastor

Cisastor is a collection of C libraries and utilities.

## Documentation

- [Testing Guide](docs/testing/how-to.md) - Learn how to write and run tests using the built-in testing framework.

### Libraries

- [Arena Allocator](docs/libs/arena_allocator.md)
- **Data Containers**:
  - [Array List](docs/libs/containers/array_list.md)
  - [Iterator](docs/libs/containers/iterator.md)
  - [String View](docs/libs/containers/string_view.md)
- [Progress Bar](docs/libs/progressbar.md)
- [Testing Framework](docs/libs/testing.md)
- [Utils](docs/libs/utils.md)

## Project Structure

- `libs/`: Core libraries.
  - `arena_allocator/`: Arena-based memory management.
  - `data/`: Data structures (containers, string views, iterators).
  - `progressbar/`: Progress bar utility.
  - `testing/`: Minimal C testing framework.
  - `utils/`: Common macros and utilities.
- `examples/`: Usage examples for the libraries.
