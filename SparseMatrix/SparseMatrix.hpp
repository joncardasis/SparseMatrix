//
//  SparseMatrix.hpp
//  SparseMatrix
//
//  Created by Jonathan Cardasis on 2/13/16.
//  Copyright © 2016 Jonathan Cardasis. All rights reserved.
//
//  Contained Classes/Structs:
//    + Element (Struct)
//    + ElementList (Class)
//    + SparseMatrix (Class)
//
//  Purpose:
//  This code creates a SparseMatrix, or a matrix in which only the locations
//  which have been set values actually exists and consume memory and all other
//  locations in the SparseMatrix are represented as zeros.
//    An example 3x5 SparseMatrix:
//     0 0 1 0 2
//     0 0 0 0 0
//     3 0 5 0 9
//    In this matrix the only existing element values are 1,2,3,5,and 9. Each element is
//    an Element stuct which contains a value and the column location it should exist at.
//
//  Each row in the SparseMatrix is an ElementList object, which is a linked list
//  containing Element objects.
//
//  Input:
//  + A SparseMatrix's value at a location can be set using matrix[row][col] = value
//  + A ElementList's value at a location can be set using list[col] = value
//  + To access the values of an ElementList or SparseMatrix an example would be list[col]
//    or matrix[row][col] respectively.
//
//  Output:
//  + A SparseMatrix as well as an ElementList can be printed using the << operator.
//


#ifndef SparseMatrix_hpp
#define SparseMatrix_hpp

#include <iostream>
#include <string>
using namespace std;


struct Element {
    int col;
    double value;
    Element *next;
    Element(int c= -1, double v=0., Element *nxt=nullptr){
        col = c;
        value = v;
        next = nxt;
    }
    void operator = (double v){
        value = v;
    }
    friend ostream &operator << (ostream &out, Element e);
};

/*
Prints out an Element's value
Post: returns an ostream with the Element's value
*/
ostream &operator << (ostream &out, Element e){
    out << e.value;
    return out;
}


//MARK: ElementList
class ElementList{
public:
    
/* ---Constructors and Destructors--- */
    ElementList(const int max=0){
        maxCols = max;
        list = nullptr;
    }
    
    ElementList(const ElementList &rhs){ //Deep Copy Constructor
        maxCols = rhs.maxCols;
        list = rhs.list;
        if(list != nullptr){
            list = new Element(rhs.list->col, rhs.list->value, rhs.list->next);
            Element *leftPtr = list;
            Element *rightPtr = rhs.list;
            
            while(rightPtr->next != nullptr){
                Element *elementToCopy = rightPtr->next;
                leftPtr->next = new Element(elementToCopy->col, elementToCopy->value, elementToCopy->next);
                
                leftPtr = leftPtr->next;
                rightPtr = rightPtr->next;
            }
        }
    }
    
    ~ElementList(){ //Destructor
        deleteList(list);
    }
    
/* ---Accessors and Mutators--- */
    
    /*
    Sets the how many columns an ElementList should have.
    Post: the ElementList will contain the number of max afterwards. The list variable is also set to nullptr
    */
    void set(int max){
        list = nullptr;
        maxCols = max;
    }
    
    
    /*
    Post: returns the first Element in the ElementList.
    */
    Element *& getList(){
        return list;
    }
    
    
    /*
    Post: Returns the value of the list at column i. If no Element exists at column, zero is returned.
    */
    double getIth(int i){
        Element *ptr = list;
        for(int j=0; j<maxCols && ptr != nullptr; j++){
            if(ptr->col == i){
                return ptr->value;
            }
            ptr = ptr->next;
        }
        return 0; //No value exists for column i so return 0
    }
    
    
/* ---Operators--- */
    
