#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

// ================== CLASSES ==================

class Item {
private:
    std::string name;
    float quantity;
    float calories;
    std::string unit;
    std::map<std::string, int> macros{
        {"protein", 0},
        {"carbs", 0},
        {"fats", 0}};
public:
    Item() : name(""), quantity(0), calories(0), unit("") {}
    Item(std::string iName, float iQuantity, std::string iUnit, float iCalories)
        : name(iName), quantity(iQuantity), calories(iCalories), unit(iUnit) {}

    std::string getName() { return name; }
    void setName(std::string newName) { name = newName; }

    std::string getUnit() { return unit; }
    void setUnit(std::string newUnit) { unit = newUnit; }

    float getQuantity() { return quantity; }
    void setQuantity(float newQuantity) { quantity = newQuantity; }

    float getCalories() { return calories; }
    void setCalories(float newCalories) { calories = newCalories; }

    int getProtein() { return macros["protein"]; }
    void setProtein(int grams) { macros["protein"] = grams; }

    int getCarbs() { return macros["carbs"]; }
    void setCarbs(int grams) { macros["carbs"] = grams; }

    int getFats() { return macros["fats"]; }
    void setFats(int grams) { macros["fats"] = grams; }
};

class Recipe {
private:
    std::string name;
    std::vector<Item> recipeIngredients;
    std::string instructions;
    float rating;
    float calories;
public:
    Recipe() : name(""), instructions(""), rating(0), calories(0) {}
    Recipe(std::string rName, std::string rInstructions)
        : name(rName), instructions(rInstructions), rating(0), calories(0) {}
    Recipe(std::string rName, std::string rInstructions, float rRating)
        : name(rName), instructions(rInstructions), rating(rRating), calories(0) {}

    std::string getName() { return name; }
    void setName(std::string rName) { name = rName; }

    std::string getInstructions() { return instructions; }
    void setInstructions(std::string rInstructions) { instructions = rInstructions; }

    float getRating() { return rating; }
    void setRating(float newRating) {
        if (newRating >= 0 && newRating <= 5) rating = newRating;
    }

    float getCalories() { return calories; }
    void setCalories(float newCals) { calories = newCals; }

    void addIngredient(Item newItem) {
        recipeIngredients.push_back(newItem);
    }

    std::vector<Item>& getIngredients() {
        return recipeIngredients;
    }
};

// ================== GLOBALS ==================

std::vector<Item> itemList;        // per-user pantry
std::vector<Recipe> recipeList;    // per-user recipes
std::vector<Recipe> appRecipeList; // shared/app recipes

const std::string USER_DIR_FILE    = "UserDirectory.txt";
const std::string USER_FOLDER      = "users";
const std::string APP_RECIPES_FILE = "AppRecipes.txt";

std::vector<std::string> g_usernames;
std::vector<std::string> g_passwords;
std::vector<std::string> g_roles;

// ================== UTILS ==================

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> parts;
    std::string current = "";
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == delim) {
            parts.push_back(current);
            current = "";
        } else {
            current += s[i];
        }
    }
    parts.push_back(current);
    return parts;
}

std::string getUserFile(std::string username) {
    fs::create_directories(USER_FOLDER);
    return USER_FOLDER + std::string("/") + username + ".txt";
}

// create or return pantry item
Item* getOrCreatePantryItem(const std::string& name) {
    for (int i = 0; i < (int)itemList.size(); i++) {
        if (itemList[i].getName() == name) {
            return &itemList[i];
        }
    }
    Item newItem;
    newItem.setName(name);
    newItem.setQuantity(0);
    newItem.setUnit("");
    newItem.setCalories(0);
    itemList.push_back(newItem);
    return &itemList.back();
}

// ================== USER DIRECTORY ==================

void loadUserDirectory(std::vector<std::string>& usernames,
                       std::vector<std::string>& passwords,
                       std::vector<std::string>& roles) {
    usernames.clear();
    passwords.clear();
    roles.clear();

    std::ifstream in(USER_DIR_FILE);
    if (!in.is_open()) return;

    std::string name, pass, role;
    while (true) {
        in >> name;
        if (!in.good()) break;
        in >> pass;
        if (!in.good()) break;
        in >> role;
        if (!in.good()) role = "user";
        usernames.push_back(name);
        passwords.push_back(pass);
        roles.push_back(role);
    }
    in.close();
}

void saveUserDirectory(const std::vector<std::string>& usernames,
                       const std::vector<std::string>& passwords,
                       const std::vector<std::string>& roles) {
    std::ofstream out(USER_DIR_FILE, std::ios::trunc);
    for (int i = 0; i < (int)usernames.size(); i++) {
        out << usernames[i] << " " << passwords[i] << " " << roles[i] << "\n";
    }
    out.close();
}

int findUserIndex(const std::vector<std::string>& usernames,
                  const std::string& username) {
    for (int i = 0; i < (int)usernames.size(); i++) {
        if (usernames[i] == username) return i;
    }
    return -1;
}

// hide owner from normal list
void showAllUsers(const std::vector<std::string>& usernames,
                  const std::vector<std::string>& roles) {
    bool any = false;
    std::cout << "Registered users:\n";
    for (int i = 0; i < (int)usernames.size(); i++) {
        if (roles[i] == "owner") {
            continue;
        }
        any = true;
        std::cout << " - " << usernames[i] << " [" << roles[i] << "]\n";
    }
    if (!any) {
        std::cout << " - (none)\n";
    }
}

// ================== PER-USER SAVE / LOAD ==================

