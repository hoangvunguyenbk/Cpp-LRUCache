# Cpp-LRUCache
Cpp LRUCache implementation


Clone project: git clone <repository_path>

Prerequisites: 
1. Cmake (minimum verion 3.16)
2. The boost library should be installed on the test machine. Can use the below command to check: 
**whereis boost**
3. If boost not installed yet. Use the below command to install:
**sudo apt-get install libboost-all-dev** 

Build project: 
4. Make a new folder named "build" by using command: **mkdir build**
5. Navigate to the created folder: **cd build/**
6. Configure cmake projet: **cmake ..**
7. Build project : **make**

Run project: 
Note: Repository contains 3 sample input data files named: Readers, Writers, Items. 
In order to test with different set of data files, please use following steps:

1. Put 3 required input files (readers_file, writers_file, items_file) into <*ClonedProjectPath*>/data folder
3. Put data files Readers[i].txt, Writer[i].txt to folder named readers/writers respectively.
4. Open Terminal, navigate to build folder created above and run commands to resynchronize input data: 
**make clean**
**make** 
5. Run program by command:
**./LRUCache <cache_size> readers_file writers_file items_file** 

Output: 
1. Readers output log located at: <*ClonedProjectPath*>/reader_output/
2. Check the updated  items_file in folder <*ClonedProjectPath*>/data/

