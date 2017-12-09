require "rake"

desc "Reformat the according to the TSC style conventions (requires astyle)."
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
