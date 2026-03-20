#include <iostream>
#include <curl/curl.h>
#include <simdjson.h>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>

struct tf2pde {
    char name[128];
    bool craftable;
    int quality; 
    bool australium;
    float buyprice;
    int qty;
    float total;
    char currency[5];
    float cur_key_Price;
    time_t date;
};

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool isdigit(std::string str) {
    for (char ch : str) {
        int v = ch;
        if (!(ch >= 48 && ch <= 57 || ch == 46)) {
            return false;
        }
    }

    return true;
}

std::string tf2perr(std::string message) {
    return "\033[38;5;196mError:\033[0m " + message;
}

std::string tf2pwarn(std::string message) {
    return "\033[38;5;226mWarning:\033[0m " + message;
}

std::string tf2pnormal(std::string message) {
    return "\033[38;2;168;168;168m" + message + "\033[0m";
}

std::string tf2pgenuine(std::string message) {
    return "\033[38;2;77;116;85m" + message + "\033[0m";
}

std::string tf2pvintage(std::string message) {
    return "\033[38;2;71;98;145m" + message + "\033[0m";
}

std::string tf2punusual(std::string message) {
    return "\033[38;2;134;80;172m" + message + "\033[0m";
}

std::string tf2punique(std::string message) {
    return "\033[38;2;255;215;0m" + message + "\033[0m";
}

std::string tf2pcommunity(std::string message) {
    return "\033[38;2;112;176;74m" + message + "\033[0m";
}

std::string tf2pvalve(std::string message) {
    return "\033[38;2;165;15;121m" + message + "\033[0m";
}

std::string tf2pselfmade(std::string message) {
    return "\033[38;2;112;176;74m" + message + "\033[0m";
}

std::string tf2pstrange(std::string message) {
    return "\033[38;2;207;106;50m" + message + "\033[0m";
}

std::string tf2phaunted(std::string message) {
    return "\033[38;2;56;243;171m" + message + "\033[0m";
}

std::string tf2pcollectors(std::string message) {
    return "\033[38;2;170;0;0m" + message + "\033[0m";
}

std::string tf2pdecorated(std::string message) {
    return "\033[38;2;250;250;250m" + message + "\033[0m";
}

std::string tf2ptexttoquality(std::string message, int quality) {
    if (quality == 0) {
        return tf2pnormal(message);
    } else if (quality == 1) {
        return tf2pgenuine(message);
    } else if (quality == 3) {
        return tf2pvintage(message);
    } else if (quality == 5) {
        return tf2punusual(message);
    } else if (quality == 6) {
        return tf2punique(message);
    } else if (quality == 7) {
        return tf2pcommunity(message);
    } else if (quality == 8) {
        return tf2pvalve(message);
    } else if (quality == 9) {
        return tf2pselfmade(message);
    } else if (quality == 11) {
        return tf2pstrange(message);
    } else if (quality == 13) {
        return tf2phaunted(message);
    } else if (quality == 14) {
        return tf2pcollectors(message);
    } else if (quality == 15) {
        return tf2pdecorated(message);
    } else {
        return tf2perr("invalid quality id!");
    }
}

