#include <stdio.h>
#include <stdlib.h>


/*Need to know the vertices that can be visited. This can be
done by creating a 1d array of arrays storing x,y location of points that can be visited*/

int possibleLocations[100];


struct Node {
	int data;
	struct Node *next;
};

struct Node *head = 0; // declared in gloabl scope
struct Node *tail = 0;

/*function to add to the end of the list*/
void appendLinkedList(int data) {
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node)); //variable on heap
	temp->data = data;
	temp->next = 0;

	if (head == 0 && tail == 0) { //if both pointers point to null then the list is empty
		head = tail = temp; // head and tail equal to temp
		return;
	}
	//else
	tail->next = temp; //last node points to the new node
	tail = temp;//tail node will now point to the newly added node
}

/*function to delete from the end of the list*/
void deleteFromLinkedList(int dataValue) {
	struct Node *temp;

	if (head == 0 && tail == 0) { //if both pointers point to null then the list is empty
		return;
	}
	struct Node *currentNode = head;
	//else

	if (head->data == dataValue) {//if head is the first pointer to be deleted
		temp = head;
		temp->next = 0; //set the pointer to null for the head
		head = head->next;
		return;
	}
	while (currentNode->next->next != 0) {
		if (currentNode->next->data == dataValue) { //if current points to the dataValue entered
			currentNode->next = currentNode->next->next; //skip the dataValue and point to the next of the next node
			return;
		}
		currentNode = currentNode->next;//else just move on to next node
	}
	currentNode->next = 0; //the next of the current node is 0 now, so the last element pointer now points to 0
	currentNode->next->data = 0;
	
}

/*function to add to the front of the list*/
//void prependLinkedList(int dataValue) {
	//struct Node *temp;
	//temp->next = head; // the new temp variable now points to head
	//temp->data = dataValue;
	//head = temp;
	
//}

int startLocation, destin_locaiton;

//functions for BFS
void generateGraph();
void bf_Traversal();
void BFS();

int delete_queue();
int isEmpty_queue();
int insert_queue();


int main() {
	return 0;
}