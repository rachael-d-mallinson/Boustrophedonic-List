//-----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "Boustrophedonic.h"

void Remove(Node *&pHead, int row, int col)
{	
	//temp to be used in traverssing the list
	Node* traverseNode = pHead;

	//Node to remove is 0,0 - Very first in list
	if (row == 0 && col == 0)
	{
		//base case - if Node is only node
		//meaning that if there is not a Node to the East of 0,0 - then it's the only 1
		if (traverseNode->pEast == nullptr)
		{
			delete traverseNode;
		}
		else
		{
			//need to fix Node to south if it points to Current Node
			//if pEast's pSouth is nullptr, there is is only 1 row
			if (traverseNode->pEast->pSouth)
			{
				//fix ptr
				traverseNode->pEast->pSouth->pWest->pNorth = nullptr;
			}
			//reassign the head
			pHead = traverseNode->pEast;
			//finally delete and return - done
			delete traverseNode;
			return;
		}
	}

	//Node to remove is in row 1
	if (row == 0 && col != 0)
	{
		int colCount = 0;
		Node* prevNode = nullptr;
		while (colCount < col)
		{
			//keep track of Node before
			prevNode = traverseNode;
			//find Node to remove
			traverseNode = traverseNode->pEast;
			colCount++;
		}
		
		//fix the Node to the West's East ptr - skip over traverseNode
		prevNode->pEast = traverseNode->pEast;

		// just need to fix Node to south if it points to current Node
		//if it doesn't - it's already a nullptr
		if (traverseNode->pSouth == nullptr && traverseNode->pEast->pSouth != nullptr)
		{
			//will always have an East ptr if it's South is null and on row 1
			traverseNode->pEast->pSouth->pWest->pNorth = nullptr;
		}
		//finally delete and return - done
		delete traverseNode;
		return;
	}

	//node to remove is in the col 1
	if (row > 0 && col == 0)
	{
		traverseNode = pHead;
		//to keep track of prev Node(might be pEast or pWest depending on the row)
		Node* prevNode = nullptr;
		int rowCount = 0;
		while (rowCount != row)
		{
			prevNode = traverseNode->pEast->pSouth;
			traverseNode = traverseNode->pEast->pSouth->pWest;
			rowCount++;
			//check in middle
			if (rowCount == row)
			{
				break;
			}
			//move down 1 Node and loop to rowCount again
			prevNode = traverseNode;
			traverseNode = traverseNode->pSouth;
			rowCount++;
		}

		Node* belowNode = traverseNode;
		//in even row
		if (row % 2 == 0)
		{
			//there will always be a second node next to (to the right of)first col - col num is always even
			belowNode = belowNode->pEast; //move 1 t the right

			//check to see if current row is last row
			if (belowNode->pSouth != nullptr)
			{
				belowNode = belowNode->pSouth->pWest;
				belowNode->pNorth = prevNode;
				prevNode->pSouth = belowNode;
			}
			else //in last row
			{
				prevNode->pSouth = nullptr;

			}
		}

		//in not even row
		if (row % 2 != 0)
		{
			//just need to fix the prevNode, nothiing else points to it
			prevNode->pWest = nullptr;
		}
		//finally delete and return - done
		delete traverseNode;
		return;
	}

	//node to remove is not in the first row, nor the first column
	if (row > 0 && col > 0)
	{
		//keep track of previous
		Node* prevNode = pHead;
		//row is even
		if (row % 2 == 0)
		{
			int helpCount = 0;
			while (helpCount != row)
			{
				//since the row is even and it's not the current,
				//we can assume there is 2 more rows at least
				prevNode = traverseNode->pEast->pSouth->pWest;
				traverseNode = traverseNode->pEast->pSouth->pWest->pSouth;
				helpCount += 2;
			}
			//now we are in the right row - let's find our Node to remove
			helpCount = 0;
			while (helpCount != col)
			{
				prevNode = traverseNode;
				traverseNode = traverseNode->pEast;
				helpCount++;
			}
			//now we have arrived at our Node to remove
			//fix the prevNode - skip over traverseNode
			prevNode->pEast = traverseNode->pEast;

			Node* aboveNode = traverseNode;
			if (col % 2 == 0)
			{
				//means it's in even col, and there will always be a pEast
				aboveNode = aboveNode->pEast->pNorth->pWest; //at node above node to be removed
				aboveNode->pSouth = nullptr; //set in case there is no row below current

				//now check for another row
				if (traverseNode->pEast->pSouth != nullptr)
				{
					traverseNode->pEast->pSouth->pWest->pNorth = aboveNode; //set below node's north to above node
					aboveNode->pSouth = traverseNode->pEast->pSouth->pWest; //set above node's South to below node
				}
			}
			//if col is in odd row
			if (col % 2 != 0)
			{
				//if Node to remove is in odd number col- and even num row
				//then pointers go from it, and only the pointer to it is from the west Node - already fixed
			}
			//finally delete and return - done
			delete traverseNode;
			return;

		}

		//uneven row
		if (row % 2 != 0)
		{
			//means that we need to find the row above and move to the col num
			if (0 != (row - 1)) //if we aren't already above the row
			{
				int helpCount = 0;
				while (helpCount != (row - 1))
				{
					//we can assume there is are 3 more rows at least
					prevNode = traverseNode->pEast->pSouth->pWest;
					traverseNode = traverseNode->pEast->pSouth->pWest->pSouth;
					helpCount += 2;
				}
			}
			//now we are in the row above  -can move to the right
			prevNode = nullptr;

			//if the col is an odd col, then we can just go to right col num and down to find Node
			if (col % 2 != 0)
			{
				int helpCount = 0;
				while (helpCount != col)
				{
					prevNode = traverseNode;
					traverseNode = traverseNode->pEast;
					helpCount++;
				}
				//check if we are the last col - if we aren't - change the prev
				if (traverseNode->pEast != nullptr)
				{
					Node* aboveNode = traverseNode; //temp to hold above
					traverseNode = traverseNode->pSouth; //find Node to remove
					prevNode = aboveNode->pEast->pEast->pSouth->pWest; //find the prev Node of Node to remove
					prevNode->pWest = traverseNode->pWest; //fix the prev node ptr

					//check if we are in last row
					if (traverseNode->pWest->pSouth != nullptr)
					{
						//fix the above and below ptrs that point to current node to remove
						aboveNode->pSouth = traverseNode->pWest->pSouth->pEast;
						traverseNode->pWest->pSouth->pEast->pNorth = aboveNode;
					}
					else
					{
						aboveNode->pSouth = nullptr;
					}
				}
				else // are at the end col
				{
					prevNode = traverseNode;  //the prev node is above it
					traverseNode = traverseNode->pSouth; //the Node to remove is in the last col
					//there is no pEast node, and pWest doesn't point to it

					//check if in last row - if NOT
					if (traverseNode->pWest->pSouth != nullptr)
					{
						//fix the above and below Node ptrs
						prevNode->pSouth = traverseNode->pWest->pSouth->pEast;
						traverseNode->pWest->pSouth->pEast->pNorth = prevNode;
					}
					else //we are in last row
					{
						prevNode->pSouth = nullptr;
					}
				}//now we are at the Node to remove and have fixed the prev

				 //finally delete and return - done
				delete traverseNode;
				return;
			}

			//if the col is an even col
			if (col % 2 == 0)
			{
				//neighbor above has pSouth = nullptr
				//neighbor below has pNorth = nullptr
				//only need to fix pWest or pEast

				//now we are in the row above  -can move to the right
				//if even row - means that the num of col is odd, and it will ALWAYS be even, 
				//so there will ALWAYS be a col of the right of even row
				int helpCount = 0;
				while (helpCount != col + 1)
				{
					prevNode = traverseNode;
					traverseNode = traverseNode->pEast;
					helpCount++;
				}
				//now we are 1 row above and 1 col East from node to Remove
				traverseNode = traverseNode->pSouth;
				prevNode = traverseNode; //this is East neighbor of Node to remove
				traverseNode = traverseNode->pWest;

				//not in first row and not can't be in last row (means that the num of col is odd - must be another one)
				//fix the previous node, or East Neighbor
				prevNode->pWest = traverseNode->pWest;

				//finally delete and return - done
				delete traverseNode;
				return;

			}

		}

	}

}


// ---  End of File ---------------