int main() {

    simdjson::dom::parser parser;
    simdjson::dom::element doc;

    std::fstream key_file("key");
    std::fstream data_file;

    std::vector<tf2pde> data;

    data_file.open("data.bin", std::ios::in | std::ios::out | std::ios::binary);

    if (!data_file.is_open()) {
        data_file.open("data.bin", std::ios::out | std::ios::binary);
        data_file.close();

        data_file.open("data.bin", std::ios::in | std::ios::out | std::ios::binary);
    }

    tf2pde entry;

    while (data_file.read(reinterpret_cast<char*>(&entry), sizeof(tf2pde))) {
        data.push_back(entry);
    }

    data_file.clear();

    data_file.seekp(0);

    std::string api_key;
    getline(key_file, api_key);
    std::string request_url = "https://backpack.tf/api/IGetPrices/v4?key=" + api_key;
    std::string price_data_buffer;
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, request_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &price_data_buffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);

        simdjson::padded_string data_json = price_data_buffer;

        doc = parser.parse(data_json);

    }
        
    while (true) {
        std::string command;
        std::cout << "\033[38;5;208mTF2PC_Console>> \033[0m";
        std::getline(std::cin, command);
        if (command == "exit") {
            break;
        } else if (command.rfind("addi", 0) == 0) {
            std::string name;
            float price;
            int quality;
            std::string currency;
            bool australium;
            bool craftable;
            float cur_key_price;
            int qty;
            std::string craftable_str;
            if (command.find("name=\"") != std::string::npos) {
                int s_pos = command.find("name=\"") + 6;
                int e_pos = command.find("\" ", s_pos);
                name = command.substr(s_pos, e_pos - s_pos);

            }else if (command.find("name=") != std::string::npos) {
                int s_pos = command.find("name=") + 5;
                int e_pos = command.find(" ", s_pos);
                name = command.substr(s_pos, e_pos - s_pos);

            } else {
                std::cout << tf2pwarn("No name set for new entry. Give name of the item for new entry>>");
                std::getline(std::cin, name);
            }
            if (command.find("quality=") != std::string::npos) {
                int s_pos = command.find("quality=") + 8;
                int e_pos = command.find(" ", s_pos);
                quality = stoi(command.substr(s_pos, e_pos - s_pos));

            } else {
                std::cout << tf2pwarn("No quality set for new entry. Give quality id of the item for new entry>>");
                std::string temp_str;
                std::getline(std::cin, temp_str);
                if (isdigit(temp_str) == true) {
                    quality = stoi(temp_str);
                } else {
                    quality = 0;
                }
            }
            if (command.find("craftable=") != std::string::npos) {
                int s_pos = command.find("craftable=") + 10;
                int e_pos = command.find(" ", s_pos);
                std::string temp_str = command.substr(s_pos, e_pos - s_pos);
                if (temp_str == "true") {
                    craftable = true;
                    craftable_str = "Craftable";
                } else {
                    craftable = false;
                    craftable_str = "Non-Craftable";
                }

            } else {
                std::cout << tf2pwarn("No value set for craftable. Give true or false for new entry>>");
                std::string temp_str;
                std::getline(std::cin, temp_str);
                if (temp_str == "true") {
                    craftable = true;
                    craftable_str = "Craftable";
                } else {
                    craftable = false;
                    craftable_str = "Non-Craftable";
                }
            }

            if (command.find("keyprice=") != std::string::npos) {
                int s_pos = command.find("keyprice=") + 9;
                int e_pos = command.find(" ", s_pos);
                std::string temp_str = command.substr(s_pos, e_pos - s_pos);
                if (isdigit(temp_str) == true) {
                    cur_key_price = stof(temp_str);
                } else {
                    cur_key_price = doc["response"]["items"]["Mann Co. Supply Crate Key"]["prices"]["6"]["Tradable"]["Craftable"].at(0)["value"].get_double();
                }
            

            } else {
                cur_key_price = doc["response"]["items"]["Mann Co. Supply Crate Key"]["prices"]["6"]["Tradable"]["Craftable"].at(0)["value"].get_double();
            }
            if (command.find("price=") != std::string::npos) {
                int s_pos = command.find("price=") + 6;
                int e_pos;
                if (command.find("k", s_pos) != std::string::npos) {
                    int e_pos = command.find("k", s_pos);
                    price = stof(command.substr(s_pos, e_pos - s_pos));
                    currency = "keys";
                } else if (command.find("m", s_pos) != std::string::npos) {
                    int e_pos = command.find("m", s_pos);
                    price = stof(command.substr(s_pos, e_pos - s_pos));
                    currency = "metal";
                } else {
                    std::cout << tf2pwarn("you need to specify currency. k for keys, m for metal (if lef blank or invalid. default = metal)>> ");
                    std::string temp_str;
                    std::getline(std::cin, temp_str);
                    if (temp_str == "k") {
                        currency = "keys";
                    } else {
                        currency = "metal";
                    }
                }

            } else {

                auto name_doc = doc["response"]["items"][name];
                auto quality_doc = name_doc["prices"][std::to_string(quality)];
                auto craftable_doc = quality_doc["Tradable"][craftable_str];
                auto value_doc = craftable_doc.at(0)["value"];
                auto currency_doc = craftable_doc.at(0)["currency"];
                

                price = value_doc.get_double();
                std::string_view currency_view = currency_doc.get_string();
                currency = std::string(currency_view);
            }

            if (command.find("quantity=") != std::string::npos) {
                int s_pos = command.find("quantity=") + 9;
                int e_pos = command.find(" ", s_pos);
                qty = stoi(command.substr(s_pos, e_pos - s_pos));

            } else {
                std::cout << tf2pwarn("No quantity set for new entry. Give quantity of the items for new entry>>");
                std::string temp_str;
                std::getline(std::cin, temp_str);
                if (isdigit(temp_str) == true) {
                    qty = stoi(temp_str);
                } else {
                    qty = 1;
                }
            }
            
            auto if_exists = doc["response"]["items"][name]["prices"][std::to_string(quality)]["Tradable"][craftable_str].at(0)["value"];

            if (if_exists.error()) {
                std::cout << tf2perr("Invalid item!") << std::endl;
            } else {

                tf2pde new_entry;
                strcpy(new_entry.name, name.c_str());
                new_entry.quality = quality;
                new_entry.craftable = craftable;
                new_entry.australium = australium;
                new_entry.buyprice = price;
                strcpy(new_entry.currency, currency.c_str());
                new_entry.qty = qty;
                new_entry.total = price * qty;
                new_entry.cur_key_Price = cur_key_price;
                new_entry.date = time(NULL);

                data.push_back(new_entry);
            }





        } else if (command == "qc") {
            std::cout << "Quality id chart\n\nid quality\n\n"
            << tf2pnormal("0. Normal\n") 
            << tf2pgenuine("1. Genuine\n") 
            << tf2pvintage("3. Vintage\n") 
            << tf2punusual("5. Unusual\n") 
            << tf2punique("6. Unique\n")
            << tf2pcommunity("7. Community\n") 
            << tf2pvalve("8. Valve\n")
            << tf2pselfmade("9. Self-Made\n")
            << tf2pstrange("11. Strange\n")
            << tf2phaunted("13. Haunted\n")
            << tf2pcollectors("14. Collectors\n")
            << tf2pdecorated("15. Decorated\n")
            << std::endl;

        } else if (command == "lookt") {
            for (tf2pde entry : data) {
                std::string craftable;
                if (entry.craftable == true) {
                    craftable = "Craftable";
                } else {
                    craftable = "Non-Craftable";
                }

                float c_price = doc["response"]["items"][std::string(entry.name)]["prices"][std::to_string(entry.quality)]["Tradable"][craftable].at(0)["value"].get_double();

                std::string_view c_currency_view = doc["response"]["items"][std::string(entry.name)]["prices"][std::to_string(entry.quality)]["Tradable"][craftable].at(0)["currency"].get_string();
                std::string c_currency = std::string(c_currency_view);


                float key_price = doc["response"]["items"]["Mann Co. Supply Crate Key"]["prices"]["6"]["Tradable"]["Craftable"].at(0)["value"].get_double();

                float difference;
                std::string s_difference;
                if (c_currency != entry.currency) {
                    if (c_currency == "keys") {
                        difference = (c_price * key_price / entry.buyprice - 1) * 100;
                    } else {
                        difference = (c_price / (entry.buyprice * entry.cur_key_Price) - 1) * 100;
                    }
                } else {
                    difference = (c_price / entry.buyprice - 1) * 100;
                }

                if (difference >= 0) {
                    s_difference = "\033[38;2;10;230;10m+" + std::to_string(difference) + "%\033[0m";
                } else {
                    s_difference = "\033[38;2;230;10;10m" + std::to_string(difference) + "%\033[0m";
                }

                std::cout << "____________________________________\n" << std::endl;
                std::cout << "Name: " << tf2ptexttoquality(entry.name, entry.quality) << std::endl;
                std::cout << "Price (bought): " << entry.buyprice << " " <<  entry.currency << std::endl;
                std::cout << "Price (current): " << c_price
                << " " << c_currency << std::endl;
                std::cout << "Difference: " << s_difference << std::endl; 
                std::cout << "Quantity: " << entry.qty << std::endl;
                std::cout << "Total: " << entry.total << " " << entry.currency << std::endl;
                std::cout << "Add date: " << ctime(&entry.date) << "\n" << std::endl;
            }

        } else {
            std::cout << tf2perr("Invalid command!") << std::endl;
        }
    }
    for (tf2pde entry : data) {
        data_file.write(reinterpret_cast<char*>(&entry), sizeof(tf2pde));
    }
    

    return 0;
}