## GetOpt++
### C++ Library for command-line argument parsing
#### Current version: 0.1.0

GetOpt++ is a small library for command-line argument parsing. It is
lightweight and performance-focused.

### Usage

Include getopt++.h in your source file and construct the GOppParser with argc
as first parameter and argv as second parameter.  


    GOpp::Parser CommandLine(argc,argv,{{LongName,ShortName,MinArgCnt,MaxArgCnt,{AcceptedArguments}}}).  


### FAQ
#### Thrown exceptions:  
	std::invalid_argument for null argv or empty vector passed to Parser
	constructor.  
	std::string for unrecognized arguments (argument name is thrown).  
	unsigned long for insufficient amount of arguments.  
	
      