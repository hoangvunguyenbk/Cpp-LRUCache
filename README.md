# Cpp-LRUCache
Cpp LRUCache implementation

Prerequisites: 

1. Cmake (minimum version 3.16)

2. The boost library

3. Google test framework


Run file setup.sh in <*ClonedProjectPath*> with sudo privilege to install prerequisites tools: 
    **./setup.sh**

Build project: 
1. Create a new folder named "build" by using command: 

    **mkdir build**
	
2. Navigate to the created folder:

    **cd build/**
	
3. Configure cmake projet:

    **cmake ..**
	
4. Build project:

    **make**

Run project: 

***Note:*** Repository contains 3 sample input data files named: Readers, Writers, Items. 
In order to test with a different set of data files, please use the following steps:

1. Put 3 required input files (readers_file, writers_file, items_file) into <*ClonedProjectPath*>/data/ folder. 

2. Put data files Readers[i].txt, Writer[i].txt to folder named readers/writers respectively.

3. Open Terminal, navigate to build folder created above and run commands to resynchronize input data, data files will be copy to build folder and run from there: 

    **make clean**

    **make** 

4. Run program by command:

    **./LRUCache <cache_size> readers_file writers_file items_file** 
	
5. Run Unittest by command:

    **./LRUCachTest** 

Output: 

1. Readers output log located at: <*ClonedProjectPath*>/reader_output/

2. Check the updated  items_file in folder <*ClonedProjectPath*>/data/

Class Diagram: 


[![Class-Diagram1.jpg](https://i.postimg.cc/3RmbpjM4/Class-Diagram1.jpg)](https://postimg.cc/6260xvpt)



