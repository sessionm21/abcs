# Final Project

Due March 18, 2019 at 11:59 PM

Team Name: nullptr

Team Members: Ryan Alder, Danila Fedorin, Matthew Sessions

Github Project Fork URL: [https://github.com/sessionm21/abcs](https://github.com/sessionm21/abcs)

## Initial Quality of the Project

### User Design
It's fairly clear this program isn't written for the everyday user. It provides few tips on how to get started,
and has a fairly high learning curve (including a new syntax). While this can be forgivable because of abcs'
"for power users" origin, we've found other issues with usability that aren't justifiable this way. The main
complaint against the software is the equality of the error reporting - or lack thereof. All errors
are reported to the user in the exact same way - by printing out the phrase "invalid expression". This blanket
phrase covers syntax errors, type errors, and missing variable names. Essentially, it's impossible to effectively
diagnose issues with user input without thoroughly checking every part of said input. Since abcs supports fairly
complex constructs, such as control flow and recursion, this naturally leads to potentially big programs.
Debugging such potentially big programs takes a very long time, as even finding a typo in a single line of
code is sometimes a challenge. Otherwise, besides the fact that the program manual was not located in the root
directory of the project, but rather in a subdirectory, there are no other significant usability issues.
The project is usable, assuming an understanding of programming, which is expected from the project's target audience.

### Code Maintainability
The code for the project is very maintainable. It follows a style guide (and includes a specification file which
allows for the automatic formatting of source code). All public functions have doc comments in the headers,
explaining the purpose of the function, the inputs, and the outputs. The same is true for some internal
functions, especially those used in the type checking algorithm. Global variables are not used at
all in the library portion of the project, and only used once in the front end (to keep a reference to
the calculator state). This means debugging involves simply looking at the inputs and outputs of functions,
rather than keeping track of possible global state. The only issue with the project before our
intervention was the complete lack of tests. None of the code in the project was tested, which means
that there's no safety net when changing the various functions of the code during maintenance. Bugs
can be introduced without notice, and the only way to test for such bugs is to manually run through
a variety of cleverly designed test cases (which didn't exist before our team's involvement with the project).

### General Release-Readiness
We belive the project is largely release-ready. Despite several small bugs (numbers counting as false in loop conditions,
and an elusive segmentation fault during type unification), the project operates as intended, and for the average
user will not present any issues. We would recommend corrective maintenance to fix the loop condition bugs and
to investigate the source of the segmentation fault. We also believe that the unique error messages should be
added to improve the learnability and efficiency of using the project.

## Methods and Tools

### Version Control
The project is managed using the git VCS. It uses git to a fairly deep level: the project is split
into several parts (abcs, the front end, libabacus, the library, libds, the data structures code, and
liblex, a NFA-based tokenizer). Each of these parts is added as its own git submodule, and lives in its
own repository. This is because each of the components can be used separately from the others - 
libds and liblex especially, since they are general purpose C libraries. libabacus is
more niche, but because of its ability to integrate with different front ends, it also
lives separately from abcs, the only front end currently created. In order to patch bugs,
we created pull requests with our new features, and performed code review on the new
feature branch. This ensured that the changes merged into the master branch are at the very
least sane.

### Testing Tools
One of the tools we used was a C library called `greatest`. This library provides a variety of macros
and assertions to help with creating unit tests. This saved us the work of writing test result output
code and other plumbing which would otherwise be necessary in C. For some of the more
complex tests, especially those that tested the deallocation of memory, we used valgrind. This
allowed us to make sure that memory was freed as intended after the program finished execution.
This was especially helpful when testing garbage collection, the whole point of which is to test that
memory no longer in use is deallocated. Finally, we used gcov to determine the coverage of our
unit tests.

### Debuggers
The GNU Debugger (GDB) is the main tool that we used. However, we were unable to reliably reproduce
the only issue that would require a debugger - the segmentation fault we discovered when writing
an identity function for a variable during manual testing was no longer present
when we attempted to perform bug fixes. As such, no use of GDB was necessary.

### Libraries
As discussed previously, we used the greatest C library to perform unit / automated testing.
We did not use any other libraries for testing.

### Environments
We tested the project in several different environments. The tests were run in full
on each of the three team members' computers. These computers had various configurations
(one used an ARM CPU, the other two had x86\_64 CPUs. Two computers ran the Linux kernel,
one ran Apple's modified BSD kernel). This was sufficient for the most part,
since the tests did not rely on any platform specific behavior (and nor did the project),
which would mean that even one computer on which tests pass should be good enough. However,
as a means of sanity checking ourselves, we also had the automated tests run on the Travis CI servers
after every commit that was pushed to GitHub. Our manual and automated tests yield more or
less the same results on platform, minus a single difference which is caused by a different
stack depth on two of the machines (one machine requires a less deep recursion to cause
a stack overflow).

## Testing

### Code and Features Tested
We tested a variety of calculator features during manual and usability testing.
Usability testing focused on error reporting and recovery (which is sprinkled throughout the project),
while manual testing had to serve as a "wide net" to catch bugs. This is because
abcs is an interpreted programming language, and any input to the program ends
up taking the full path through the program. This means that any input
we gave went through the parser, the interpreter, and the garbage collector.
As such, it was difficult to isolate units, so we focused on tests
that cover a major component of functionality that may depend on more
than one component of the application.

During manual testing, we focused on the underlying primitives in the program.
This meant the reference counting pointers, tries, and garbage collection.
Reference counting pointers were used throughout to manage memory -
there were practically no raw pointers for anything but the most simple data.
Tries are a data structure used to map strings to data, and the abacus
code used them to associative variable and function names with their 
implementations / values. Finally, garbage collection was an important
foundation for memory management which we suspected had a bug.

We tested garbage collection, parsing (including arithmetic expressions,
functions, and if/else statements), as well as interpretation (including
first-class functions, function and operator evaluation, and loops).
These tests covered the following areas:
* if/else/while statements parsing
* Math parsing (order of operations, associativity, parentheses)
* Function declaration parsing
* Function calling (partial function application, type substitution, generics)
* First class functions (functions as values, closures)
* Garbage collection (via functions deliberately engineered to cause heavy memory allocation)

### Tests Performed
#### Usability Tests
For usability testing, we had the user run through the following set of tasks:

* Start the program
* Add one number to another number
* Call a trigonometric function (sin)
* Change precision to be 20 digits
* Implement a recursive factorial function (without the built-in factorial function)
* Quit the program without using Ctrl-C

This was done twice at different times, so that we could assess the learnability
of the program.

#### Manual Tests
For manual testing, we performed a variety of tests, briefly summarized here:
Input:

`3+3`

Explanation: This is the basic test for arithmetic.
Input:

`fun plus(x: num, y: num): num { x+y } 3+3`

Explanation: This test makes sure that addition calls the plus function.

Input:

`fun plus(x: num): num { x+1 } 3+3`

Explanation: This test makes sure that addition uses the binary plus function only.

Input:

`fun plus(x: num, y: num, z: num): num { x+y+z } 3+3`

Explanation: This test makes sure that ternary plus isn't used by addition.

Input:

`1--2`

Explanation: This test makes sure that unary prefix operators work, and can
be disambiguated from binary operators with the same name.

Input:

`3!`

Explanation: This test makes sure that unary postfix operators work.

Input:

`3+!4`

Explanation: This test makes sure that postfix operators are parsed correctly.

Input:

`6-3*2+3^2^3+2`

Explanation: This test makes sure that associativity and order of operations are correct.

Input:

`(6-3)*2`

Explanation: This test makes sure that parentheses work as intended.

Input:

`(5-3`

Explanation: This test makes sure that unbalanced parentheses cause errors.

Input:

`5-3)`

Explanation: This test also makes sure unbalanced parentheses cause errors.

Input:

`fun test(x: 'T, y: 'T): 'T { x } test(3, 3)`

Explanation: This tests generic type substitution with numbers. 

Input:

`fun test(x: 'T, y: 'T): 'T { x } test(3, true)`

Explanation: This test makes sure generics don't allow incorrectly typed parameters.

Input:

`fun one_or_another(x: 'T, y: 'T): (bool)->'T { fun test(b: bool): 'T { if(b) x else y } }`

Explanation: This is a prerequisite for other tests, used once again to check type parameter substitution.

Input:

`fun test(x: 'T, y: 'T): 'T { x }`

Explanation: This is a prerequisite for other tests as well. This is just the equivalent of the Haskell "const" function.

Input:

`first = one_or_another(1, 2);`

Explanation: This makes sure that generic functions are correctly returned and type substitution is performed as expected.

Input:

`fun test(a: num): num { a } test = fun weird(a: num): num { -a }`

Explanation: This makes sure that assigning a function to a variable overrides overloading.

Input:

`plus(3)(7)`

Explanation: This makes sure that partial function application works as expected, returning a partially applied function after the first call.

Input:

`fun do_something(f: (num)->num, x: num): num { f(x) }; a = plus(3); do_something(a, 7)`

Explanation: This test makes sure that functions, including those that are partially applied, can be passed as values.

Input:

`x = 0; fun increment_x(): unit { x = x + 1; }; increment_x(); increment_x();`

Explanation: This test makes sure that functions capture the scope in which they're declared.

Input:

`fun adder(start: num): () -> num { fun add(): num { prev = start; start = start + 1; prev } }`

Explanation: This test makes sure that functions keep a reference to the scope in which they're declared even when
it's not reachable by other means.

Input:

`a = adder(4); a(); a(); a()`

Explanation: This is a continuation of the previos test (the closure is called several times)

Input:

`fun waste_space(x: num): (num)->num { ...omitted due to length... }; waste_space(5000)`

Explanation: This is a test specifically designed to stress the garbage collector by allocating
 a lot of memory via recursively created interdependent closures.

Input:

`if(true) 3 else 4`

Explanation: This is a test that makes sure an if-expression evaluates the first branch if the condition is true.

Input:

`if(false) 3 else 4`

Explanation: This is a test that makes sure an if-expression evaluates the second branch if the condition is true.

Input:

`if 3 else 4`

Explanation: This is a test that makes sure parsing fails if an if-expression lacks a conditional.

Input:

`if () 3 else 4`

Explanation: This is a test that makes sure an if-expression with an empty condition doesn't parse.

Input:

`while(x < 5) { x = x + 1 }`

Explanation: This is a test that makes sure the while loop works as expected.

Input:

`while { x = x + 1}`

Explanation: This is a test that makes sure that a while expression with no condition doesn't parse.

Input:

`while (3) { x = x + 1}`

Explanation: This is a test that makes sure that a while expression with a non-boolean condition causes an error.

Input:

`a = 1; a`

Explanation: This is a test of variable assignment.

Input:

`a = 1; a = 2; a`

Explanation: This is a test of variable overwriting.

Input:

`{ a = 1 }; a`

Explanation: this is a test of variables going out of scope.

#### Automated Tests
We wrote the following automated tests:

| _Test_ | _Explanation_ |
|--------|---------------|
|`ref_new_is_strong`| New reference counter pointers keep refcount up.|
|`ref_new_no_gc`| New references don't have garbage collection enabled.|
|`ref_new_free_func`| New references have the correct free function.|
|`ref_new_not_null`| Newly allocated references aren't null.|
|`ref_copy_value`| Copying works and increments reference count.|
|`ref_copy_null`| Copying null creates another null reference.|
|`ref_swap`| Swap works without changing refcounts.|
|`ref_get`| References return the correct value when dereferenced.|
|`gc_init`| Garbage collection correctly initializes an empty list.|
|`gc_add`| Adding an item to garbage collection correctly configures it.|
|`gc_run_basic`| Containers with no cycles are freed as expected.|
|`gc_run_robust`| Containers with references cycles are freed as expected.|
|`trie_init`|A trie is initialized correctly.|
|`trie_clear`| A trie is cleared correctly.|
|`trie_put_get`| Storing and retrieving from the trie work as expected.|

### Issues Found
During usability testing, we found only one very significant issue - the error reporting. As we
briefly discussed above, the errors reported were limited to "Invalid expression", which
did not in any way indicate the location of the error. It was, in fact, so difficult to
identify the error in most cases that the users tended towards assuming that
they had encountered a bug in the software. This was not the case - the calculator
was reporting real issues - but to figure this out, it took as many as 10 minutes
per expression to determine the location of the mistake. We found that this
is because the errors are only reported in the main loop, at which point
the information about the error is limited to the error code (which is not even used).

During manual testing, we found several small issues, and a couple of big ones.
The small issues were that if/else statements, as well as loops, did not create
errors when their conditions did not evaluate to boolean values. Instead, they
simply treated these non-boolean values as false. Intuitively, this makes no sense.
The more significant issue we found was a segmentation fault in the type unification code.
This occured when a function took a generic parameter (similar to templates in C++).
With certain type checking cases, it was possible to cause a type substitution
failure. We also found that creating a type-generic identity function caused
a segmentation fault. Lastly, we found that while running input
specifically engineered to create unused memory, the memory usage
of the calculator went up, but never went back down. This indicated that the 
garbage collection was malfunctioning, which was also a very serious issue.

We found no issues during code review and automated testing. Though this
doesn't seem very interesting, it is: we applied automatic testing
to garbage collection, which we have determined during manual testing
to contain an issue. This forced us to reconsider the cause of the high memory
usage. Likely, this is because the garbage collection runs after the command is
finished executing, which means that by that time, system calls have been made
to request memory from the operating system. Thus, even though the memory was
freed, it still belonged to the process, leading the monitoring software
to show high memroy usage. We also were unable to replicate the segmentation
fault that we previously found during manual testing. In fact,
we were unable to replicate it after re-running the manual testing cases that
were previously guaranteed to cause an error. This ruled out the two
significant bugs that we found, and left us only with the usability issues
and the strange loop condition behavior.

## Patching

### Triage Process
With all the significant issues being impossible to reproduce (not for a lack of trying),
we were left with only two categories of issues - the error reporint and the non-boolean
control flow conditions. Since error reporting is a very intensive task (and a high priority
for usability), we assigned two of the three team members to it. One of the team members would
patch the parser.c source file to include the error reporting during text processing, while
the other team member would patch the interpreter to do the same. The last issue (non-boolean conditions)
is a low priority bug because it doesn't limit correct behavior: the valid programs in the language
with the bug are a strict superset of the valid programs without the bug. As such, unless users
rely on this invalid behavior, they will not experience issues. This issue was assigned to one
team member, since it was both less significant and less work intensive.

### Issues Resolved

#### Danila Fedorin: If/Else/While non-boolean weirdness
As explained above, the calculator did not report an error when a loop was executed
that had a non-boolean value as a condition. This value was simply interpreted as false.
When initially investigating this issue, I simply ran the "reference" front end provided
with libababcus, a program called "interactive". This program uses doubles instead
of arbitrary precision floating point numbers, and doesn't support anything besides
the most minimal set of mathematical functions. This program, however, did not suffer
from the same defect as abcs - it didn't allow numbers or other values in conditions,
and immediately reported an error. This meant that the issue lay on the boundary between
the front end and the library. It turns out that the front end actually doesn't use the
error codes as intended - it ignores them. Rather, it checks that the returned value
from evaluating an expression is null or not. By the convention of the library, a function
that can fail initializes its outputs to NULL. Thus, this was sufficient. However,
it turned out that in the while/do-while loops, while the error code is
set correctly, the result of evaluating the loop is not freed / set to NULL.
Since abcs ignored error codes, it was not aware that an error occured. This
was fixed by adding the missing freeing and clearing code after the loop.

#### Matthew Sessions: Descriptive Error Messages in interpreter.c
Given that any error in a line of the script would print the same error message, and only one error message.
More descriptive error messages need to printed such that an error would be easier to identify. A string 
was added to the libabacus struct for storing this error message, since this struct should be available
throughout functions in the interpreter.c file. Simply reallocating the size of this string and appending
the current error to the full error message was enough. This error message is then printed in the main
shell portion of the program should the interpreter fail.

#### Ryan Alder: Improved Error Messages in parser.c
As previously mentioned, one of the largest issues that we found with abcs is the lack of informative error messages. For every single error the user was greeted with "Invalid Expression", regardless of what the error was. Given that, I set out to add more informative error statements in the parser. Error strings are set to the main struct, and accessed inside parser as the program encounters errors. Based on where the error is found, a unique error statement is set and then printed outside in main.cpp once it realizes the error variable has been set. A full set of potential errors has now been accounted for and manually tested for any errors the parser encounters.

### Maintenance Process Types Performed
We did not perform adaptive, or preventive maintenance.
We did not perform adaptive maintenance becuase the outside requirements for the software have not changed, and thus
the program still fits the criteria for which it was developed. Preventive maintenance was not needed as we
did not find any issues (during code review or otherwise) that could potentially
lead to problems later on.

We did perform perfective maintenance. The thing we were perfecting was the error reporting - which
was practically nonexistent. By improving error reporting, we are able to improve the quality
of the interaction between the user and the software, thereby increasing efficiency and learnability.

We also performed corrective maintenance. We corrected the strange behavior in if/else/while expressions,
as described several times above. After our patches, non-boolean values actually created an error
was would be expected from the software, instead of evaluating to false. 

### Regression Testing
We did regression testing by running through all of the manual and automatic tests that we have previously
written, as well as by doing code reviews on the patches submitted by various team members. The code
review allowed us to discover any potentially incorrect modifications to the code before they were
merged into the source tree, saving the effort of rerunning tests after fixing the bugs. The
automated and manual tests had previously been formulated with the goals of broadly testing the
software, as well as testing its foundations. As such, they cover most of the usual use cases, and
therefore should serve as a decent metric of how well the changes we made work. Regression testing
did not find any issues in the patches.

## Evaluation

### Unresolved Issues
Although we are unable to reproduce it, the segmentation fault in type unification is likely
very real and still present in the code. We will be on the lookout for inputs that cause
any errors regarding type unification, minimizing which will likely lead us back to the segmentation
fault. However, at present, despite several hours of additional testing, we were unable to once
again reproduce the crash we experienced during manual testing.

### Recommendations
We recommend that more automated tests are written for the project. Our team simply did not have
enough man hours to completely test the entire 4000+ line piece of software. Especially given the
fact that a segmentation fault potentially lurks somewhere in the source code, it should be high
priority to create automated tests for type unification, which takes up a significant portion of
the interpreter.

### Testing Comparison
For this project, it turned out that manual testing was more effective at finding bugs. 
This was likely because we did not have the manpower to write a sufficiently sophisticated
set of test. Automated testing is easier to set up if the project begins with it in mind
(using TDD or otherwise), and if tests are written alongside code. On the other hand,
it is very difficult to write tests for a 4000+ line project without any formal specification
of what each function / unit is supposed to do.

Manual testing, on the other hand, tests the product through the eyes of the user. It's
therefore much easier to use things like user stories ("as a user, I want to be able to add
numbers by writing + between them") to create test cases. These test cases cover a broad
section of the code, but are very helpful in finding behavior that deviates from the
users' expectations.

It wouldn't be fair to compare usability testing to the above two techniques. While it did
not discover faults in the code, it did unearth issues with the interaction between the
user and abcs. Since abcs' primary goal is to interact with users (it's a utility, after all),
its usbility is incredibly important. As such, we believe that usability testing was a key
component of our investigation into the software.

Code review did not help our team very much. This was likely because most of the team members were completely
unfamilliar with the code of the application, and even the domain of the software (programming language interpreters).
As such, their feedback was frequently limited to stylistic remarks (formatting, variable name, etc). While these
remarks help improve the maintainability of the code, they do not necessarily help with locating bugs. abcs is a
complex system, and it requires a significant amount of state to be kept in the reviewers' heads in order
to uncover actual bugs. Because the program code and state spans thousands of lines of code and several files,
discovering real, complex bugs with code review is very difficult. This can be remedied by spending more
time on the project, however, this is simply not realistic for our team of 3 full time college students.