void saveData(std::string username) {
    std::string file = getUserFile(username);
    std::ofstream out(file);
    if (!out.is_open()) {
        std::cout << "Error opening file for writing.\n";
        return;
    }

    out << "[ITEMS]\n";
    for (int i = 0; i < (int)itemList.size(); i++) {
        Item it = itemList[i];
        out << it.getName() << ","
            << it.getQuantity() << ","
            << it.getUnit() << ","
            << it.getCalories() << ","
            << it.getProtein() << ","
            << it.getCarbs() << ","
            << it.getFats() << "\n";
    }

    out << "[RECIPES]\n";
    for (int i = 0; i < (int)recipeList.size(); i++) {
        Recipe r = recipeList[i];
        out << r.getName() << "|" << r.getRating() << "|" << r.getInstructions() << "\n";
        out << "#INGREDIENTS\n";
        std::vector<Item>& ings = r.getIngredients();
        for (int j = 0; j < (int)ings.size(); j++) {
            Item ing = ings[j];
            out << ing.getName() << ","
                << ing.getQuantity() << ","
                << ing.getUnit() << ","
                << ing.getCalories() << ","
                << ing.getProtein() << ","
                << ing.getCarbs() << ","
                << ing.getFats() << "\n";
        }
        out << "#END_INGREDIENTS\n";
    }

    out.close();
    std::cout << "Data saved for " << username << "\n";
}

void loadData(std::string username) {
    std::string file = getUserFile(username);
    std::ifstream in(file);
    itemList.clear();
    recipeList.clear();
    if (!in.is_open()) {
        std::cout << "No data for " << username << ", starting fresh.\n";
        return;
    }

    std::string line;
    enum Section { NONE, ITEMS, RECIPES };
    Section current = NONE;
    Recipe currentRecipe;
    bool readingIngredients = false;

    while (std::getline(in, line)) {
        if (line == "[ITEMS]") {
            current = ITEMS;
            continue;
        }
        if (line == "[RECIPES]") {
            current = RECIPES;
            continue;
        }
        if (line == "#INGREDIENTS") {
            readingIngredients = true;
            continue;
        }
        if (line == "#END_INGREDIENTS") {
            recipeList.push_back(currentRecipe);
            readingIngredients = false;
            continue;
        }

        if (current == ITEMS && !line.empty()) {
            std::vector<std::string> parts = split(line, ',');
            if (parts.size() >= 7) {
                Item it(parts[0], std::stof(parts[1]), parts[2], std::stof(parts[3]));
                it.setProtein(std::stoi(parts[4]));
                it.setCarbs(std::stoi(parts[5]));
                it.setFats(std::stoi(parts[6]));
                itemList.push_back(it);
            }
        } else if (current == RECIPES && !line.empty() && !readingIngredients) {
            std::vector<std::string> parts = split(line, '|');
            if (parts.size() >= 3) {
                currentRecipe = Recipe(parts[0], parts[2], std::stof(parts[1]));
            }
        } else if (readingIngredients) {
            std::vector<std::string> parts = split(line, ',');
            if (parts.size() >= 7) {
                Item ing(parts[0], std::stof(parts[1]), parts[2], std::stof(parts[3]));
                ing.setProtein(std::stoi(parts[4]));
                ing.setCarbs(std::stoi(parts[5]));
                ing.setFats(std::stoi(parts[6]));
                currentRecipe.addIngredient(ing);
            }
        }
    }

    in.close();
    std::cout << "Data loaded for " << username << "\n";
}

// ================== APP RECIPES SAVE / LOAD (with nutrition) ==================

void loadAppRecipes() {
    appRecipeList.clear();
    std::ifstream in(APP_RECIPES_FILE);
    if (!in.is_open()) {
        return;
    }

    std::string line;
    Recipe currentRecipe;
    bool readingIngredients = false;

    while (std::getline(in, line)) {
        if (line == "[RECIPES]") {
            continue;
        }
        if (line == "#INGREDIENTS") {
            readingIngredients = true;
            continue;
        }
        if (line == "#END_INGREDIENTS") {
            appRecipeList.push_back(currentRecipe);
            readingIngredients = false;
            continue;
        }

        if (!readingIngredients && !line.empty()) {
            std::vector<std::string> parts = split(line, '|');
            if (parts.size() >= 3) {
                currentRecipe = Recipe(parts[0], parts[2], std::stof(parts[1]));
            }
        } else if (readingIngredients) {
            std::vector<std::string> parts = split(line, ',');
            if (parts.size() >= 7) {
                Item ing(parts[0], std::stof(parts[1]), parts[2], std::stof(parts[3]));
                ing.setProtein(std::stoi(parts[4]));
                ing.setCarbs(std::stoi(parts[5]));
                ing.setFats(std::stoi(parts[6]));
                currentRecipe.addIngredient(ing);
            }
        }
    }

    in.close();
}

void saveAppRecipes() {
    std::ofstream out(APP_RECIPES_FILE, std::ios::trunc);
    if (!out.is_open()) return;

    out << "[RECIPES]\n";
    for (int i = 0; i < (int)appRecipeList.size(); i++) {
        Recipe r = appRecipeList[i];
        out << r.getName() << "|" << r.getRating() << "|" << r.getInstructions() << "\n";
        out << "#INGREDIENTS\n";
        std::vector<Item>& ings = r.getIngredients();
        for (int j = 0; j < (int)ings.size(); j++) {
            Item ing = ings[j];
            out << ing.getName() << ","
                << ing.getQuantity() << ","
                << ing.getUnit() << ","
                << ing.getCalories() << ","
                << ing.getProtein() << ","
                << ing.getCarbs() << ","
                << ing.getFats() << "\n";
        }
        out << "#END_INGREDIENTS\n";
    }

    out.close();
}

// ================== LOOKUPS ==================

