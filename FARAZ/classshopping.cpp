class ShoppingSystem {
private:
    Database db;
    Cart cart;

    bool authenticateAdmin() {
        string email, password;
        system("cls");
        setColor(14);
        cout << "+---------------------------------------------+\n";
        cout << "|               ADMIN LOGIN                  |\n";
        cout << "+---------------------------------------------+\n";
        setColor(15);
        cout << "  Enter Email: ";
        cin >> email;
        cout << "  Enter Password: ";
        cin >> password;
        return (email == "aqibmehmood13w@gmail.com" && password == "password123");
    }

    void administratorMenu();
    void buyerMenu();

public:
    ShoppingSystem() : db(), cart(db) {}

    void menu();
};



void ShoppingSystem::administratorMenu() {
    while (true) {
        system("cls");
        setColor(10);
        cout << "\n\n\t\t\t ADMINISTRATOR MENU\n";
        cout << "\t\t\t =======================\n";
        setColor(15);
        cout << "\t\t\t [1] Add Product\n";
        cout << "\t\t\t [2] Modify Product\n";
        cout << "\t\t\t [3] Delete Product\n";
        cout << "\t\t\t [4] Back to Main Menu\n";
        cout << "\t\t\t =======================\n";
        cout << "\t\t\t Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                Product p;
                int code;
                string name;
                float price, discount;
                cout << "\n Enter Product Code: ";
                cin >> code;
                cin.ignore();
                cout << " Enter Name of Product: ";
                getline(cin, name);
                cout << " Enter Price: ";
                cin >> price;
                cout << " Enter Discount %: ";
                cin >> discount;
                p.setCode(code);
                p.setName(name);
                p.setPrice(price);
                p.setDiscount(discount);
                db.addProduct(p);
                break;
            }
            case 2: {
                int pkey;
                cout << "\n Enter Product Code to Modify: ";
                cin >> pkey;
                Product p;
                int code;
                string name;
                float price, discount;
                cout << " Enter new Product Code: ";
                cin >> code;
                cin.ignore();
                cout << " Enter new Product Name: ";
                getline(cin, name);
                cout << " Enter new Product Price: ";
                cin >> price;
                cout << " Enter new Discount %: ";
                cin >> discount;
                p.setCode(code);
                p.setName(name);
                p.setPrice(price);
                p.setDiscount(discount);
                db.editProduct(pkey, p);
                break;
            }
            case 3: {
                int pkey;
                cout << "\n Enter Product Code to Delete: ";
                cin >> pkey;
                db.deleteProduct(pkey);
                break;
            }
            case 4:
                return;
            default:
                cout << "Invalid choice\n";
        }
        system("pause");
    }
}

void ShoppingSystem::buyerMenu() {
    while (true) {
        system("cls");
        setColor(11);
        cout << "\t\t\t BUYER MENU\n";
        cout << "\t\t\t ___________\n";
        setColor(15);
        cout << "\t\t\t [1] Buy Product\n";
        cout << "\t\t\t [2] Go Back\n";
        cout << "\t\t\t ___________\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        switch (choice) {
            case 1: {
                db.listProducts();
                cout << "\n\n ===== PLACE YOUR ORDER =====\n";
                char more;
                do {
                    int code, quantity;
                    cout << "\n Enter Product Code: ";
                    cin >> code;
                    cout << " Enter Quantity: ";
                    cin >> quantity;
                    if (cart.addItem(code, quantity)) {
                        cout << " Item added to cart.\n";
                    }
                    cout << " Add another product? (y/n): ";
                    cin >> more;
                } while (more == 'y' || more == 'Y');
                cart.generateReceipt();
                break;
            }
            case 2:
                return;
            default:
                cout << "Invalid choice\n";
        }
        system("pause");
    }
}

void ShoppingSystem::menu() {
    while (true) {
        system("cls");
        ofstream create("database.txt", ios::app);
        create.close();

        setColor(9);
        cout << "\t\t\t\t +===========================================+\n";
        cout << "\t\t\t\t |             SUPER MARKET MENU             |\n";
        cout << "\t\t\t\t +===========================================+\n";
        setColor(15);
        cout << "\t\t\t\t |                                           |\n";
        cout << "\t\t\t\t |    [1] ADMINISTRATOR                      |\n";
        cout << "\t\t\t\t |    [2] BUYER                              |\n";
        cout << "\t\t\t\t |    [3] EXIT                               |\n";
        cout << "\t\t\t\t |                                           |\n";
        cout << "\t\t\t\t +===========================================+\n";
        cout << "\t\t\t\t >> Please select: ";
        int choice;
        cin>>choice;

        switch (choice) {
            case 1:
                if (authenticateAdmin()) {
                    administratorMenu();
                } else {
                    cout << "\n Invalid credentials.\n";
                    system("pause");
                }
                break;
            case 2:
                buyerMenu();
                break;
            case 3:
                exit(0);
            default:
                cout << "\n Invalid option.\n";
                system("pause");
        }
    }
}

int main() {
    ofstream f("database.txt", ios::app);
    f.close();

    ShoppingSystem shop;
    shop.menu();
    return 0;
}