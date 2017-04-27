************READ-ME***********

->*****NAME OF FILE AND LINE NUMBERS IMPLEMENTING SURFACE NORMAL CALCULATIONS******* - ***-- Source.cpp: 408 - 745 --***

-> Features of Code : The code is implemented as modular as possible and command-line arguments are set as parameters to "Parameters" Class.

bezier is the function which implements the general bi-cubic bezier patch equation.

computeSum implements the sum part of the equation.

->convertNorm1 & convertNorm2 are used to generate Tangents.

->normal is the function to generate the normals

->flatshade is the function for the general Triangle Indices. 

Additional Functions are present to print out the curve coordinates and Input points to the console in .iv format. 

->Language of Choice and OS: The code is implemented in C++ on the tux (Linux) platform and would require the g++ compiler to compile.

->Name of File containing Main: Source.cpp is the name of the file containting main()

*****HOW-TO-RUN****
->cd into the directory of the folder containing Source.cpp and type make
->Type in make to compile. This produces an executable of the format CG_hw3
->Type in ./CG_hw3 [command-line parameters][-f filename.txt -u num_u -v num_v -r radius [-S or -F]] to run the code
-> Follow the run-time command by > [filename].iv to redirect the output to [filename].iv format

******EXTRA-NOTES****
-> You can type make rm to remove executable the CG_hw3 and run again.
-> Default File patchPoints.txt contain some default points. 
->filename.txt consists of the other example given in the specification of the Assignment.
You could use/modify those example files itself and run.
-> Please let me know if you face any issues

***********************THANK YOU*********************