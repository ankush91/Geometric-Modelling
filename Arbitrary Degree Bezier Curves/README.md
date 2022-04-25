### Name of File containing `main` 
* `Source.cpp`

### Details
* The code is implemented as modular as possible and command-line arguments are set as parameters to `Parameters` Class
* `bezier` is the function which implements the general bezier curve equation
* Additional functions print out the curve coordinates and control points to the console in `.iv` format
* The file `cpts_in.txt` contains some default point options. You could modify that file itself

### Language of Choice and OS
* The code is implemented in C++ on the tux (Linux) platform and would require the g++ compiler to compile.

### Instructions 
* `cd` into the directory of the folder containing `Source.cpp` and `make`
* Type in `make` to compile. This produces an executable of the format `CG_hw1`
* Type in `./CG_hw1 [command-line parameters][-f filename.txt -u du -r radius]` to run the code
* Folow the run-time command by `> [filename].iv` to redirect the output to `[filename].iv` format

### Notes
* You can type make `rm` to remove executable `CG_hw1` and run again
