
Checkpoint 3:

Files that make up the test cases program given to us, in base folder:
	main_test.cpp

Files that make up our database API, in base folder:
	database.h
	database-api.lib rename to .lib
	api-specification.md
	README.txt
	checkpoint-3-report.md
	visualStudioTestCases/visualStudioTestCases.sln


Steps to run the program:

open the Visual Studio folder
open .sln Visual Studio project
	settings to make sure in menu bar:
		make sure Build -> Configuration Manager -> Project testQueryVisualStudio -> Platform 
			is x64
	settings to make sure in right sidebar:
		make sure the .cpp files are source files
			if not, add them from base folder as existing files
		right-click the project name and select Properties:
			go to Configuration Properties -> Linker -> General -> Additional Library Directories
				add your base path, which is holding the .lib file
			go to Configuration Properties -> Linker -> Input -> Additional Dependencies
				add the name of your .lib file
			make sure Configuration Properties -> C/C++ -> Precompiled Headers
				is off
run the program