Item* findItemByName(const std::string& name) {
    for (int i = 0; i < (int)itemList.size(); i++) {
        if (itemList[i].getName() == name) return &itemList[i];
    }
    return NULL;
}

Recipe* findRecipeByName(const std::string& name) {
    for (int i = 0; i < (int)recipeList.size(); i++) {
        if (recipeList[i].getName() == name) return &recipeList[i];
    }
    return NULL;
}

Recipe* findAppRecipeByName(const std::string& name) {
    for (int i = 0; i < (int)appRecipeList.size(); i++) {
        if (appRecipeList[i].getName() == name) return &appRecipeList[i];
    }
    return NULL;
}

// ================== USER FEATURES ==================

void viewAllIngredients() {
    if (itemList.empty()) {
        std::cout << "No ingredients.\n";
        return;
    }
    std::cout << "Ingredients:\n";
    for (int i = 0; i < (int)itemList.size(); i++) {
        Item it = itemList[i];
        std::cout << " - " << it.getName()
                  << " | Quantity: " << it.getQuantity() << it.getUnit()
                  << " | Macros Per 100g/ml: ("
                  << "Calories: " << it.getCalories()
                  << " Protein: " << it.getProtein()
                  << " Carbs: " << it.getCarbs()
                  << " Fats: " << it.getFats()
                  << ")\n";
    }
}

void listAllRecipes(bool isAdmin, bool showAppRecipes) {
    if (recipeList.empty() && appRecipeList.empty()) {
        std::cout << "No recipes available.\n";
        return;
    }

    std::cout << "\n=== My Recipes ===\n";
    if (recipeList.empty()) {
        std::cout << "(none)\n";
    } else {
        for (auto &r : recipeList) {
            std::cout << "- " << r.getName();

            // Show rating if any
            if (r.getRating() > 0)
                std::cout << " (" << r.getRating() << "/5)";

            // Show calories if ingredients exist
            if (r.getCalories() > 0)
                std::cout << " - " << r.getCalories() << " cal";

            std::cout << "\n";
        }
    }
    if(showAppRecipes) {
        std::cout << "\n=== App Recipes ===\n";
        if (appRecipeList.empty()) {
            std::cout << "(none)\n";
        } else {
            for (auto &r : appRecipeList) {
                std::cout << "- " << r.getName();

                if (r.getRating() > 0)
                    std::cout << " (" << r.getRating() << "/5)";

                if (r.getCalories() > 0)
                    std::cout << " - " << r.getCalories() << " cal";

                std::cout << "\n";
            }
        }
    }

    std::cout << "\n";
}


void removeRecipe() {
    if (recipeList.empty()) {
        std::cout << "You have no recipes to remove.\n";
        return;
    }

    std::cout << "Your recipes:\n";
    for (int i = 0; i < (int)recipeList.size(); i++) {
        std::cout << i + 1 << ") " << recipeList[i].getName() << "\n";
    }

    int choice;
    std::cout << "Enter the number of the recipe to remove (0 to cancel): ";
    std::cin >> choice;
    std::cin.ignore();

    if (choice <= 0 || choice > (int)recipeList.size()) {
        std::cout << "Cancelled.\n";
        return;
    }

    std::string removedName = recipeList[choice - 1].getName();
    recipeList.erase(recipeList.begin() + (choice - 1));
    std::cout << "Recipe \"" << removedName << "\" has been removed.\n";
}

void listRecipeIngredients(std::string recipeName) {
    std::cout << "Ingredients:\n";

    Recipe* r = findRecipeByName(recipeName);
    std::vector<Item>& ings = r->getIngredients();

    float recipeTotalCals   = 0.0f;
    int   recipeTotalProt   = 0;
    int   recipeTotalCarbs  = 0;
    int   recipeTotalFats   = 0;

    for (int i = 0; i < (int)ings.size(); i++) {
        Item ing = ings[i];

        float qty        = ing.getQuantity();
        float calsPer    = ing.getCalories();
        std::string unit = ing.getUnit();

        int protPer = ing.getProtein();
        int carbPer = ing.getCarbs();
        int fatPer  = ing.getFats();

        float thisCals = 0.0f;
        float thisProt = 0.0f;
        float thisCarb = 0.0f;
        float thisFat  = 0.0f;

        bool per100 = (unit == "g" || unit == "ml" || unit == "gram" || unit == "grams");

        if (per100) {
            float scale = qty / 100.0f;
            thisCals = scale * calsPer;
            thisProt = scale * protPer;
            thisCarb = scale * carbPer;
            thisFat  = scale * fatPer;
        } else {
            thisCals = qty * calsPer;
            thisProt = qty * protPer;
            thisCarb = qty * carbPer;
            thisFat  = qty * fatPer;
        }

        recipeTotalCals  += thisCals;
        recipeTotalProt  += (int)thisProt;
        recipeTotalCarbs += (int)thisCarb;
        recipeTotalFats  += (int)thisFat;

        std::cout << " - " << ing.getName()
                  << " x " << qty;
        if (unit != "") {
            std::cout << unit;
        }
        std::cout << " (Calories: " << thisCals
                  << ", Protein: " << thisProt
                  << "g, Carbs: " << thisCarb
                  << "g, Fats: " << thisFat
                  << "g)\n";
    }
}

