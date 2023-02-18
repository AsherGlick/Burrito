cd ..

cd ./generators

python code_generator.py 

cd ..

cd ./build
cmake ..
make

# cd ./generators