# Compiler
Design and implementation of lexical analyzer and syntax analyzer of PYTHON language using C++ ,and Node JS as a UI

enter yout python code in "input.txt"
the outout is in "output.txt"
 output contains : 
              1.stream of tokens (Value and Type)
              2.symbol table (functions,parameters and variables)
              3.syntax analyzer of the tokens and the Parse Tree
              4.error handeling of unknown tokens or wrong syntax 


To run the app without GUI :  
1.open the directory using vs code  
2.open the terminal then enter “g++ pilot.cpp” for compilation  
3.enter your input in the file named ‘input.txt’ 
4.to run the program enter this in the terminal : “./a input.txt” 
5.the output will be printed in the file ‘output.txt’ 

To run the app with GUI : 
1.first you have to install node js dependences  
in terminal : 
1.1 npm init –y 
1.2 npm install express 
1.3 node GUI\server\server.js   
2.open the terminal then enter “g++ pilot.cpp” for compilation 
3. open http://localhost:3000 
4.enter your python code in the text field  
5.press save 
6.get back to terminal to load the new input file  ‘./a input.txt’ 
7.go back to the website and press Display  
Then every time you want to edit the input :  apply second approach from 4 to 7 