void viewRecipeDetails() {
    // 1) show all recipes (user + app)
    std::cout << "\nYour recipes:\n";
    if (recipeList.empty()) {
        std::cout << " - (none)\n";
    } else {
        for (int i = 0; i < (int)recipeList.size(); i++) {
            std::cout << " - " << recipeList[i].getName() << "\n";
        }
    }

    std::cout << "App recipes:\n";
    if (appRecipeList.empty()) {
        std::cout << " - (none)\n";
    } else {
        for (int i = 0; i < (int)appRecipeList.size(); i++) {
            std::cout << " - " << appRecipeList[i].getName() << " (app)\n";
        }
    }

    // 2) ask which one
    std::string rname;
    std::cout << "\nEnter recipe name to view: ";
    std::getline(std::cin, rname);

    // 3) find it (user first, then app)
    Recipe* r = findRecipeByName(rname);
    bool isApp = false;
    if (r == NULL) {
        r = findAppRecipeByName(rname);
        isApp = true;
    }

    if (r == NULL) {
        std::cout << "Recipe not found.\n";
        return;
    }

    std::cout << "\nRecipe: " << r->getName() << (isApp ? " (app recipe)" : "") << "\n";
    std::cout << "Rating: " << r->getRating() << "\n";
    std::cout << "Instructions: " << r->getInstructions() << "\n";

    listRecipeIngredients(rname);

    float recipeTotalCals   = 0.0f;
    int   recipeTotalProt   = 0;
    int   recipeTotalCarbs  = 0;
    int   recipeTotalFats   = 0;

    std::cout << "\nTotals for this recipe:\n";
    std::cout << "Calories: " << recipeTotalCals << "\n";
    std::cout << "Protein: "  << recipeTotalProt  << " g\n";
    std::cout << "Carbs: "    << recipeTotalCarbs << " g\n";
    std::cout << "Fats: "     << recipeTotalFats  << " g\n";
}

void addNewIngredient() {
    std::string name, unit;
    float qty, cals;
    int p, c, f;

    std::cout << "Ingredient name: ";
    std::getline(std::cin, name);
    std::cout << "Quantity: ";
    std::cin >> qty;
    std::cin.ignore();
    std::cout << "Unit: ";
    std::getline(std::cin, unit);
    std::cout << "Calories per 100g: ";
    std::cin >> cals;
    std::cout << "Protein per 100g: ";
    std::cin >> p;
    std::cout << "Carbs per 100g: ";
    std::cin >> c;
    std::cout << "Fats per 100g: ";
    std::cin >> f;
    std::cin.ignore();

    Item it(name, qty, unit, cals);
    it.setProtein(p);
    it.setCarbs(c);
    it.setFats(f);
    itemList.push_back(it);
    std::cout << "Ingredient added.\n";
}

void removeIngredient() {
    if (itemList.empty()) {
        std::cout << "You have no ingredients to remove.\n";
        return;
    }

    viewAllIngredients();

    int choice;
    std::cout << "Enter the number of the ingredient to remove (0 to cancel): ";
    std::cin >> choice;
    std::cin.ignore();

    if (choice <= 0 || choice > (int)itemList.size()) {
        std::cout << "Cancelled.\n";
        return;
    }

    std::string removedName = itemList[choice - 1].getName();
    itemList.erase(itemList.begin() + (choice - 1));
    std::cout << "Ingredient \"" << removedName << "\" has been removed.\n";
}

void modifyIngredient() {
    std::string name;
    std::cout << "\nIngredients in your pantry:\n";
    if (itemList.empty()) {
        std::cout << " - (none)\n";
    } else {
        for (int i = 0; i < (int)itemList.size(); i++) {
            std::cout << " - " << itemList[i].getName() << "\n";
        }
    }
    std::cout << "Enter ingredient name to modify: ";
    std::getline(std::cin, name);
    if (name.empty()) {
        std::cout << "Cancelled.\n";
        return;
    }
    Item* it = findItemByName(name);
    if (it == NULL) {
        std::cout << "Not found.\n";
        return;
    }
    int choice = -1;
    while (choice != 0) {
        std::cout << "\nIngredient Modification Menu:\n";
        std::cout << "1) Rename\n";
        std::cout << "2) Change quantity\n";
        std::cout << "3) Change unit\n";
        std::cout << "4) Change calories\n";
        std::cout << "5) Change macros\n";
        std::cout << "0) Done\n";
        std::cout << "Choice: ";
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 1) {
            std::string newName;
            std::cout << "New name: ";
            std::getline(std::cin, newName);
            it->setName(newName);
        } else if (choice == 2) {
            float q;
            std::cout << "New quantity: ";
            std::cin >> q;
            std::cin.ignore();
            it->setQuantity(q);
        } else if (choice == 3) {
            std::string u;
            std::cout << "New unit: ";
            std::getline(std::cin, u);
            it->setUnit(u);
        } else if (choice == 4) {
            float cals;
            std::cout << "New calories: ";
            std::cin >> cals;
            std::cin.ignore();
            it->setCalories(cals);
        } else if (choice == 5) {
            int p, c, f;
            std::cout << "Protein: ";
            std::cin >> p;
            std::cout << "Carbs: ";
            std::cin >> c;
            std::cout << "Fats: ";
            std::cin >> f;
            std::cin.ignore();
            it->setProtein(p);
            it->setCarbs(c);
            it->setFats(f);
        }
    }
}

void createRecipe() {
    std::string name, instr;
    float rating;
    std::cout << "Recipe name: ";
    std::getline(std::cin, name);
    if(name.empty()) {
        std::cout << "Recipe creation cancelled.\n";
        return;
    }
    std::cout << "Instructions: ";
    std::getline(std::cin, instr);
    std::cout << "Rating (0-5): ";
    std::cin >> rating;
    std::cin.ignore();

    Recipe r(name, instr, rating);

    while (true) {
        std::cout << "Add ingredient? (1=yes, 0=no): ";
        int add;
        std::cin >> add;
        std::cin.ignore();
        if (!add) break;

        std::string iname;
        std::cout << "Ingredient name: ";
        std::getline(std::cin, iname);
        Item* base = findItemByName(iname);
        if (base == NULL) {
            std::cout << "Ingredient not found in pantry.\n";
            std::cout << "Create new ingredient? (1=yes, 0=no): ";
            int createNew;
            std::cin >> createNew;
            std::cin.ignore();
            if (createNew) {
                addNewIngredient();
                base = findItemByName(iname);
                if (base == NULL) {
                    std::cout << "Failed to create ingredient.\n";
                    continue;
                } else {
                    std::cout << "Ingredient created and added to pantry.\n";
                }
            } else 
                continue;
        }
        float q;
        std::cout << "Quantity to use (" << base->getUnit() << "): ";
        std::cin >> q;
        std::cin.ignore();
        Item copy = *base;
        copy.setQuantity(q);
        r.addIngredient(copy);
    }

    recipeList.push_back(r);
    std::cout << "Recipe created.\n";
}

