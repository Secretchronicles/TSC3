# coding: utf-8
#
# TSC is a 2-dimensional platform game.
# Copyright © 2018 The TSC Contributors
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

require "rake"

desc "Reformat the code according to the TSC style conventions (requires astyle)."
task :format do
  sources = FileList["src/**/*.hpp", "src/**/*.cpp"]
  sh "astyle " +
     "--style=stroustrup " +
     "--indent=spaces=4 " +
     "--indent-namespaces " +
     "--pad-comma " +
     "--unpad-paren " +
     "--pad-header " +
     "--align-pointer=type " +
     "--break-closing-braces " +
     "--convert-tabs " +
     "--close-templates " +
     "--max-code-length=100 " +
     sources.to_s
end

desc "Update the copyright year to the current year in all files."
task :copyright do
  sources = FileList["src/**/*.hpp",
                     "src/**/*.cpp",
                     "cmake/modules/*.cmake",
                     "*.in",
                     "README.md",
                     "Rakefile",
                     "CMakeLists.txt"]
  sources.each do |file|
    print "Processing #{file}... "
    str = File.open(file, "r:UTF-8") { |f| f.read }
    str2 = str.sub(/Copyright © \d+ The TSC Contributors$/,
                   "Copyright © #{Time.now.year} The TSC Contributors")

    if str != str2
      File.open(file, "w") { |f| f.write(str2) }
      puts "Updated."
    else
      puts "No change."
    end
  end

end

desc "List those files that do not conform to TSC's license requirements."
task :licenses do
  require "rexml/document"
  require "rexml/xpath"
  require "find"

  Find.find("data") do |path|
    next if path !~ /.xml$/

    xml = REXML::Document.new(File.open(path, "r:UTF-8"))
    if node = REXML::XPath.first(xml, "/assetinfo/customlicense")
      printf "%-30s%s\n", "File", "License"
      printf "%-30s%s\n", path, node.text.strip
    end
  end
end
