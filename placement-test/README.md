# Submission for Ampify engineering placement test- Lewis Hamilton 

## Running the C++ File
- For the C++ file, I've added a couple of additional std 'includes' including cmath and iomanip. Iomanip is simply used for getting the correct no of decimal places while cmath was imported but none of its functions used because without it, on my system, std::abs gives a ```call to abs is ambiguous ``` error 
- All the code runs correctly on my MacOS using ``` g++ calculator-main.cpp -std=c++17 ``` 

## Running the Python File 
- The Python file uses Python3 (specifically 3.9.7) with the urllib.request and json packages. Both are part of the standard python libraries however and should come packaged with Python3. 
- Script was exectued on MacOS terminal with simply ```python3 webApi.py ```


## What I changed / Design Decisions 
- With the calculator cpp, I added the divison operator with error handling for dividing by zero
- I also added a consts function, which find different const strings (pi, root 2) and returns their approximated value 
- In terms of changes made to the calculator, I removed a test case in test() which seemed to suggest 3 - 5.6 = +2.6 insteasd of -2.6. I'm not sure if this was implying that I had to change the calculator to remove any instances of negative answers and replace them with the absolute values but since that wasn't explicit in the original readme, I didn't add it. 
- I also changed both findAndExtract..() and calculate() so that they 'fail gracefully' i.e return a more human-readable error message instead of a stod error or something like that. 