    /*
    Post: sets the ElementList of the left of the '=' operator to an exact copy of whats on the right, copying and allocating
          new memory for copied Elements.
    */
    ElementList operator = (const ElementList &rhs){ //Deep Copy equals
        maxCols = rhs.maxCols;
        
        if(list != nullptr){ //Delete lhs's list if it exists
            deleteList(list);
        }
        
        if(rhs.list != nullptr){
            list = new Element(rhs.list->col, rhs.list->value, rhs.list->next);
            Element *leftPtr = list;
            Element *rightPtr = rhs.list;
            
            while(rightPtr->next != nullptr){
                Element *elementToCopy = rightPtr->next;
                leftPtr->next = new Element(elementToCopy->col, elementToCopy->value, elementToCopy->next);
                
                leftPtr = leftPtr->next;
                rightPtr = rightPtr->next;
            }
        }
        
        return *this;
    }
    

    /*
    Adds together two ElementLists: ex. [ 1 0 0 2 ] + [ 2 4 0 1 ] is [ 3 4 0 3 ]
    Post: returns an ElementList with Elements added together which have the same column
    */
    ElementList operator + (const ElementList &rhs){
        ElementList newElementList(max(maxCols,rhs.maxCols)); //Create new list with num of elements to match the larger of the two operanded elementlists
        
        Element *ptr = list;
        Element *rhsPtr = rhs.list;
        while(ptr != nullptr || rhsPtr != nullptr){
            
            if(ptr!=nullptr && rhsPtr == nullptr){//If there are elements in lhs that have a bigger col than rhs' maxCols
                newElementList[ptr->col] = ptr->value;
                ptr = ptr->next;
            }
            else if(rhsPtr!=nullptr && ptr == nullptr){ //If there are elements in rhs that have a bigger col than lhs' maxCols
                newElementList[rhsPtr->col] = rhsPtr->value;
                rhsPtr = rhsPtr->next;
            }
            
            
            else if(ptr != nullptr && rhsPtr != nullptr){ //Elements exists at current pointers
                if(ptr->col == rhsPtr->col){ //element's cols match so add them together
                    newElementList[rhsPtr->col] = ptr->value + rhsPtr->value; //Add a new element at the index the two ptr elements share
                    rhsPtr = rhsPtr->next;
                    ptr = ptr->next;
                }
                
                else if(ptr->col < rhsPtr->col){ //An element exists in lhs that doesnt in the rhs
                    newElementList[ptr->col] = ptr->value;
                    ptr = ptr->next;
                }
                else if(ptr->col > rhsPtr->col){ //An element exists in rhs that doesnt in the lhs
                    newElementList[rhsPtr->col] = rhsPtr->value;
                    rhsPtr = rhsPtr->next;
                }
            }
        }
        return newElementList;
    }

    
    /*
    Subtracts two ElementLists: ex. [ 1 0 0 2 ] - [ 2 4 0 1 ] is [ -1 -4 0 1 ]
    (This function is the same as the addition operator with the execption that it subtracts values and also
     produces a negative if a value in the rhs exists for a col which doesnt for the lhs in the same col)
    Post: returns an ElementList with Elements added together which have the same column
    */
    ElementList operator - (const ElementList &rhs){
        ElementList newElementList(max(maxCols,rhs.maxCols)); //Create new list with num of elements to match the larger of the two operanded elementlists
        
        Element *ptr = list;
        Element *rhsPtr = rhs.list;
        while(ptr != nullptr || rhsPtr != nullptr){
            
            if(ptr!=nullptr && rhsPtr == nullptr){//If there are elements in lhs that have a bigger col than rhs' maxCols
                newElementList[ptr->col] = ptr->value;
                ptr = ptr->next;
            }
            else if(rhsPtr!=nullptr && ptr == nullptr){ //If there are elements in rhs that have a bigger col than lhs' maxCols
                newElementList[rhsPtr->col] = -rhsPtr->value;
                rhsPtr = rhsPtr->next;
            }
            
            
            else if(ptr != nullptr && rhsPtr != nullptr){ //Elements exists at current pointers
                if(ptr->col == rhsPtr->col){ //element's cols match so add them together
                    newElementList[rhsPtr->col] = ptr->value - rhsPtr->value; //Add a new element at the index the two ptr elements share
                    rhsPtr = rhsPtr->next;
                    ptr = ptr->next;
                }
                
                else if(ptr->col < rhsPtr->col){ //An element exists in lhs that doesnt in the rhs
                    newElementList[ptr->col] = ptr->value;
                    ptr = ptr->next;
                }
                else if(ptr->col > rhsPtr->col){ //An element exists in rhs that doesnt in the lhs
                    newElementList[rhsPtr->col] = -rhsPtr->value;
                    rhsPtr = rhsPtr->next;
                }
            }
        }
        return newElementList;
    }
    
    
    /*
    Returns a value when ElementList[col] is accessed.
    Pre:  col must be within the ElementList's range
    Post: if a value exists at col it is returned. If no value exists, zero is returned
    */
    const double operator[] (int col) const {
        Element * ptr = list;
        while (ptr != nullptr && ptr->col  < col){
            ptr = ptr->next;
        }
        if (ptr == nullptr || ptr->col  > col)
            return 0;
        else
            return ptr->value;
    }
    
    
    /*
    Returns a value when ElementList[col] is accessed. Called when an Element at an index is set.
    Pre:  col must be within the ElementList's range
    Post: returns the value of the Element at the column in the ElementList. If no Element exists at that column, one is created and it's new value (0) is returned.
    */
    double & operator[] (int col)  {
        Element * ptr = list;
        Element * newNode;
        Element * trailer = list;
        if (list == nullptr || col < list->col){
            newNode = new Element(col,0, list);
            list = newNode;
            return newNode->value;
        } while (ptr != nullptr && ptr->col  < col){
            trailer = ptr;
            ptr = ptr->next;
        }
        
        if (ptr != nullptr && ptr->col == col)
            return ptr->value;
        else{
            newNode = new Element(col, 0, ptr);
            trailer->next = newNode;
            return newNode->value;
        }
    }
    
