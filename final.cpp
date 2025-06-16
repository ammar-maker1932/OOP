#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <windows.h>
using namespace std;

class Product {
private:
    int pcode;
    string name;
    float price;
    float discount;

public:
    Product(int code = 0, string n = "", float p = 0.0, float d = 0.0)
        : pcode(code), name(n), price(p), discount(d) {}

    int getCode() const { return pcode; }
    string getName() const { return name; }
    float getPrice() const { return price; }
    float getDiscount() const { return discount; }

    void setCode(int code) { pcode = code; }
    void setName(const string& n) { name = n; }
    void setPrice(float p) { price = p; }
    void setDiscount(float d) { discount = d; }
};

class Database {
private:
    string filename;

public:
    Database(const string& fname = "database.txt") : filename(fname) {}

    bool addProduct(const Product& product) {
        fstream data(filename.c_str(), ios::in);
        int code;
        string name;
        float price, discount;
        bool duplicate = false;

        if (data.is_open()) {
            while (data >> code) {
                getline(data, name, ' ');
                getline(data, name, ' ');
                data >> price >> discount;
                if (code == product.getCode()) {
                    duplicate = true;
                    break;
                }
            }
            data.close();
        }

        if (duplicate) {
            cout << "\n\n\t\t Product code already exists!\n";
            return false;
        }

        fstream data_out(filename.c_str(), ios::app);
        if (!data_out) {
            cout << "\n\n\t\t Error: Could not open file for writing!\n";
            return false;
        }
        data_out << product.getCode() << " " << product.getName() << " "
                 << product.getPrice() << " " << product.getDiscount() << endl;
        data_out.close();
        cout << "\n\n\t\t Record Inserted\n";
        return true;
    }

    bool editProduct(int pkey, const Product& newProduct) {
        fstream data(filename.c_str(), ios::in);
        fstream data1("database1.txt", ios::out | ios::app);
        int code;
        string name;
        float price, discount;
        bool found = false;

        if (!data || !data1) {
            cout << "\n\n\t\t Error: Could not open file!\n";
            return false;
        }

        while (data >> code) {
            getline(data, name, ' ');
            getline(data, name, ' ');
            data >> price >> discount;
            if (code == pkey) {
                data1 << newProduct.getCode() << " " << newProduct.getName() << " "
                      << newProduct.getPrice() << " " << newProduct.getDiscount() << endl;
                cout << "\n\n\t\t Record Edited\n";
                found = true;
            } else {
                data1 << code << " " << name << " " << price << " " << discount << endl;
            }
        }
        data.close();
        data1.close();

        remove(filename.c_str());
        rename("database1.txt", filename.c_str());
        if (!found) cout << "\n\n Record not found\n";
        return found;
    }

    bool deleteProduct(int pkey) {
        fstream data(filename.c_str(), ios::in);
        fstream data1("database1.txt", ios::out | ios::app);
        int code;
        string name;
        float price, discount;
        bool found = false;

        if (!data || !data1) {
            cout << "\n\n\t\t Error: Could not open file!\n";
            return false;
        }

        while (data >> code) {
            getline(data, name, ' ');
            getline(data, name, ' ');
            data >> price >> discount;
            if (code == pkey) {
                cout << "\n\n\t Product Deleted Successfully\n";
                found = true;
            } else {
                data1 << code << " " << name << " " << price << " " << discount << endl;
            }
        }
        data.close();
        data1.close();

        remove(filename.c_str());
        rename("database1.txt", filename.c_str());
        if (!found) cout << "\n\n Record not found\n";
        return found;
    }

    void listProducts() {
        fstream data(filename.c_str(), ios::in);
        if (!data) {
            cout << "\n\n\t File doesn't exist or is empty\n";
            return;
        }

        cout << "\n\n===========================================\n";
        cout << "PRODUCT NO\t \tNAME\t\tPRICE\n";
        cout << "===========================================\n";
        int code;
        string name;
        float price, discount;
        while (data >> code) {
            getline(data, name, ' ');
            getline(data, name, ' ');
            data >> price >> discount;
            cout <<"  "<< code << "\t\t\t" << name << "\t\t" << price << endl;
        }
        data.close();
    }

    bool getProduct(int code, Product& product) {
        fstream data(filename.c_str(), ios::in);
        if (!data) return false;

        int pcode;
        string name;
        float price, discount;
        while (data >> pcode) {
            getline(data, name, ' ');
            getline(data, name, ' ');
            data >> price >> discount;
            if (pcode == code) {
                product.setCode(pcode);
                product.setName(name);
                product.setPrice(price);
                product.setDiscount(discount);
                data.close();
                return true;
            }
        }
        data.close();
        return false;
    }
};

class Cart {
private:
    static const int MAX_ITEMS = 100;
    int product_codes[MAX_ITEMS];
    int quantities[MAX_ITEMS];
    int item_count;
    Database& db;

public:
    Cart(Database& database) : db(database), item_count(0) {
        for (int i = 0; i < MAX_ITEMS; ++i) {
            product_codes[i] = 0;
            quantities[i] = 0;
        }
    }

    bool addItem(int code, int quantity) {
        for (int i = 0; i < item_count; ++i) {
            if (product_codes[i] == code) {
                cout << "\n\n Duplicate Product Code. Please Try Again\n";
                return false;
            }
        }
        if (item_count >= MAX_ITEMS) {
            cout << "\n\n Cart is full!\n";
            return false;
        }
        product_codes[item_count] = code;
        quantities[item_count] = quantity;
        item_count++;
        return true;
    }

    void generateReceipt() {
        float total = 0.0;
        cout << "\n\n\t\t\t***************************************\n";
        cout << "\t\t\t*             RECEIPT                *\n";
        cout << "\t\t\t***************************************\n";
        cout << "PRODUCT NO\tPRODUCT NAME\tQUANTITY\tPRICE\tAMOUNT\tAMOUNT WITH DISCOUNT\n";

        for (int i = 0; i < item_count; ++i) {
            Product p;
            if (db.getProduct(product_codes[i], p)) {
                float amount = p.getPrice() * quantities[i];
                float discounted_amount = amount - (amount * p.getDiscount() / 100.0);
                total += discounted_amount;
                cout << p.getCode() << "\t\t" << p.getName() << "\t\t" << quantities[i] << "\t\t"
                     << p.getPrice() << "\t\t" << amount << "\t\t" << discounted_amount << endl;
            }
        }
        cout << "\n\n\t\t\t Total Amount: " << total << endl;
        item_count = 0;
    }
};

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

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
        cin >> choice;

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
