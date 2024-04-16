# Naming Conventions
- snake_case for file names and .hpp should be used instead of .h when C++ is used.
- snake_case for variable names
- camelCase for function names
- PascalCase for class, structure and enum names

- "m_" in front of private variables inside classes
-  "mp_" in front of private pointers variables inside classes
- "m" for private functions

# Code Formatting
- no separators (i.e. ////////////////////////////)

- inline curly brackets should be used i.e. as shown below
```
if
{
	\\ not like this
}
```
```
if {
	\\ yes like this
}
```
# Error Codes
- invalid_argument has error exit code -1
- runtime_error has error exit code -2
- exception has error exit code -3
- out_of_range has an error exit code -4

# Doxygen commentation
- @brief comment should have imperative mood and the first letter should be capitalised i.e. "Compute trans func" not "computes trans func" 