#include <iostream>
#include <pugixml.hpp>
#include <string>
#include <string.h>
#include <stdlib.h>

namespace XMLParser
{
    struct Walker: pugi::xml_tree_walker
    {
        virtual bool for_each(pugi::xml_node &node)
        {
            for (int i = 0; i < depth(); ++i)
                std::cout << "  "; // indentation

            std::cout << node.name() << " " << node.value() << "\n";

            return true; // continue traversal
        }
    };
}

namespace EditXml
{
    struct XmlEditor
    {
        XmlEditor(pugi::xml_document* doc, char* file_name) 
            : doc_(doc), file_name_(file_name)
        {
            ;
        } 
        void append()
        {
            while (true)
            {
                std::string item_parent =
                    fill_string("\nName of a parent item ('-1' is global) > ");
                std::string item_name = fill_string("\nName of a new item > ");
                std::string value = fill_string("\nValue ('-1' if nothing) > ");

                pugi::xml_node node;
                if (item_parent == "-1")
                    node = doc_->append_child(item_name.c_str());
                else
                {
                    node = doc_->child(item_parent.c_str());
                    node = node.append_child(item_name.c_str());
                }
                if (value != "-1")
                    node.append_child(pugi::node_pcdata).set_value(value.c_str());
                char exit;
                std::cout << "\n\nContinue? (n - no) > ";
                std::cin >> exit;
                if (exit == 'n')
                    break;
                system("clear");
                std::cin.ignore();
            }
            doc_->save_file(file_name_);
        }
        void show_console()
        {
            XMLParser::Walker walker;
            doc_->traverse(walker);
        }

    private:
        std::string fill_string(const char* title)
        {
            std::string res;
            std::cout << title;
            std::getline(std::cin, res);
            return res;
        }
    private:
        pugi::xml_document* doc_;
        char* file_name_;
    };
}

int main(int argc, char **argv)
{
    char* file_name;
    switch (argc)
    {
    case 2: 
        file_name = argv[1]; 
        break;
    case 3:
        if (strcmp(argv[1], "-d") == 0)
            std::remove(argv[2]);
        return 0;
    default:
        return 1;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file_name);

    EditXml::XmlEditor editor {&doc, file_name};
    editor.append();
    editor.show_console();

    return 0;
}