    /* Friends */
    friend ostream & operator << (ostream &out, ElementList list);
private:
    Element *list;
    int maxCols;
    
    void deleteList(Element *head){
        Element *current = head;
        Element *next;
        while (current != nullptr && current->next !=nullptr){
            next = current->next;
            delete current;
            current = next;
        }
        list = nullptr;
    }
};

/*
Prints out an ElementList in the format [ x x x x ]
Post: returns an ostream with each Element in ElementList list in its correct column with zeros where no element exists.
*/
ostream & operator << (ostream &out, ElementList list){
    Element *ptr = list.list; //Set a pointer to the ElementList's list pointer
    out << "[ ";
    for(int i=0; i<list.maxCols; i++){
        if(ptr != nullptr && ptr->col == i){
            out << ptr->value << " ";
            ptr = ptr->next;
        }
        else
            out << 0 << " ";
    }
    out << "]";
    return out;
}





//MARK: SparseMatrix
class SparseMatrix {
public:
/* ---Constructors and Destructors--- */
    SparseMatrix(int n=0, int m=0){
        numRows = n;
        numCols = m;
        rows = new ElementList[n];
        for(int i=0; i < n; i++){
            rows[i].set(m);
        }
    }
    
    SparseMatrix(const SparseMatrix &rhs){ //Deep Copy Constructor
        numRows = rhs.numRows;
        numCols = rhs.numCols;
        rows = new ElementList[rhs.numRows];
        for(int i=0; i < rhs.numRows; i++){
            rows[i] = rhs.rows[i];
        }
    }
    
    ~SparseMatrix(){ //Destructor
        delete [] rows; //Will call destructors for each ElementList in the rows array
    }
    
/* ---Accessors and Mutators--- */
    