void recalculateRecipeCalories(Recipe &r) {
    float total = 0.0f;
    std::vector<Item> &ings = r.getIngredients();

    for (int i = 0; i < (int)ings.size(); i++) {
        Item &ing = ings[i];

        float qty     = ing.getQuantity();
        float calsPer = ing.getCalories();
        std::string unit = ing.getUnit();

        bool per100 = (unit == "g" || unit == "ml" || unit == "gram" || unit == "grams");

        if (per100) {
            total += (qty / 100.0f) * calsPer;
        } else {
            total += qty * calsPer;
        }
    }

    r.setCalories(total);
}

void modifyRecipe(bool isAdmin) {
    std::cout << "\nYour recipes:\n";
    if (recipeList.empty()) {
        std::cout << " - (none)\n";
    } else {
        for (int i = 0; i < (int)recipeList.size(); i++) {
            std::cout << " - " << recipeList[i].getName() << "\n";
        }
    }

    std::cout << "App recipes:\n";
    if (appRecipeList.empty()) {
        std::cout << " - (none)\n";
    } else {
        for (int i = 0; i < (int)appRecipeList.size(); i++) {
            std::cout << " - " << appRecipeList[i].getName() << " (app)\n";
        }
    }

    std::cout << "\nEnter recipe name to modify: ";
    std::string rname;
    std::getline(std::cin, rname);

    Recipe *r = findRecipeByName(rname);
    bool isApp = false;

    if (r == NULL) {
        r = findAppRecipeByName(rname);
        if (r != NULL) {
            isApp = true;
        }
    }

    if (r == NULL) {
        std::cout << "Recipe not found.\n";
        return;
    }

    if (isApp) {
        std::cout << "You can only modify your own recipes.\n";
        std::cout << "Would you like to create a copy in your own recipes to modify? (1=yes, 0=no): ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        if (choice) {
            Recipe copy = *r;
            recipeList.push_back(copy);
            std::cout << "Recipe copied to your own recipes as \"" << copy.getName() << "\".\n";
        }
    }

    int choice = -1;
    while (choice != 0) {
        // show the current recipes ingredients
        listRecipeIngredients(r->getName());
        std::cout << "\nModify Recipe: " << r->getName()
                  << (isApp ? " (app recipe)" : "") << "\n";
        std::cout << "1) Rename recipe\n";
        std::cout << "2) Change instructions\n";
        std::cout << "3) Change rating\n";
        std::cout << "4) Add ingredient\n";
        std::cout << "5) Change ingredient quantity\n";
        std::cout << "6) Remove ingredient\n";
        std::cout << "0) Done\n";
        std::cout << "Choice: ";
        std::cin >> choice;
        std::cin.ignore();
        std::cout << "\n";

        if (choice == 1) {
            std::cout << "Current name: " << r->getName() << "\n";
            std::string newName;
            std::cout << "New recipe name: ";
            std::getline(std::cin, newName);
            r->setName(newName);
        }
        else if (choice == 2) {
            std::cout << "Current instructions: " << r->getInstructions() << "\n";
            std::string newInstr;
            std::cout << "New instructions: ";
            std::getline(std::cin, newInstr);
            r->setInstructions(newInstr);
        }
        else if (choice == 3) {
            std::cout << "Current rating: " << r->getRating() << "\n";
            float newRating;
            std::cout << "New rating (0-5): ";
            std::cin >> newRating;
            std::cin.ignore();
            r->setRating(newRating);
        }
        else if (choice == 4) {
            std::string iname, unit;
            float q, cals;
            int p, c, f;

            std::cout << "Ingredient name: ";
            std::getline(std::cin, iname);
            std::cout << "Quantity: ";
            std::cin >> q;
            std::cin.ignore();
            std::cout << "Unit (g, ml, cup, tbsp, etc): ";
            std::getline(std::cin, unit);
            std::cout << "\nPut macros per 100g/ml or per unit depending on the unit used.\n";
            std::cout << "Add nutrition info for this ingredient:\n";
            std::cout << "Calories: ";
            std::cin >> cals;
            std::cout << "Protein: ";
            std::cin >> p;
            std::cout << "Carbs: ";
            std::cin >> c;
            std::cout << "Fats: ";
            std::cin >> f;
            std::cin.ignore();

            Item ing(iname, q, unit, cals);
            ing.setProtein(p);
            ing.setCarbs(c);
            ing.setFats(f);
            r->addIngredient(ing);

            recalculateRecipeCalories(*r);
        }
        else if (choice == 5) {
            std::vector<Item> &ings = r->getIngredients();
            if (ings.empty()) {
                std::cout << "This recipe has no ingredients.\n";
            } else {
                listRecipeIngredients(r->getName());
                int idx;
                std::cout << "Select ingredient to change (0 to cancel): ";
                std::cin >> idx;
                std::cin.ignore();
                if (idx > 0 && idx <= (int)ings.size()) {
                    float newQty;
                    std::cout << "New quantity: ";
                    std::cin >> newQty;
                    std::cin.ignore();
                    ings[idx - 1].setQuantity(newQty);
                    recalculateRecipeCalories(*r);
                } else {
                    std::cout << "Cancelled.\n";
                }
            }
        }
        else if (choice == 6) {
            std::vector<Item> &ings = r->getIngredients();
            if (ings.empty()) {
                std::cout << "This recipe has no ingredients to remove.\n";
            } else {
                listRecipeIngredients(r->getName());
                int idx;
                std::cout << "Select ingredient to remove (0 to cancel): ";
                std::cin >> idx;
                std::cin.ignore();
                if (idx > 0 && idx <= (int)ings.size()) {
                    std::string removedName = ings[idx - 1].getName();
                    ings.erase(ings.begin() + (idx - 1));
                    recalculateRecipeCalories(*r);
                    std::cout << "Removed ingredient \"" << removedName << "\".\n";
                } else {
                    std::cout << "Cancelled.\n";
                }
            }
        }
    }

    if (isApp && isAdmin) {
        saveAppRecipes();
        std::cout << "App recipes updated on disk.\n";
    }
}

