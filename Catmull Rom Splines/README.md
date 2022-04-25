### Name of File containing `main`: 
* `Source.cpp`

### Details
* The code is implemented as modular as possible and command-line arguments are set as parameters to `Parameters` Class.
* `generateControlPoints` is the function to compute control points, firstPiece, lastPiece and concatenate are called from this function as well.
* `bezier` is the function which implements the general bezier curve equation.
* Additional Functions are present to print out the curve coordinates and Input points to the console in `.iv` format. 
* **PLEASE NOTE - DEFAULT VALUE FOR RADIUS IS SET TO 0.1 AS IN HW1, VALUE OF 1.0 AS MENTIONED IN SPECIFICATION OF HW2 IS TOO BIG FOR FREECAD.**
* The file `cpts_in.txt` contains some default point options as in example1 of the HW specification.
* [`filename1.txt` - `filename5.txt`] consist of all other examples (2-6) given in the specification of the Assignment. You could use/modify those example files itself and run.

### Language of Choice and OS
* The code is implemented in C++ on the tux (Linux) platform and would require the g++ compiler to compile.

### Instructions
* `cd` into the directory of the folder containing `Source.cpp` and `make`
* Type in make to compile. This produces an executable of the format `CG_hw2`
* Type in `./CG_hw2 [command-line parameters][-f filename.txt -u du -r radius]` to run the code
* Follow the run-time command by `> [filename].iv` to redirect the output to `[filename].iv` format

### Cleanup
* You can type make rm to remove executable the CG_hw1 and run again.