     /*
     Transposes values of two SparseMatrixs. Where B.tr() is called B[i][j] = A[j][i].
     Post: returns a deep copy transpose of self.
     */
    SparseMatrix tr(){
        //Create a new matrix with the numRows and numCols variables swapped
        SparseMatrix newMatrix(numCols, numRows);
        
        for(int row=0; row < newMatrix.numRows; row++){
            Element *ptr = rows[row].getList(); //point to head of row
            while(ptr != nullptr && row < numRows){
                //Swap values from current matrix to the new matrix
                
                newMatrix.rows[ptr->col][row] = rows[row][ptr->col]; //Will deep copy the Element
                ptr = ptr->next;
            }
        }
        
        return newMatrix;
    }
    
/* ---Operators--- */
    
    /*
    Sets the current SparseMatrix to have equal values to the rhs SparseMatrix
    Post: returns a SpraseMatrix which has equal values to the rhs
    */
    SparseMatrix operator = (const SparseMatrix &rhs){
        
        numRows = rhs.numRows;
        numCols = rhs.numCols;
        
        delete [] rows; //erase rows from memory
        rows = new ElementList[rhs.numRows]; //create new list with new size
        for(int i=0; i < rhs.numRows; i++){
            rows[i] = rhs.rows[i];
        }
        
        return *this;
    }
    
    
    /*
    Creates a new matrix square containing multiplied values of the two product matrices.
    If C = A*B, where A is size n x m and B is m x p then C is n x p
    And C[i,j] = A[i,0]*B[0,j] + A[i,1]*B[1,j] + ... + A[i,m]*B[m,j]
    
    Pre:  rhs numRows must be the exact same as the lhs numCols.
    Post: returns self * rhs
    */
    SparseMatrix operator * (const SparseMatrix &rhs){
        SparseMatrix newMatrix(numRows, rhs.numCols);
        int m = numCols; //Value of the matrices' matching col and row
        
        for(int row=0; row < newMatrix.numRows; row++){
            for(int col=0; col < newMatrix.numCols; col++){
                //Now set the element value for (row,col) in the newMatrix matrix
                int val = 0;
                
                //Calculate the value for pos (row,col) in newMatrix
                for(int counter=0; counter<m; counter++){ //run through the rows and cols to find the value of what newMatrix at (row,col) should be
                        double lhsVal = rows[row].getIth(counter);
                        double rhsVal = rhs.rows[counter].getIth(col);//gets value at current col
                        val += (lhsVal * rhsVal);
                }
                
     
                if(val != 0)//if the value is zero, dont record it since its the default access value
                    newMatrix.rows[row][col] = val; //Set the calculated value
            }
        }

        return newMatrix;
    }
    
    
    /*
    Returns a value when SparseMatrix[row] is accessed.
    Pre:  row must be within the SparseMatrix's range
    Post: returns the elementlist in the Sparsematrix at the row row
    */
    const ElementList operator [] (int row ) const{
        return rows[row];
    }

    
    /*
    Returns a value when SparseMatrix[row] is accessed. Called when an ElementList at an index is set.
    Pre:  row must be within the SparseMatrix's range
    Post: returns the elementlist in the Sparsematrix at the row row
    */
    ElementList & operator [] (int row){
        return rows[row];
    }
    
    
/*---Unit Tests---*/
    /*
    Unit test for multiplying two sparsematrices. Checks if after multiplying the resulting
     sparsematrix's rows and cols are correctly sized and that all resulting values in the
     new sparsematrix are correct.
    Post: returns true if the unit test passes, or false if it fails
    */
    bool sparseMatrixMultUnitTest(){
        SparseMatrix a(3,5);
        SparseMatrix b(5,3);
        
        a[0][1] = 1;
        a[0][4] = 5;
        a[1][2] = 2;
        a[2][2] = 3;
        
        b[0][0] = 1;
        b[1][1] = 6;
        b[2][2] = 2;
        b[3][0] = 0;
        b[3][2] = 3;
        
        SparseMatrix c = a*b;
        if(c.numRows==3 && c.numCols==3){ //Unit test - first check if the correct size is set
            if(c[0][1]==6 && c[1][2]==4 && c[2][2]==6){ //check if values are correct
                return true;
            }
        }
        return false;
    }
    
