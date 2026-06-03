# Documentation guideline
You are documenting a systems programming library written in C.

It is ok to not follow a section of this if doesn't apply to current library concepts.

Your audience is a developer who has never seen this project before but knows basic C.

Generate COMPLETE USER-FACING documentation for the library, not implementation notes.

The goal is that after reading the documentation, a user should be able to:

* Understand what problem the library solves.
* Understand when to use it.
* Understand ownership and allocation semantics.
* Use the library without reading the source code.
* Avoid common mistakes.

Documentation requirements:

# Structure

## Overview

Explain:

* What the library is.
* Why it exists.
* Typical use cases.
* Relationship to other libraries in the project.

## Why?

Explain:

* What problem this abstraction solves.
* Why a user would choose it over raw C alternatives.
* Tradeoffs.

## Concepts

Explain all important concepts before showing APIs.

Examples:

* Ownership
* Iteration
* Views vs owned data
* Arena allocation
* Mutable vs immutable data

## Quick Start

Provide the smallest useful example.

A user should be able to copy-paste it.

## Common Usage Patterns

Show realistic examples.

Examples:

* Creating values
* Appending data
* Iterating
* Parsing
* Transforming data
* Interoperating with standard C APIs

Prefer multiple short examples over one giant example.

## Memory Ownership

For every API that allocates or returns references, explicitly state:

* Who owns the memory.
* Whether memory is copied.
* Whether memory may become invalid after mutation.
* Arena allocation behavior.

This section is mandatory.

## API Reference

For every public function:

### function_name

Purpose.

Parameters.

Return value.

Ownership semantics.

Failure cases.

Short example.

Do not simply restate the function signature.

## Common Pitfalls

Document mistakes users are likely to make.

Examples:

* Invalidated pointers.
* Iterator exhaustion.
* Arena lifetime assumptions.
* String ownership.
* Mutation while iterating.

## Performance Characteristics

Document:

* Time complexity.
* Allocation behavior.
* Reallocation behavior.
* Copying behavior.

Approximate descriptions are acceptable.

## Related Libraries

Explain how this library interacts with:

* array_list
* iterator
* string
* string_builder
* string_view
* arena
* json
* mover

when applicable.

# Style

* Write as if this were a public open source library.
* Be concise.
* Prefer examples over prose.
* Avoid implementation details unless they affect API behavior.
* Assume the reader does not know the source code.
* Explain semantics, not code.

The resulting documentation should feel similar in quality to Go standard library documentation or Rust crate documentation.
