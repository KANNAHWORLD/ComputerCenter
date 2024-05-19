#include <iostream>

/*
 * @todo Fix the command to see the correct ip address
 */
void manageTerminal(){
    char option;
    std::cout << "Type 'h' for help\n";
    while(std::cin >> option){
       switch(option){
            case 'q':
                goto exit;
                break;
            case 'i':
                std::cout << "Under maintenance, check back later for this to work\n";
            case 'h':
            default:
                std::cout << "\nThese are the following valid commands:\n";
                std::cout << "\tq -- stop the server\n";
                std::cout << "\th -- help, view all commands\n";
                std::cout << "\ti -- view server's ip address\n";
       }
       std::cin.ignore(INT_MAX, '\n');
       std::cin.clear();
    }
exit:
    return;
}