
### Name of File containing `main`
* `Source.cpp`

### Name of File with line numbers implementing `SURFACE NORMAL`
* `Source.cpp`: 434-579

### Name of File with line numbers implementing `FLAT SHADE TRIANGLE INDEX`
* `Source.cpp`: 582-635

### Details
* The code is implemented as modular as possible and command-line arguments are set as parameters to `Parameters` Class
* `superellipsoid` is the function which implements the general superellipsoid function
* `computeProd` implements the equations for x, y and z coordinates
* `normal` is the function to generate the normals
* `flatshade` is the function for the faces of the Triangle along the body and at the poles
* Additional Functions are present to print out points to the console in `.iv` format

### Language of Choice and OS 
* The code is implemented in C++ on the tux (Linux) platform and would require the g++ compiler to compile

### Instructions
* `cd` into the directory of the folder containing `Source.cpp` and `make`
* Type in `make` to compile. This produces an executable of the format `CG_hw4`
* Type in `./CG_hw4` [command-line parameters][-u du -v dv -r s1_val -t s2_val -A Aval -B  Bval -C Cval [-S or -F]] to run the code
* Follow the run-time command by `> [filename].iv` to redirect the output to `[filename].iv` format

### Cleanup
* You can type make rm to remove executable the CG_hw4 and run again
