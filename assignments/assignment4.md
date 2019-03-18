# Assignment 4: Automated Testing

Due February 21, 2019 at 11:59 PM

Team Name: nullptr

Team Members: Ryan Alder, Danila Fedorin, Matthew Sessions

Github Project Fork URL: [https://github.com/sessionm21/abcs](https://github.com/sessionm21/abcs)

## Test Plan
We decided to use the `greatest` testing framework, which is a simple unit testing framework for C.
We settled on this because `libabacus`, the project that powers `abcs`, is written in C, and thus
doesn't require an overkill testing suite. We were considering doing property-based testing,
but decided that the functions in `abcs` do not have any clear properties besides those
that are easy to test with regular unit tests.

We chose to test three source files, each with crucial importance. The first is `refcount.c`, a file
which houses the reference counting pointer implementation that is used by `abcs`. Reference counting
is needed because there is no clear ownership of an object by another in `abcs` code (for instance,
a function isn't owned by a scope, since a reference to that function can be assigned to a variable
and thus duplicated into another scope). Reference counting provides a sane yet powerful way
to make sure unused memory is cleared, and is used throughout the several thousand lines of
code comprising the libabacus library. Thus, we needed to test it to make sure that the foundation
on which we are standing are sound. 

The same was the case with `trie.c`, which is another
source file that we tested. `abcs` uses tries for maps of strings to other things, which
are very common - the "scope" implementation is a chain of tries, each pointing
to a parent scope. This means that all variable name lookups (including those for `+` and other
basic operators) go through tries. We needed to make sure that the trie code was working
properly, since a lot of the project is based on it.

Lastly, we tested the `gc.c` file. This was in part because garbage collection is what
complements reference coutning for freeing unused memory: reference counting doesn't work
when there are "cycles" in the references (i.e. one object pointing to another which
points back at the first object), because it means that both objects in the cycle
always have a reference count of at least one. Garbage collection finds such objects and frees them.
We also tested it because we suspect there is a bug in garbage collection, and we wanted to make
sure that it was not directly in the main implementation file. Since we didn't find anything
here, it's possible that the bug resides in one of the functions provided by the "client" code.
Since C doesn't have templating or any other means for polymorphism, objects that use GC are
required to provide their own functions for finding other objects they reference. If one
of those functions is incorrect, it's possible that a child of an object isn't being visited,
and therefore that the garbage collection implementation is not provided with a complete
list of "freeable" objects, and is not in itself doing anything wrong.

## Tets Performed
Note: Copying test source here is redundant. Please see the `tests/` folder in the
project repo which contains the source files for the tests.

### Garbage Collection
The garbage collection implementation in `abcs` is based on Python's
garbage collector, which keeps a linked list of "container" objects. These container objects are
then used to visit other objects. If a container object is visited as many times as there
are other containers, it means it's __only__ pointed to by other containers, and thus
can be freed. Otherwise, it's referenced from outside, and thus has to be kept alive (saved).

#### List Initialization
This unit tests that a "GC list" is correctly initialized. This usually
just means that by default, the GC list (the list of containers) is empty.

#### List Addition
This unit makes sure that elements are correctly added to the list. This is critical
because if a node isn't added to the garbage collection list, it's not known as
a container, and thus will not be cheked for circular references.

#### Garbage Collection (Basic)
This test makes sure the garbage collection algorithm works fine for objects that have no references
to other objects (despite being marked as containers). This tests the simple cases, and is akin
to the GC running on empty lists.

#### Garbage Collection (Robust)
This test makes sure the garbage collection algorithm actually takes care of reference cycles.
Two objects that reference each other are created, and pointed at each other. References to them
in the main code are then freed, and they are only remaining in the GC list. For this test
to pass, both objects must be freed after a `gc_run` (which is correct behavior since
they only point at each other, and are otherwise unreachable). We do not use an actual
delete / free call here (because it's impossible to test if a pointer has been freed),
and instead call a method on the dummy wrappers that flips an internal "freed" flag.

### Trie
Tries are special trees in which the path to a node from the root node holds the prefix of the 
strings held by the child node. The complete path to a leaf node contains the entire string
that was used as a key.

#### Trie Init
This tests that the trie has been initialized correctly. This means that the root node is NULL,
and that the internal empty list (returned when the path / key is not found) is also
actually empty, not uninitialized.

#### Trie Clear
This tests that the trie is cleared correcrtly. This function is called when scopes (implemented as tries)
are freed, which happens a lot (once every iteration of a loop, for instance). Thus, it's very important
that this function works as expected. When a trie is cleared, anything that was previously stored inside is released.

#### Trie Put + Get
This tests that the trie can correctly store and retrieve information, as well as the fact that
storing doesn't interfere with the internal "empty list". A value is stored and immediately requested
back, making sure that it was indeed correctly added.

### Reference Counting Pointer
The reference counting pointer is implemented almost the same
as a C++ `shared_ptr` - that is, it has a separately allocated block of memory which is pointed
to by all references to the same value. This block of memory contains the strong and weak refcounts,
and frees the data when the strong refcount becomes 0. It frees itself when the weak refcount reaches 0, too.

#### New Reference is Strong
A strong reference is a reference that keeps the data pointed to by the pointer allocated. References should
start strong, since when they are first created they are the only thing pointing to the data. Thus, if they are
weak, the strong count is immediately 0 and the data is freed. This test makes sure that a newly initialized
reference is strong.

#### New Reference isn't a Container
The additional memory block allocated by a `libab_ref` also includes the data needed for garbage collection.
However, this data is opt-in, and initially, a thing pointed to by a `libab_ref` should not be a GC object. 
This means the `visit_children`, `next`, and `prev` fields must be uninitialized. That is ensured by this test.
If this wasn't the case, non-GC objects could be somehow added to GC, causing, at worst, infinite loops.

#### New Reference Has Provided Free Function
A free function is used by the reference counting pointer to release the memory once the strong count reaches 0.
This test makes sure that the free function stored in a newly created reference is the same one that
was provided during initialization.

#### New Reference Has Provided Data
This test makes sure that a reference counting pointer that has been initialized points at the data that was provided
during initialization.

#### Reference Copies Correctly
This test makes sure that once a reference has been initialized with some data, and then copied (thereby increasing
the reference count), it does not free its value more than once. A pathologic case would be each individual
copy of the reference freeing the underlying data once, causing a double free error. This test checks the number
of times dummy data is freed by two references, one of which was initialized by copying the first.

#### Null Reference Copies
The null reference is a special case reference that supports all the functions of a usual reference but
doesn't point to a value or allocate additional memory. It is used in error handling contexts, since
a potential cause for an error is memory allocation failure, and thus it is not guaranteed that more memory
can be allocated. This test makes sure that a copy of the null reference is also a null reference.

#### Reference Swap
This test makes sure that reference swapping behavior (sometimes used for storing data into scopes)
works correctly, and that both variables pointed to originally are swapped.

#### Reference Get
This test makes sure that noy only is a reference point to a correct value, but it also returns
that value when `libab_ref_get` is called.

## Conclusion
We did not discover any bugs during the process. While it is true that we do not 100% understand
how the garbage collection system works, it's most likely that memory allocated during the
testing is freed correctly, but still belongs to the process, leading memory profiling software
to show high memory usage.
