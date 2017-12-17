#!/bin/bash
# TSC is a two-dimensional jump’n’run platform game.
# Copyright © 2017 The TSC Contributors
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

colpng=$(zenity --file-selection \
		--title "Collision PNG" \
		--text "Select the PNG with the collision rectangles" \
		--file-filter="*.png")

colspec=$(zenity --forms\
		 --title "Row entry" \
		 --text "Specify the tile rows and columns of your tileset" \
		 --add-entry=Rows \
		 --add-entry=Columns \
		 --separator=":")

./tscproc -M -c "$colpng" -d $colspec -x metadata.xml

exit 0
