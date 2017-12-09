Contributing
============

This file documents the most important facts about the development
process of The Secret Chronicles of Dr. M. (TSC). Be sure to read it
carefully if you plan to contribute to this project, and then follow
the conventions outlined here when you submit your contribution.

* \tableofcontents
{:toc}

Formatting Rules
----------------

This section describes the general style used in TSC's codebase. It's
loosely related to the [Stroustrup style][1], but doesn't exactly
match it. If this section leaves any questions open, you should refer
to the Stroustrup style.

Note that for Emacs an appropriate style file is already provided in
the source tree, `tsc/.dir_locals.el`. That file will automatically be
picked up by Emacs when you edit the sourcecode, so you don’t have to
worry about changing the indentation and other style settings
manually. Stil, a style file doesn’t lift the burden of properly
formatting the code, it is just a help.

Code in TSC is indented with 4 spaces, no tabs. Most code editors can
be configured to automatically insert the required number of spaces
when you hit the TAB key.

~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
while (true) {
    cool_code();
}
~~~~~~~~~~~~~~~~~~~~~~~~~~

Try to keep line lengths around at maximum 80 characters (typical
terminal width).

It is recommended you run `$ rake format` when you're done with your
code changes. This invokes the `astyle(1)` command with a set of
options that will format your code according to the TSC project's
general formatting conventions.

Naming Conventions
------------------

### File names ###

Sourcecode files end in `.cpp`, header files end in `.hpp` in case of
C++ code (which should be everything in TSC). Use `.c` and `.h` if for
some reason pure C code is required.

### Class Names ###

Use CamelCase without underscores for class names. For types that
aren't classes, use lowercase_snake_case.

~~~~~~~~~~~~~~~~~~~~{.cpp}
class FooBar {
};
~~~~~~~~~~~~~~~~~~~~

Nesting classes is OK for indicating a very strong relationship
between two classes.

~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
class Path {

    class Segment {
    };

};
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Do not repeat the outer class name in the inner class’ name, it’s
obvious from the namespacing (`Path::PathSegment` is **wrong**,
`Path::Segment` is correct). Do not nest further than one level
unless absolutely required.

### Function Names ###

For methods (i.e., functions that are part of a class), use
CamelCase(). For class-independent methods (i.e. utility methods), use
lower_snake_case.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
class Foo {
public:
    void CoolMethod();
};

void utility_method();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Variables and Parameters ###

Variable and parameter names use an abbreviated hungarian
notation.

* For local throw-away variables just use a descriptive name,
  don’t apply any prefixes.
* Member variables are prefixed with "m_"
* Static member variables are prefixed with "s_"
* Global variables are prefixed with "g_"

In any case, a variable for a pointer should always contain
"p", references an "r". Prefixes are combinable. Examples:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
int count;          // local variable
int m_count;        // member variable
static int s_count; // static member variable
extern int g_count; // global variable
int& r_count;       // local-var reference
int& mr_count;      // member-var reference
int* p_count;       // local-var pointer
int* mp_count;      // member-var pointer
int* gp_count;      // global-var pointer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Macros ###

A macro, if really required (see below), should be ALL_IN_CAPS so it
stands out of the code and warns everyone it is a macro.

Code organisation
-----------------

### Variable access ###

