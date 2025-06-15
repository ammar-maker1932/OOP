//class start
class Cart {
    //private
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
        cout << "\n\n\t\t\t***************\n";
        cout << "\t\t\t*             RECEIPT                *\n";
        cout << "\t\t\t***************\n";
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