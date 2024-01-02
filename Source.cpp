#include <iostream>
#include <fstream>
#include "SFML/Graphics.hpp"

using namespace std;

class Node {
public:
    string word;
    Node* next = nullptr;
};

class List {
public:
    Node* head;
    List() {
        head = nullptr;
    }

    void insert_name(const string& p_word) {
        Node* temp = new Node;
        temp->word = p_word;
        temp->next = nullptr;

        if (head == nullptr) {
            head = temp;
        }
        else {
            Node* itemp = head;
            while (itemp->next != nullptr) {
                itemp = itemp->next;
            }
            itemp->next = temp;
        }
    }

    void display_obj(sf::RenderWindow& window, const std::string& title, float xPos, float yPos) {
        Node* temp = head;
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            cerr << "Error: Font not found." << endl;
            return;
        }

        sf::Text titleText;
        titleText.setFont(font);
        titleText.setCharacterSize(30);
        titleText.setFillColor(sf::Color::Red);
        titleText.setPosition(xPos, yPos);
        titleText.setString(title);

        sf::Text fileListText;
        fileListText.setFont(font);
        fileListText.setCharacterSize(25);
        fileListText.setFillColor(sf::Color::White);

        float yOffset = 50.0f;
        while (temp != nullptr) {
            fileListText.setPosition(xPos + 100.f, yPos + yOffset);
            fileListText.setString(temp->word);
            window.draw(fileListText);
            yOffset += 40.0f;
            temp = temp->next;
        }

        window.draw(titleText);
    }


};

void remove_duplicate_words(List& obj3) {
    Node* temp = obj3.head;

    while (temp != nullptr) {
        string wordToRemove = temp->word;
        string filePath = "testfile/testfile_2.txt";

        ifstream inFile(filePath);
        ofstream tempFile("testfile/temp.txt");

        if (!inFile || !tempFile) {
            cerr << "Error: File handling issue!" << endl;
            return;
        }

        string word;
        while (inFile >> word) {
            if (word != wordToRemove) {
                tempFile << word << " ";
            }
        }

        inFile.close();
        tempFile.close();

        // Remove the original file
        remove(filePath.c_str());
        // Rename temp file to original filename
        rename("testfile/temp.txt", filePath.c_str());

        temp = temp->next;
    }
}


void find_duplicate(List obj1, List obj2, List& obj3) {
    Node* temp1 = obj1.head;
    while (temp1 != NULL)
    {
        Node* temp2 = obj2.head;
        while (temp2 != NULL)
        {
            if (temp1->word == temp2->word)
            {
                obj3.insert_name(temp1->word);
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }

}
void read_file(const string& filePath, List& obj) {
    ifstream file(filePath);
    if (!file) {
        cerr << "Error: Unable to open file " << filePath << endl;
        return;
    }

    string filename;
    while (file >> filename) {
        obj.insert_name(filename);
    }

    file.close();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1300, 800), "File Contents Display");

    List obj1, obj2, obj3;
    read_file("testfile/testfile_1.txt", obj1);
    read_file("testfile/testfile_2.txt", obj2);
    find_duplicate(obj1, obj2, obj3);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Error: Font not found." << endl;
        return EXIT_FAILURE;
    }

    sf::Text promptText;
    promptText.setFont(font);
    promptText.setCharacterSize(30);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(50.f, 600.f);
    promptText.setString("Do you want to remove the duplicate files? (y/n): ");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) 
            {
                if (event.key.code == sf::Keyboard::Y)
                {
                    remove_duplicate_words(obj3);
                    promptText.setString("Duplicate file has been deleted. Press Escape to Exit ! ");

                }
                if (event.key.code == sf::Keyboard::N) 
                {
                    promptText.setString("Duplicate file has remained same. Press Escape to Exit ! ");
                    
                }
                if (event.key.code == sf::Keyboard::Escape) {
                   
                    return 0;
                }

            }
        }

        window.clear();

        obj1.display_obj(window, "Contents of file 1:", 50.f, 50.f);
        obj2.display_obj(window, "Contents of file 2:", 50.f, 230.f);
        obj3.display_obj(window, "following are the duplicate words : ", 50.f, 450.f);
        window.draw(promptText);
        window.display();
    }
    return 0;
}