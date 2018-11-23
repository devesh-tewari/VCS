#include <bits/stdc++.h> 
using namespace std; 


int searchStr(string arr[], string str, int first, int last) 
{ 
	if (first > last) 
		return -1; 

	int mid = (last+first)/2; 
	if (str.compare(arr[mid]) == 0) 
		return mid; 
	if (str.compare(arr[mid]) > 0) 
		return searchStr(arr, str, mid+1, last); 
	return searchStr(arr, str, first, mid-1); 
} 

// Driver Code 
int main() 
{ 
	// Input arr of Strings. 
	string arr[] = {"for", "forr", "forr", "forr", "geeks", "ide", "idle", 
					"practice", "practice" , "practice", "quiz", "z", "zz"}; 

	// input Search String 
	string str = "for"; 
	int n = sizeof(arr)/sizeof(arr[0]); 

	cout << searchStr(arr, str, 0, n-1); 
	return 0; 
} 

