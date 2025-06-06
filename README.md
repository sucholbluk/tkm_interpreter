- author: Łukasz Suchołbiak

### General
The project's goal consisted of 2 main parts:
1. Design a programming language that has:
	- strong and static typing
	- variables constant by default
	- variables passed to functions by reference
	- higher order functions support
		- and at least 2 higher order functions operators (chosen bind front and function composition)
2. Implementation of an interpreter for this language

*The language specification is available only in Polish at `./docs/TKOM-dokumentacja-końcowa.md`*
#### Tools used in this project:
- **C++20** standard
	- with libraries:
		- Boost (v1.88
		- or newer; required: program options and test)
		- spdlog
		- fmt
- **clang-format** - for file formatting (configuration in file `.clang-format`)
- **cmake** - for building/linking the project and tests
- **doxygen** - for generating documentation from code
- **lcov** - for generating coverage information
- **just** - for easier and more convenient project automation and task management (build, testing, running, docs generation, coverage info generation).

#### Building the project
In the repository's root run
```
just build
```
And the build is available in `build/`

#### Running the project
There are 2 options:
1. Using `just`:
```
just tkm_interpreter [filename]
# or with options
just tkm_interpreter "-v [filename]" 
```
eg: `just tkm_interpreter "-v ./example_programs/counter.tkm"`
2. Manually from the `build/` directory:
```
tkm_interpreter [filename] [options]
```
##### Available options
```
usage: ./tkm_interpreter [file] [options]
available options::
  -h [ --help ]         display help info
  -s [ --stdin ]        read data from standard input
  -v [ --verbose ]      enable verbosity
  --input arg           input filename
```
Help is the default option
#### Testing
To run the tests:
```
just test [target]
```
Possible targets are:
- all - (default) - runs all tests
- source_handler
- position
- lexer
- parser
- interpreter

#### Coverage information
To generate coverage information run:
```
just gen_coverage
```
Automatically after generating the coverage information result with open in browse

If coverage information is already present - to open it in browser run:
```
just disp_coverage
```
*But if it doesnt exist it will generate it*

#### Documentation from code
To generate run:
```
just docgen
```
This will generate the documentation from code `doxydoc` directory. After successful doc generation it will open in browser.

To just display exsistant docs in browser run:
```
just open_doc
```

#### Clean-up
To cleanup project run:
```
just clean
```
This will remove: `build` directory, `out` directory (with generated coverage), `coverage.info`, `coverage_filtered.info`, `doxydoc` directory


