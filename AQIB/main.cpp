
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