Usually member variables should be either `private` or `protected`,
and getter and setter methods should be defined to access these
variables. This way, you can be sure in the implementation of your
class that nobody messes with the variables from the outside in an
unexpected way.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
class Foo {
public:
    int GetBar();
    void SetBar(int x);

private:
    int m_bar;
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Additional method calls have performance overhead. Most getters and
setters are trivial, so you want to squash the performance overhead by
taking advantage of C++’s `inline` keyword that retains the secrecy
principle and still allows the compiler to optimize the method call
away:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
class cFoo {
public:
    inline int GetBar(){ return m_bar; }
    inline void SetBar(int x){ m_bar = x; }

private:
    int m_bar;
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Only do this for small one-line accessor methods, otherwise
you abuse the `inline` keyword.

### Macros ###

Try to prevent macro use as much as possible. C++ has templates that
will cover a good number of macro usecases, but sometimes you just
can’t avoid a macro (such as the one defining the event handler
callback for mruby (a C library!) functions; doing that without the
`MRUBY_IMPLEMENT_EVENT()` macro would be much more work and much more
unclear than this way).

The header guards that prevent `#include` from including a header file
multiple times are of course an exception. Header guard macros in TSC
should always look like this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
#ifndef TSC_NAMESPACE_CLASS_HPP
#define TSC_NAMESPACE_CLASS_HPP
// Code...
#endif
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Do not indent `#ifdef` and `#if defined` conditions. They are not part
of the code and must quickly be found from just glancing over the file
when you want to know whether you are in a Windows- or Linux-specific
part of the code. Do not nest preprocessor conditional statements
unless absolutely required.

Implementing code that needs to differenciate between multiple
different operating systems (or other environment-related macros)
should look like this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
void foo()
{
#if defined(_WIN32)
    windows_specific_stuff();
    with_another_line();
#elsif defined(__linux)
    linux_specific_stuff();
#else
#error unsupported platform
#endif
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Be sure to always add an `#else` part containing an `#error` line so
that users that want to compile on an unhandled platform receive a
compilation error rather than a dysfunctional program.

### Headers, #include Statements, forward declarations ###

Headers should be feature-complete. This means that if someone does a
`#include "yourheader.hpp"` the header should include (recursively)
all other headers that are required to use this header, but no
more. Especially the header should not include header files that are
only required in the corresponding implementation `.cpp` file (this
speeds up compilation time).

Use pointers and references as much as you reasonably can in headers,
and then use forward declarations in your header rather than including
other headers of TSC. This again helps to speed up compilation time.
Do not do that with standard library classes, though -- for those
always include the STL header.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
#ifndef TSC_MY_HEADER_HPP
#define TSC_MY_HEADER_HPP
#include <string>

namespace TSC {
    // forward-declare
    class ComplexThing;

    class MyFoo {
     public:
        // ...
     private:
        string m_ident;
        ComplexThing& mr_complex_thing; // << Note reference!
    };
}

#endif
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note that it's perfectly possible to forward-declare `enum`
statements.

### Namespaces ###

All TSC code is required to be defined under the `TSC` namespace. In
header files, always write out `namespace TSC {` in full and indent
accordingly. All other namespaces, including the `std` namespace,
should be spelled out in full in header files.

In your `.cpp` implementation files however always include:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
using namespace TSC;
using namespace std;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

near the top so you don’t have to indent all your implementation
code and have easy access to the C++ STL.

### License header ###

Each file in TSC is required to start with the following license
header:

~~~~~~~~~~~~~~~~~~~~{.cpp}
/*******************************************************************************
 * This file is part of TSC.
 *
 * TSC is a 2-dimensional platform game.
 * Copyright © <YEAR> The TSC Contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 ******************************************************************************/
~~~~~~~~~~~~~~~~~~~~

`<YEAR>` should be replaced with the current year.

### Comments ###

Comments should be added where you have to say things that you can’t
express in code. Comments that repeat what the code is saying already
are bad, especially if they get out of sync with the code itself.

Single and two-line comments use `//`. For comments with three and
more lines, use `/* */`.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
// Short comment

/* Long comment that explains a more complicated factum that
 * definitely needs more than three lines, because the explanation
 * is so insanely complicated that each less line would prevent
 * the understanding.
 */
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Comments should not exceed a line-length of 80 characters, try to
break at column 70. Use proper English grammar and punctuation. Short
oneline comments may ommit the final period. If you use British
English or American English, or whatever dialect you are familiar
with, does not matter. Since TSC is an international project, you
should avoid very local slang, though.

Documentation comments should be attached to complex methods and
classes. Although at this point TSC's codebase is not processed by
[Doxygen](http://www.stack.nl/~dimitri/doxygen/), you should use it's
commenting style when you write a documentation comment. This allows
us to opt-in into Doxygen at a later point in time.

Commits
-------

Commit messages should be descriptive and summarise what you did. If
the change is nontrivial, some reasoning should be given as to why you
solved a certain problem in the way you did, if it isn't obvious from
the source code (including comments).

The commit metadata should be valid, i.e. each commit should be
attributed to the person who really authored the content. While this
seems obvious, it isn’t that easy when you cope with someone who
doesn’t know how to use git, e.g. artists. In this case, you have to
ensure the commit is really attributed correctly to the artist, and
not to you. You do this by commiting the content in question yourself,
but use the `--author` option of the `git commit` command set to the
name and email address of the real contributor.

Signoff
-------

There are times when you want to commit something into the main
development branch from which you don’t really know whether it’s
legally acceptable. Take for example a graphic that has
been sent to the mailinglist with the goal to get it included into the
game. Due to timing constraints, nobody answers the contribution, and
when three days later someone comes around and reviews it and finds it
good, the author has disappeared and is not catchable anymore. The
graphic is awesome, but the contribution email did not include any
license information. Now, if you decide to add it into one of
the main branches, it will fall back on you in case of possible legal
problems following. You probably don’t want that. So, what do do?

The hierarchy in open-source projects is usually very flat. Some
structures, though, are inevitable. One of them is the project
leadership, which includes the burden to finally decide on all
problems that cannot be answered by the team in a collective mannor
for whatever reason. Unless a lawyer joins the team, this also
includes the final decision on legal questions regarding
contributions. Thus, the answer to the above question is to have the
project lead or the project assistant lead “signoff” your commit. With
the signoff, **the person who does the signoff certifies that to the
best of his knowledge the contribution is legally OK**.

The submission procedure for a signoff request is as follows. Extract
the legally fragile content in such a way you can add it in as a
single commit. If it’s a code patch in Git format, that has already
been done for you. If it isn’t, you have to construct a patch in Git
format by first including and commiting the contribution as usual (if
required, under use of the `--author` option as described above). Then
create the Git patch:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
$ git format-patch HEAD~1
$ git reset --hard HEAD~1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This creates a `.patch` file in the current working directory and then
immediately removes the commit from the Git history so you don’t
accidentally push it onto the remote. Send the patch file to one of
the persons mentioned above by email or post it on the tracker.

This is all you have to do; the project lead/assistant lead will keep
track of the rest. For reference, what he’ll be doing is the
following: First, he will carefully check if the contribution is
legally safe. If he comes to the result it isn’t, it will not get
in. Period. If he is sure all is well, then he will add the patch back
in using the `git am` command. Then he will do the classic signoff
command:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
$ git commit --amend --gpg-sign=THEGPGKEYID --signoff
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This keeps the `author` metadata information and additionally adds
`signoff` metadata information. The `signoff` information will then be
set to the person who did the signoff (i.e. the project lead/assistant
lead). Afterwards, an ordinary `git push` will follow to add the
signed-off commit to the remote repository.

The signoff procedure has the purpose of handing the bloat load of the
legal question off to someone different so you don’t have to deal with
details you don’t know how to deal with. It’s *not* a way to get any
weirdly licensed code into the project codebase; the maintainer doing
the signoff still has to check the legal circumstances and may well
reject the contribution if he isn’t certain about permission. The
maintainer doing the signoff also will not necessarily check the
contribution semantically; whether it solves an important problem or
has a good code quality is not checked. The signoff procedure solely
focuses on the legal aspect; it is on you as a developer to check the
contribution matches the conventions layed down in this document. If
you want to initiate a technical peer-review of the contribution, file
a usual pull request again the target branch.

[1]: http://www.stroustrup.com/Programming/PPP-style-rev3.pdf
