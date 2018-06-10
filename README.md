# utils
Some utilities to share

## Union-Find

The folder union_find contains an implementation of the Union-Find
algorithm with a rewind feature to undo operations on the
structure. See the examples in union_find/examples to see how to use
it.

## Workflow

The folder workflow provides a framework for implementing a module
based workflow, each module representing an algorithm with input,
output and parameters. It is designed using Partial Template
Specialization so that it can be integrated in any other
framework. The workflow provides many options for easily implementing
modules from existing algorithms, connecting them, creating
executables with command line options and analyzing the workflow
itself. See the examples in workflow/examples to see how to use it.

###Todo

The base is done, we now need to implement the features.