void makeRecipe() {
    listAllRecipes(false, true);
    std::string rname;
    std::cout << "Enter recipe name to make: ";
    std::getline(std::cin, rname);

    Recipe* r = findRecipeByName(rname);
    if (r == NULL) {
        r = findAppRecipeByName(rname);
    }
    if (r == NULL) {
        std::cout << "Recipe not found.\n";
        return;
    }

    std::vector<Item>& ings = r->getIngredients();
    for (int i = 0; i < (int)ings.size(); i++) {
        Item need = ings[i];
        Item* pantry = findItemByName(need.getName());
        if (pantry == NULL) {
            std::cout << "Missing ingredient: " << need.getName() << "\n";
            return;
        }
        if (pantry->getQuantity() < need.getQuantity()) {
            std::cout << "Not enough of: " << need.getName() << "\n";
            return;
        }
    }

    for (int i = 0; i < (int)ings.size(); i++) {
        Item need = ings[i];
        Item* pantry = findItemByName(need.getName());
        pantry->setQuantity(pantry->getQuantity() - need.getQuantity());
    }

    std::cout << "Recipe made. Pantry updated.\n";
}

void suggestRecipes() {
    bool foundAny = false;

    for (int i = 0; i < (int)recipeList.size(); i++) {
        Recipe r = recipeList[i];
        std::vector<Item>& ings = r.getIngredients();
        bool canMake = true;
        for (int j = 0; j < (int)ings.size(); j++) {
            Item need = ings[j];
            Item* pantryItem = findItemByName(need.getName());
            if (pantryItem == NULL || pantryItem->getQuantity() < need.getQuantity()) {
                canMake = false;
                break;
            }
        }
        if (canMake) {
            if (!foundAny) {
                std::cout << "Recipes you can make right now:\n";
                foundAny = true;
            }
            std::cout << " - " << r.getName() << " (your recipe)\n";
        }
    }

    for (int i = 0; i < (int)appRecipeList.size(); i++) {
        Recipe r = appRecipeList[i];
        std::vector<Item>& ings = r.getIngredients();
        bool canMake = true;
        for (int j = 0; j < (int)ings.size(); j++) {
            Item need = ings[j];
            Item* pantryItem = findItemByName(need.getName());
            if (pantryItem == NULL || pantryItem->getQuantity() < need.getQuantity()) {
                canMake = false;
                break;
            }
        }
        if (canMake) {
            if (!foundAny) {
                std::cout << "Recipes you can make right now:\n";
                foundAny = true;
            }
            std::cout << " - " << r.getName() << " (app recipe)\n";
        }
    }

    if (!foundAny) {
        std::cout << "No recipes can be made with your current ingredients.\n";
    }
}

// shopping list + buy
void generateShoppingList() {
    listAllRecipes(false, true);
    std::string rname;
    std::cout << "Enter recipe name to generate list for: ";
    std::getline(std::cin, rname);

    Recipe* r = findRecipeByName(rname);
    if (r == NULL) {
        r = findAppRecipeByName(rname);
    }
    if (r == NULL) {
        std::cout << "Recipe not found.\n";
        return;
    }

    struct MissingItem {
        std::string name;
        float amount;
        std::string unit;
    };
    std::vector<MissingItem> missing;

    std::cout << "Shopping list for " << rname << ":\n";
    std::vector<Item>& ings = r->getIngredients();
    for (int i = 0; i < (int)ings.size(); i++) {
        Item need = ings[i];
        Item* pantry = findItemByName(need.getName());
        if (pantry == NULL) {
            std::cout << " - " << need.getName() << ": need "
                      << need.getQuantity() << " " << need.getUnit() << "\n";
            MissingItem m;
            m.name = need.getName();
            m.amount = need.getQuantity();
            m.unit = need.getUnit();
            missing.push_back(m);
        } else {
            float diff = need.getQuantity() - pantry->getQuantity();
            if (diff > 0) {
                std::cout << " - " << need.getName() << ": need "
                          << diff << " " << need.getUnit() << "\n";
                MissingItem m;
                m.name = need.getName();
                m.amount = diff;
                m.unit = need.getUnit();
                missing.push_back(m);
            }
        }
    }

    if (missing.empty()) {
        std::cout << "You already have everything.\n";
        return;
    }

    int buy = 0;
    std::cout << "\nDo you want to buy/add these to your pantry now? (1=yes, 0=no): ";
    std::cin >> buy;
    std::cin.ignore();

    if (!buy) {
        return;
    }

    for (int i = 0; i < (int)missing.size(); i++) {
        MissingItem m = missing[i];
        Item* pantryItem = getOrCreatePantryItem(m.name);
        if (pantryItem->getUnit() == "" && m.unit != "") {
            pantryItem->setUnit(m.unit);
        }
        pantryItem->setQuantity(pantryItem->getQuantity() + m.amount);
    }

    std::cout << "Items added to your pantry.\n";
}

