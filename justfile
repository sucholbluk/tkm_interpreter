set quiet

build:
    cmake -DCMAKE_CXX_COMPILER=g++-13 -B build -S .
    cmake --build build

tkm_interpreter tkm_program="":
    if [ ! -f ./build/tkm_interpreter ]; then just build; fi
    ./build/tkm_interpreter {{tkm_program}}

# possible targets:
# - all
# - source_handler
# - position
# - lexer
# - parser
# - interpreter
test target="all": build
    cd build && if [ "{{target}}" = "all" ]; then ctest; else ctest -R "{{target}}_tests"; fi

gen_coverage: clean
    cmake -DCMAKE_CXX_COMPILER=g++-13 -DCOVERAGE=ON -B build -S .
    cmake --build build
    cd build && ctest
    lcov --gcov-tool gcov-13 --capture --directory build --output-file coverage.info
    lcov --remove coverage.info '/usr/*' '*/include/*' '*/tests/*' '*/src/core/*' '*/src/exceptions/*' --output-file coverage_filtered.info
    genhtml coverage_filtered.info --output-directory out
    just disp_coverage

disp_coverage:
    if [ ! -f ./coverage_filtered.info ]; then just coverage; fi
    xdg-open out/index.html

clean:
    rm -rf build coverage.info coverage_filtered.info out doxydoc

docgen:
    doxygen Doxyfile
    just open_doc

open_doc:
    xdg-open doxydoc/html/index.html
