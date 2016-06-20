<img src="http://i.imgur.com/L5RC8o1.png" height="85"> 
#SparseMatrix

##What is a Sparse Matrix?
A sparse matrix is a 2D matrix in which only specified values are stored. Any positions in the matrix which do not have a value set will store not store a value. This sparse matrix implements multiple linked lists (called ElementLists here) in order to create the SparseMatrix.

For example, this code:
```C++
SparseMatrix matrix(3,5);
matrix[0][0] = 1;
matrix[1][1] = 2;
matrix[2][2] = 9;
matrix[2][4] = 4;
```
will create a matrix as follows:
```
1 x x x x
x 2 x x x
x x 9 x 4
``` 
where the **x**'s symbolize the absense of an element being stored.

##Supported Functionality
####ElementList
- Addition/Subtraction
- Deep copy using `=`
- Access and mutate values using the `[]` operator like an array

####SparseMatrix
- Muliplication
- Deep copy using `=`
- Access and mutate values using the `[][]` operator like a two-dimentional array

##Example Usage
###:large_orange_diamond:ElementList
####Addition/Subtraction
```C++
ElementList lhs(5);
ElementList rhs(5);
lhs[0] = 1;
lhs[3] = 2;
rhs[0] = 2;
rhs[1] = 4;
rhs[3] = 1;

ElementList total = lhs - rhs;
cout << lhs << " - " << rhs << " = " << total << endl;
``` 

###:large_orange_diamond:SparseMatrix
####Multiplication
```C++
SparseMatrix a(2,3);
SparseMatrix b(3,2);

a[0][0]=1;
a[0][1]=2;
a[1][1]=4;
b[0][0]=3;
b[1][1]=2;

SparseMatrix c = a*b;
``` 

####Transpose
```C++
SparseMatrix matrix(3,5);
SparseMatrix xirtam;
matrix[0][0] = 1;
matrix[1][1] = 2;
matrix[2][2] = 3;
matrix[2][4] = 4;

xirtam = matrix.tr();
```