void cloneAppRecipeToUser(std::string rname) {
    listAllRecipes(false, true);
    if(rname == "" || rname.empty()) {
        std::cout << "Enter app recipe name to clone: ";
        std::getline(std::cin, rname);
    }

    Recipe* appR = findAppRecipeByName(rname);
    if (appR == NULL) {
        std::cout << "App recipe not found.\n";
        return;
    }


    Recipe userCopy(appR->getName(), appR->getInstructions(), appR->getRating());
    std::vector<Item>& src = appR->getIngredients();

    for (int i = 0; i < (int)src.size(); i++) {
        Item srcIng = src[i];

        // Rebuild the item and copy macros explicitly
        Item ing(
            srcIng.getName(),
            srcIng.getQuantity(),
            srcIng.getUnit(),
            srcIng.getCalories()
        );

        ing.setProtein(srcIng.getProtein());
        ing.setCarbs(srcIng.getCarbs());
        ing.setFats(srcIng.getFats());

        userCopy.addIngredient(ing);
    }
    for(int i = 0; i < recipeList.size(); i++) {
        if(recipeList[i].getName() == userCopy.getName()) {
            std::cout << "You already have a recipe with this name. Do you want to overwrite it? (1=yes, 0=no): ";
            int choice;
            std::cin >> choice;
            std::cin.ignore();
            if(choice) {
                recipeList[i] = userCopy;
                std::cout << "Recipe overwritten in your recipes. You can now modify it.\n";
                return;
            } else {
                std::cout << "Do you want to make a copy with a different name? (1=yes, 0=no): ";
                std::cin >> choice;
                std::cin.ignore();
                if(choice) {
                    std::cout << "Enter new recipe name: ";
                    std::string newName;
                    std::getline(std::cin, newName);
                    userCopy.setName(newName);
                } else {
                    std::cout << "Clone cancelled.\n";
                    return;
                }
            }
        }
    }
    recipeList.push_back(userCopy);
    std::cout << "Recipe cloned into your recipes. You can now modify it.\n";
}

// ================== ADMIN / OWNER FEATURES ==================

void createOwnerRecipe() {
    std::string name, instr;
    float rating;
    std::cout << "[ADMIN] Recipe name: ";
    std::getline(std::cin, name);
    std::cout << "[ADMIN] Instructions: ";
    std::getline(std::cin, instr);
    std::cout << "[ADMIN] Rating (0-5): ";
    std::cin >> rating;
    std::cin.ignore();

    Recipe r(name, instr, rating);

    while (true) {
        std::cout << "[ADMIN] Add ingredient? (1=yes, 0=no): ";
        int add;
        std::cin >> add;
        std::cin.ignore();
        if (!add) break;

        std::string iname;
        std::cout << "Ingredient name: ";
        std::getline(std::cin, iname);

        float q;
        std::cout << "Quantity: ";
        std::cin >> q;
        std::cin.ignore();

        std::string unit;
        std::cout << "Unit (g, cup, tbsp, etc): ";
        std::getline(std::cin, unit);

        float cals;
        std::cout << "Calories per 100g: ";
        std::cin >> cals;

        int p, c, f;
        std::cout << "Protein per 100g: ";
        std::cin >> p;
        std::cout << "Carbs per 100g: ";
        std::cin >> c;
        std::cout << "Fats per 100g: ";
        std::cin >> f;
        std::cin.ignore();

        Item ing(iname, q, unit, cals);
        ing.setProtein(p);
        ing.setCarbs(c);
        ing.setFats(f);

        r.addIngredient(ing);
    }

    appRecipeList.push_back(r);
    saveAppRecipes();
    std::cout << "App recipe added.\n";
}

void listAppRecipes() {
    std::cout << "[ADMIN] App recipes:\n";
    for (int i = 0; i < (int)appRecipeList.size(); i++) {
        std::cout << " - " << appRecipeList[i].getName() << "\n";
    }
}

void ownerListUsers() {
    showAllUsers(g_usernames, g_roles);
}

void ownerPromoteUser() {
    std::string uname;
    std::cout << "Enter username to promote to admin: ";
    std::getline(std::cin, uname);
    int idx = findUserIndex(g_usernames, uname);
    if (idx == -1) {
        std::cout << "User not found.\n";
        return;
    }
    g_roles[idx] = "admin";
    saveUserDirectory(g_usernames, g_passwords, g_roles);
    std::cout << "User " << uname << " promoted to admin.\n";
}

void ownerDemoteUser() {
    std::string uname;
    std::cout << "Enter username to demote to user: ";
    std::getline(std::cin, uname);
    int idx = findUserIndex(g_usernames, uname);
    if (idx == -1) {
        std::cout << "User not found.\n";
        return;
    }
    if (g_roles[idx] == "owner") {
        std::cout << "Cannot demote an owner here.\n";
        return;
    }
    g_roles[idx] = "user";
    saveUserDirectory(g_usernames, g_passwords, g_roles);
    std::cout << "User " << uname << " demoted to user.\n";
}

void ownerCreateUser() {
    std::string uname, pw;
    std::cout << "New username: ";
    std::getline(std::cin, uname);
    std::cout << "Password: ";
    std::getline(std::cin, pw);
    g_usernames.push_back(uname);
    g_passwords.push_back(pw);
    g_roles.push_back("user");
    saveUserDirectory(g_usernames, g_passwords, g_roles);
    std::cout << "User created.\n";
}