    /*
    Unit test for copying a sparsematrix with the = operator. Checks for sizes being set
     correctly and that all values are the same after the = operator with no extra elements being added.
    Post: returns true if the unit test passes, or false if it fails
    */
    bool sparseMatrixEqualsUnitTest(){
        SparseMatrix a(2,3);
        // 0 1 2
        // 3 4 5
        a[0][1] = 1;
        a[0][2] = 2;
        a[1][0] = 3;
        a[1][1] = 4;
        a[1][2] = 5;
        
        SparseMatrix b = a;
        
        if(b.numRows!=a.numRows || b.numCols!=a.numCols) //Test if sizes are the same
            return false;
        
        
        for(int row=0; row<a.numRows; row++){//Test if same number of elements exist in both
            Element *bRowHead = b[row].getList();
            Element *aRowHead = a[row].getList();
            
            int bRowCount=0;
            int aRowCount=0;
            
            while(bRowHead != nullptr){
                bRowCount++;
                bRowHead = bRowHead->next;
            }
            while(aRowHead != nullptr){
                aRowCount++;
                aRowHead = aRowHead->next;
            }
            
            if(aRowCount != bRowCount)
                return false;
        }
        
        if(b[0][1]==1 && b[0][2]==2 && b[1][0]==3 && b[1][1]==4 && b[1][2]==5){//check if values are correct
            //If here has been reached the matrix b has only the elements a has and in the correct locations.
            return true;
        }
        
        return false;
    }
    
    /*
    Unit test for transposing a sparsematrix. Checks if the transposed sparsematrix has its
     transposed values in the correct places.
    Post: returns true if the unit test passes, or false if it fails
    */
    bool sparseMatrixTransposeUnitTest(){
        SparseMatrix a(3,5);
        /*
        2 x 1 9 x
        x x x x 4
        x 7 x x x
        */
        
        a[0][0] = 2;
        a[0][2] = 1;
        a[0][3] = 9;
        a[1][4] = 4;
        a[2][1] = 7;
        
        SparseMatrix b = a.tr();
        if(b.numRows==a.numCols && b.numCols==a.numRows){//Unit test - first check if the sizes have been flipped
            int numOfElementsA=0;
            for(int row=0; row < a.numRows; row++){
                Element *ptr = a.rows[row].getList(); //point to head of row
                while(ptr != nullptr){
                    numOfElementsA++;
                    ptr = ptr->next;
                }
            }
            
            int numOfElementsB=0;
            for(int row=0; row < b.numRows; row++){
                Element *ptr = b.rows[row].getList();
                while(ptr != nullptr){
                    numOfElementsB++;
                    ptr = ptr->next;
                }
            }
            
            //Unit test - Ensure matrices have same number of element
            if(numOfElementsA != numOfElementsB){
                return false;
            }
            
            //Unit test - Ensure that the matrices share the same values and positions
            if(b[0][0]==2 && b[1][2]==7 && b[2][0]==1 && b[3][0]==9 && b[4][1]==4){//check if values are in the correct positions for matrix b
                return true;
            }
            
        }
        
        return false;
    }
    
    
    /* Friends */
    friend ostream &operator << (ostream &out, SparseMatrix matrix);
private:
    int numRows;
    int numCols;
    
    ElementList *rows;
};

/*
Prints out a SparseMatrix
Post: prints the values stored in the SparseMatrix matrix.
*/
ostream &operator << (ostream &out, SparseMatrix matrix){
    for(int i=0; i < matrix.numRows; i++){
        out << matrix[i];
        if(i+1 < matrix.numRows)//Don't print a newline after the whole sparsematrix
            out << endl;
    }
    return out;
}

#endif /* SparseMatrix_hpp */
