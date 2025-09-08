#include <iostream>
#include <string>
using namespace std;

class MenuItem {
public:
	string itemName;
	int itemPrice;
	MenuItem* left;
	MenuItem* right;

	MenuItem(string n, int p) {
		itemName = n;
		itemPrice = p;
		left = right = NULL;
	}
};

class MenuSystem {
	MenuItem* root;

	MenuItem* insert(MenuItem* current , string n, int p) {
		if (!current) return new MenuItem(n, p);
		if (n < current->itemName)
			current->left = insert(current->left, n, p);
		else
			current->right = insert(current->right, n, p);
		return current;
	}

	void inOrder(MenuItem* node) {
		if (!node) return;
		inOrder(node->left);
		cout << node->itemName << " --> Rs. " << node->itemPrice << endl;
		inOrder(node->right);
	}

	bool search(MenuItem* node, string target) {
		if (!node) return false;
				if (node->itemName == target) return true;
				if (target < node->itemName)
					return search(node->left, target);
				else
					return search(node->right, target);
			}
public:
	MenuSystem()
	{
		root = NULL;
	}

	void addItem(string name, int price) {
		root = insert(root, name, price);
	}

	void showMenu() {
		cout << "\n========== MENU ==========\n";
		inOrder(root);
		cout << "==========================\n";
	}

	bool isItemAvailable(string name) {
		return search(root, name);
	}

	void searchItem(string name) {
		if (isItemAvailable(name))
			cout << name << " is available on the menu.\n";
		else
			cout << name << " is NOT on the menu.\n";
	}
};


struct Order {
	int orderId;
	string customerName;
	string itemName;
	Order(int id, string c, string i) {
		orderId = id;
		customerName = c;
		itemName = i;
	}
};

struct QueueNode {
	Order order;
	QueueNode* next;
	QueueNode(Order o) : order(o), next(NULL) {}
};

class OrderQueue {
	QueueNode* front;
	QueueNode* rear;
public:
	OrderQueue() { front = rear = NULL; }

	void enqueue(Order o) {
		QueueNode* newNode = new QueueNode(o);
		if (!rear) front = rear = newNode;
		else {
			rear->next = newNode;
			rear = newNode;
		}
		cout << "Order ID " << o.orderId << " placed successfully.\n";
	}

	bool dequeue(Order& o) {
		if (!front) return false;
		QueueNode* temp = front;
		o = front->order;
		front = front->next;
		if (!front) rear = NULL;
		delete temp;
		return true;
	}

	void display() {
		if (!front) {
			cout << "No active orders.\n";
			return;
		}
		QueueNode* temp = front;
		cout << "\n===== ACTIVE ORDERS =====\n";
		while (temp) {
			cout << "Order ID: " << temp->order.orderId
				<< ", Customer: " << temp->order.customerName
				<< ", Item: " << temp->order.itemName << endl;
			temp = temp->next;
		}
	}
};

struct DeliveryNode {
	Order order;
	DeliveryNode* next;
	DeliveryNode(Order o) : order(o), next(NULL) {}
};

class DeliveryHistory {
	DeliveryNode* head, *tail;
public:
	DeliveryHistory() { head = tail = NULL; }

	void add(Order o) {
		DeliveryNode* n = new DeliveryNode(o);
		if (!head) head = tail = n;
		else {
			tail->next = n;
			tail = n;
		}
	}

	void show() {
		if (!head) {
			cout << "No deliveries yet.\n";
			return;
		}
		cout << "\n===== DELIVERY HISTORY =====\n";
		DeliveryNode* temp = head;
		while (temp) {
			cout << "Order ID: " << temp->order.orderId
				<< ", Customer: " << temp->order.customerName
				<< ", Item: " << temp->order.itemName << endl;
			temp = temp->next;
		}
	}
};


struct StackNode {
	Order order;
	StackNode* next;
	StackNode(Order o) : order(o), next(NULL) {}
};

class CancelStack {
	StackNode* top;
public:
	CancelStack() { top = NULL; }

	void push(Order o) {
		StackNode* n = new StackNode(o);
		n->next = top;
		top = n;
	}

	bool pop(Order& o) {
		if (!top) return false;
		StackNode* temp = top;
		o = top->order;
		top = top->next;
		delete temp;
		return true;
	}
};

int main() {
	MenuSystem menu;
	OrderQueue queue;
	DeliveryHistory history;
	CancelStack cancel;

	menu.addItem("Zinger", 450);
	menu.addItem("Pizza", 800);
	menu.addItem("Biryani", 300);
	menu.addItem("Burger", 250);
	menu.addItem("Fries", 150);

	int id = 1;
	int choice;
	string cname, item;

	while (true) {
		cout << "\n====== ONLINE FOOD ORDERING SYSTEM ======\n";
		cout << "1. Show Menu\n2. Search Item\n3. Place Order\n4. Serve Order\n";
		cout << "5. Cancel Last Order\n6. View Active Orders\n7. Delivery History\n8. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;
		cin.ignore();

		if (choice == 1) {
			menu.showMenu();
		}
		else if (choice == 2) {
			cout << "Enter item to search: ";
			getline(cin, item);
			menu.searchItem(item);
		}
		else if (choice == 3) {
			cout << "Enter customer name: ";
			getline(cin, cname);
			do {
				cout << "Enter item to order: ";
				getline(cin, item);
				if (!menu.isItemAvailable(item)) {
					cout << "Item not available! Please choose from the menu:\n";
					menu.showMenu();
				}
			} while (!menu.isItemAvailable(item));
			Order o(id++, cname, item);
			queue.enqueue(o);
			cancel.push(o);
		}
		else if (choice == 4) {
			Order o(0, "", "");
			if (queue.dequeue(o)) {
				cout << "Serving Order ID: " << o.orderId << endl;
				history.add(o);
			}
			else {
				cout << "No order to serve.\n";
			}
		}
		else if (choice == 5) {
			Order o(0, "", "");
			if (cancel.pop(o)) {
				cout << "Canceled Order ID: " << o.orderId << endl;
			}
			else {
				cout << "No order to cancel.\n";
			}
		}
		else if (choice == 6) {
			queue.display();
		}
		else if (choice == 7) {
			history.show();
		}
		else if (choice == 8) {
			cout << "Thank you for using the system!\n";
			break;
		}
		else {
			cout << "Invalid choice!\n";
		}
	}
	system("pause");
	return 0;
}
