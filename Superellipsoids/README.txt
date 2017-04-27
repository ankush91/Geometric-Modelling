************READ-ME***********

->*****NAME OF FILE AND LINE NUMBERS IMPLEMENTING SURFACE NORMAL CALCULATIONS******* - ***-- Source.cpp: 434-579--***

->->*****NAME OF FILE AND LINE NUMBERS IMPLEMENTING FLAT SHADE TRIANGLE INDEX CALCULATIONS******* --- Source.cpp:582 - 635***

-> Features of Code : The code is implemented as modular as possible and command-line arguments are set as parameters to "Parameters" Class.

superellipsoid is the function which implements the general superellipsoid function.

computeProd implements the equations for x, y and z coordinates.

->normal is the function to generate the normals

->flatshade is the function for the faces of the Triangle along the body and at the poles.

Additional Functions are present to print out the points to the console in .iv format. 

->Language of Choice and OS: The code is implemented in C++ on the tux (Linux) platform and would require the g++ compiler to compile.

->Name of File containing Main: Source.cpp is the name of the file containting main()

*****HOW-TO-RUN****
->cd into the directory of the folder containing Source.cpp and make
->Type in make to compile. This produces an executable of the format CG_hw4
->Type in ./CG_hw4 [command-line parameters][-u du -v dv -r s1_val -t s2_val -A Aval -B  Bval -C Cval [-S or -F]] to run the code
-> Follow the run-time command by > [filename].iv to redirect the output to [filename].iv format

******EXTRA-NOTES****
-> You can type make rm to remove executable the CG_hw4 and run again.
-> Please let me know if you face any issues

***********************THANK YOU*********************