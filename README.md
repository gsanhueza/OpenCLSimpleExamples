# OpenCL C++ Examples

These are simple C++ examples to start programming in OpenCL.

## Device info

Prints your currently installed OpenCL implementations.

## Main_*

These are 3 variants of the same code, a vector addition program.

### Original

Standard implementation.

### Vector

Uses vectors and iterators to start buffers. In addition, arguments are passed with an *EnqueueArgs* object instead of manually setting them.

### Minimal

Same as *Vector*, but with less code, uses a default context.
