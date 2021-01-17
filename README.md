# Cpp-LRUCache
Cpp LRUCache implementation


Clone project: git clone <repository_path>

Prerequisites: 
1. The boost library should be installed on the test machine at. Can use the below command to check:
whereis boost
2. If boost not installed yet. Use the below command to install:
sudo apt-get install libboost-all-dev 

Build project: 
1. make a new folder named "build" by using command: mkdir build
2. navigate to the created folder: cd build/
3. configure cmake projet: cmake ..
4. build project : make

Run project: 
Note: Repository contains 3 sample input data files named: Readers, Writers, Items. 
In order to test with different set of data files, please use following steps:

1. Put 3 required input files (readers_file, writers_file, items_file) into <ProjectWorkSpace>/data folder
3. Put data files Readers[i].txt, Writer[i].txt to folder named readers/writers respectively.
4. Open Terminal and navigate to build folder created above and run commands to resynchronize input data: 
make clean
make 
5. Run program by command:
./LRUCache <cache_size> readers_file writers_file items_file 

Output: 
1. Readers output log located at: <ClonedProjectPath>/reader_output/
2. Check the updated  items_file in folder <ClonedProjectPath>/data/


	