void deleteUserAccount() {
    ownerListUsers();
    std::string uname;
    std::cout << "\nEnter username to delete: ";
    std::getline(std::cin, uname);
    int idx = findUserIndex(g_usernames, uname);
    if (idx == -1) {
        std::cout << "User not found.\n";
        return;
    }
    if (g_roles[idx] == "owner") {
        std::cout << "Cannot delete the owner account.\n";
        return;
    }
    g_usernames.erase(g_usernames.begin() + idx);
    g_passwords.erase(g_passwords.begin() + idx);
    g_roles.erase(g_roles.begin() + idx);
    saveUserDirectory(g_usernames, g_passwords, g_roles);

    // Also delete user data files
    std::string userFile = USER_FOLDER + "/" + uname + ".txt";
    if (fs::exists(userFile)) {
        fs::remove(userFile);
    }

    std::cout << "User account deleted.\n";
}

// ================== MENUS ==================

void displayMenu(bool isAdmin, bool isOwner) {
    std::cout << "\nCooking Helper Menu:\n";
    std::cout << "1) View all recipes\n";
    std::cout << "2) View all ingredients\n";
    std::cout << "3) Create a new recipe\n";
    std::cout << "4) Modify an existing recipe\n";
    std::cout << "5) Modify an existing ingredient\n";
    std::cout << "6) Add a new ingredient\n";
    std::cout << "7) Remove an ingredient\n";
    std::cout << "8) Make a recipe\n";
    std::cout << "9) Remove a recipe\n";
    std::cout << "10) Suggest recipes\n";
    std::cout << "11) Generate a shopping list for a recipe (and buy)\n";
    std::cout << "12) Clone an app recipe into my recipes\n";
    std::cout << "13) View recipe details\n";
    if (isAdmin) {
        std::cout << "100) [ADMIN] Create app/global recipe\n";
        std::cout << "101) [ADMIN] List app recipes\n";
    }
    if (isOwner) {
        std::cout << "200) [OWNER] List all users\n";
        std::cout << "201) [OWNER] Promote user to admin\n";
        std::cout << "202) [OWNER] Demote admin to user\n";
        std::cout << "203) [OWNER] Create a new user\n";
        std::cout << "204) [OWNER] Delete a user account\n";
    }
    std::cout << "0) Save and exit\n";
    std::cout << "Choice: ";
}

// ================== MAIN ==================

int main() {
    fs::create_directories(USER_FOLDER);

    loadUserDirectory(g_usernames, g_passwords, g_roles);

    std::cout << "=== Cooking Helper ===\n";
    showAllUsers(g_usernames, g_roles);
    std::cout << "\nEnter username: ";

    std::string username;
    std::getline(std::cin, username);

    int idx = findUserIndex(g_usernames, username);
    bool isAdmin = false;
    bool isOwner = false;

    if (idx == -1) {
        std::string pw;
        std::cout << "No account found. Create one (Type Cancel to Cancel).\n";
        std::cout << "Set password: ";
        std::getline(std::cin, pw);
        if(pw == "Cancel" || pw.empty()) {
            std::cout << "Account creation cancelled. Exiting.\n";
            return 0;
        }
        g_usernames.push_back(username);
        g_passwords.push_back(pw);
        g_roles.push_back("user");
        saveUserDirectory(g_usernames, g_passwords, g_roles);
    } else {
        std::string pw;
        bool ok = false;
        for (int tries = 0; tries < 3; tries++) {
            std::cout << "Enter password: ";
            std::getline(std::cin, pw);
            if (pw == g_passwords[idx]) {
                ok = true;
                break;
            } else {
                std::cout << "Wrong password.\n";
            }
        }
        if (!ok) {
            std::cout << "Too many tries. Exiting.\n";
            return 0;
        }
        if (g_roles[idx] == "admin") {
            isAdmin = true;
        } else if (g_roles[idx] == "owner") {
            isAdmin = true;
            isOwner = true;
        }
    }

    loadData(username);
    loadAppRecipes();

    for (auto &r : recipeList) {
        recalculateRecipeCalories(r);
    }
    for (auto &r : appRecipeList) {
        recalculateRecipeCalories(r);
    }

    int choice = -1;
    while (choice != 0) {
        displayMenu(isAdmin, isOwner);
        std::cin >> choice;
        std::cin.ignore();

        if      (choice == 1)  listAllRecipes(isAdmin, false);
        else if (choice == 2)  viewAllIngredients();

        // user recipe/item CRUD
        else if (choice == 3)  createRecipe();
        else if (choice == 4)  modifyRecipe(isAdmin);
        else if (choice == 5)  modifyIngredient();
        else if (choice == 6)  addNewIngredient();
        else if (choice == 7) removeIngredient();
        else if (choice == 8)  makeRecipe();
        else if (choice == 9)  removeRecipe();

        // helpers / smart features
        else if (choice == 10) suggestRecipes();
        else if (choice == 11) generateShoppingList();
        else if (choice == 12) cloneAppRecipeToUser("");
        else if (choice == 13) viewRecipeDetails();

        // admin
        else if (choice == 100 && isAdmin) createOwnerRecipe();
        else if (choice == 101 && isAdmin) listAppRecipes();

        // owner
        else if (choice == 200 && isOwner) ownerListUsers();
        else if (choice == 201 && isOwner) ownerPromoteUser();
        else if (choice == 202 && isOwner) ownerDemoteUser();
        else if (choice == 203 && isOwner) ownerCreateUser();
        else if (choice == 204 && isOwner) deleteUserAccount();

        // exit
        else if (choice == 0) {
            saveData(username);
            saveAppRecipes();
            std::cout << "Goodbye!\n";
        }
        else {
            std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}