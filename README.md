The Secret Chronicles of Dr. M.
===============================

“The Secret Chronicles of Dr. M.” is a 2D sidecrolling platform game,
with a rich set of graphics, music, and an advanced level editor that
allows you to create your own levels. The level editor allows for
in-game scripting so there are no borders apart from your imagination.

The project takes much inspiration from
[SMC](http://www.secretmaryo.org), which it was originally a fork
of. This repository however contains an independant codebase that's
not based on the SMC code anymore.

TSC 3
-----

This repository contains the rewrite that will eventually become TSC
version 3.0.0. The current stable TSC 2 is available at
<https://github.com/Secretchronicles/TSC>.

Links
-----

* The TSC website: <https://secretchronicles.org>
* The mailinglists: <https://lists.secretchronicles.org/postorius/lists/>
* The forums: <https://lists.secretchronicles.org/hyperkitty/>
* The bugtracker: <https://github.com/Secretchronicles/TSC/issues>

Contributing
------------

Any contributions to the code, the graphics, the music, etc. are
greatly appreciated. However, before starting your work on the game,
please consider the following:

* You have to be familiar with at least the basics of using the Git
  version control system. This can be achieved by reading the first
  two chapters of the Git book available for free at
  <http://git-scm.com/book>. Also reading chapter 3 is highly
  recommended as we use branches all the time.
* If you want to contribute code, please read the coding conventions
  document in the directory docs/contributing.md.
* If you want to contribute artistic work, please read
  <https://wiki.secretchronicles.org/StyleGuide.html>
* If you specifically target issues from the issue tracker, please
  use “fixes #43” for bug tickets you fix and “closes #43” for other
  tickets you resolve as part of the message in your commits. This
  causes GitHub to automatically close the corresponding ticket if
  we merge your changes.

Custom local configurations are provided for Emacs and ViM. In order for local
ViM configurations to work, you will need the localvimrc plugin, which can be
installed with the following command on most Bourne-compatible shells:

~~~sh
mkdir -p ~/.vim/plugin
cd ~/.vim/plugin
wget https://raw.githubusercontent.com/embear/vim-localvimrc/master/plugin/localvimrc.vim
~~~

License
-------

TSC is a 2-dimensional platform game.

Copyright © 2017 The TSC Contributors

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
