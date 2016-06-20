//
//  main.cpp
//  SparseMatrix
//
//  Created by Jonathan Cardasis on 2/10/16.
//  Copyright © 2016 Jonathan Cardasis. All rights reserved.
//

#include <iostream>
#include <string>
#include "SparseMatrix.hpp"
using namespace std;


/*
 Here are a few tests and examples of how ElementLists and SparseMatrix's can
 be used.
*/
int main() {
    /* Test for creating link lists and deep copying */
    ElementList list(5);
    cout <<"Starting List: "<< list << endl;
    cout << "Setting some elements..." << endl;
    list[0] = 1;
    list[3] = 4.5;
    
    list[4] = 9;
    list[4] = 10; //Override the element
    
    cout << "Element at [0]: " << list[0] << endl;
    cout << "Element at [3]: " << list[3] << endl;
    cout << "Element at [4]: " << list[4] << endl;
    
    cout << "----------------" << endl;
    cout << "Current List: " << list << endl;
    
    ElementList anotherList = list;
    cout << "Copy of List: " << anotherList << endl;
    cout << "_______________________" << endl;
    
    
    
    
    /* Test for subtracting lists from one another */
    ElementList lhs(5);
    ElementList rhs(5);
    lhs[0] = 1;
    lhs[3] = 2;
    rhs[0] = 2;
    rhs[1] = 4;
    rhs[3] = 1;
    cout << "Subtracting lists: " << endl;
    cout << lhs << " - " << rhs << " = " << lhs - rhs << endl;
    cout << "_______________________" << endl;
    
    
    
    /* Run unit tests */
    SparseMatrix sm;
    if(sm.sparseMatrixMultUnitTest())
        cout << "Passed Multiply Unit Test"<<endl;
    else
        cout << "Failed Multiply Unit Test"<<endl;
    
    if(sm.sparseMatrixEqualsUnitTest())
        cout << "Passed Equals Unit Test"<<endl;
    else
        cout << "Failed Equals Unit Test"<<endl;
    
    if(sm.sparseMatrixTransposeUnitTest())
        cout << "Passed Transpose Unit Test"<<endl;
    else
        cout << "Failed Transpose Unit Test"<<endl;
    
    cout << "_______________________"<<endl;
    
    
    
    
    /* Squares and Transposes */
    SparseMatrix matrix(3,5);
    SparseMatrix square;
    SparseMatrix esraps;
    matrix[0][0] = 1;
    matrix[1][1] = 2;
    matrix[2][2] = 3;
    matrix[2][4] = 4;
    
    esraps = matrix.tr();
    cout << "The transpose of" << endl << matrix << " is "<< endl << endl << esraps << endl << endl;
    
    square = matrix*esraps;
    cout << "The square of" << endl << matrix << " is " << endl << endl << square << endl << endl;
    cout << "_______________________"<<endl;
    
    
    
    /* Test for multiplying different sized matricies */
    SparseMatrix a(2,3);
    SparseMatrix b(3,2);
    
    a[0][0]=1;
    a[0][1]=2;
    a[1][1]=4;
    b[0][0]=3;
    b[1][1]=2;
    
    SparseMatrix c = a*b;
    cout<< a<<endl<<"*"<<endl<<b<<endl<<"is"<<endl<<endl<<c;
    
    return 0;
}
