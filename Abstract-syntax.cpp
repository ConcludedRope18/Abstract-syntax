#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct Node {
    std::string value;
    int lindex; //The index of the left child node, if it exists
    int rindex; //The index of the right child node, if it exists
    int pindex; //The index of the parent node
    bool truthValue; //The truth value given by the interpretation if the node has a variable in it
};

bool checkProp(std::string n) { //Function that checks if the user's input is a valid propositional formula
    int sym = 0, open = 0, closed = 0; //Variables to memorise the number of logical operators, open paranthesis, and closed paranthesis
    if (n[0] != '(' || n[n.size()-1]!=')') //If the whole string is not inside () it means it is not a valid propositional formula
        return false;
    for (int i = 0; i < n.size(); i++) //Go through every character of the input string
    {

        //std::cout << "DEBUG: n[i]= " << n[i] << " sym= " << sym << " open= " << open << " closed= " << closed << std::endl; //Debug output

        if (n[i] == '(') //If the character is an open paranthesis, add 1 to the counter for open paranthesis then make sure that it's followed by either another open paranthesis or a variable
        {
            open++;
            if (n[i + 1] == '&' || n[i + 1] == '|' || n[i + 1] == '>' || n[i + 1] == '<')
                return false;
        }
        else if (n[i] == ')') //If the character is a closed paranthesis, add 1 to the counter for closed paranthesis then make sure that it's not followed by an open paranthesis because that would make it not valid
        {
            closed++;
            if (n[i + 1] == '(')
                return false;
        }
        else if (n[i] == '&' || n[i] == '|' || n[i] == '>' || n[i] == '<' || n[i] == '!') //If the character is a logical operator, add 1 to the counter for operators and make sure that it's followed by either a variable or an open paranthesis
        {
            sym++;
            if (i + 1 < n.size())
                if (n[i + 1] == ')')
                    return false;
        }
        else if (n[i + 1] == '(' || isalpha(n[i+1])) //If the character is a variable make sure it's followed by either an operator or a closed paranthesis
            return false;
    }
    
    if (sym == open && sym == closed && open == closed) //This returns true if there is a set of paranthesis for every single operator and all other conditions from before are true
        return true;
    
    return false; //Default case
}

void changeNode(Node& node, std::string value, int l, int r, int p) //Function that changes all of the values of a node using given values
{
    node.value = value;
    node.lindex = l;
    node.rindex = r;
    node.pindex = p;
}

void makeList(std::string s, std::vector<Node>& list) //Function that creates the actual list of nodes needed for the output; It takes the input string, and a vector of nodes for the list as arguments
{
    int index = 0, n = 0; //Index to be used when creating the list, and a variable n that stores how many nodes are in the list
    Node newNode; //Node that will be used to update all of the nodes in the vector list
    changeNode(newNode, "", 0, 0, 0); //Set default values for the new node
    list[0] = newNode; //Create the root of the list

    for (int j = 0; j < s.size(); j++) //Go through every single character in the input string and check what type of character they are
    {
        //std::cout << "DEBUG: j = " << j << ", index = " << index << ", n = " << n << ", Value in string: " << s[j] << ", Next: " << s[j+1] << std::endl; //Debug output

        if (s[j] == '(')
        {
            if (s[j + 1] == '!')
            {
                //Make a node on the left by adding 1 to the number of nodes in the list and saving the index of the last node as the lindex from the current node
                list[index].lindex = n + 1;
                changeNode(newNode, "", 0, 0, index); //Set the new node to default values other than the parent index which is going to be the index of the current node, then add it as the last element in the list
                list[n + 1] = newNode;
                n++;

                //std::cout << "made a node on the left" << std::endl; //Debug output
            }
            else
            {
                //Make nodes on both left and right, then move to the left child node of the current node
                list[index].lindex = n + 1;
                list[index].rindex = n + 2; //Adds the right node to the end of the list, after the left node
                changeNode(newNode, "", 0, 0, index);
                list[n + 1] = newNode;
                changeNode(newNode, "", 0, 0, index);
                list[n + 2] = newNode;
                index = n + 1; //Moves to the index of the left child node
                n += 2; //Goes up by 2 because we added two nodes

                //std::cout << "Made nodes on both left and right, then moved to the left" << std::endl; //Debug output
            }
        }

        else if (s[j] == '!')
        {
            //Add ! as a value then move to the left child node
            list[index].value = "!";
            index = list[index].lindex;

            //std::cout << "Added ! as a value then made a node on the left and moved left" << std::endl; //Debug output
        }

        else if (s[j] == '&' || s[j] == '|' || s[j] == '<' || s[j] == '>') //
        {
            //add the symbol as value then move to the right child node
            list[index].value = s[j];
            index = list[index].rindex;

            //std::cout << "added the symbol as value then moved right" << std::endl; //Debug output
        }

        else if (s[j] == ')')
        {
            if (j + 1 == s.size()) //Checks if the next element exists by comparing the next index in the string with the complete length of the string, this works because the indexing starts from 0 so the last element has the index s.size()-1
            {
                break;
            }
            else
            {
                //Go up to the parent node of the current node
                index = list[index].pindex;

                //std::cout << "Went up to the parent" << std::endl; //Debug output
            }
        }

        else //if it's a variable, save the variable name as the value of the node and move back up to the parent node
        {
            list[index].value = s[j]; // Store the variable name
            index = list[index].pindex; // Go up to the parent node
        }
    }
}

void interpretation(std::vector<Node>& list) //Adds interpretations to all variables in the list
{
    for (int i = 0; list[i].value != ""; i++) //Goes through all nodes in the list
    {
        if (isalpha(list[i].value[0])) //Checks if the current node's value is a variable
        {
            for (int j = i - 1; j >= 0; j--) //Checks if the variable already appeared once in the list by going backwards starting from the element before the current one
            {
                if (list[j].value == list[i].value) //If the variable already appeared before it means it already has a truth value so it just copies it then stops checking further
                {
                    list[i].truthValue = list[j].truthValue;
                    break;
                }
            }

            if (list[i].truthValue != true && list[i].truthValue != false) //If the variable hasn't appeared in the list before, it means the truth value isn't initialised, which is what this if checks for
            {
                // Prompt the user to enter the truth value for this variable
                char input;
                std::cout << "Enter truth value for variable " << list[i].value << " (0 for false, 1 for true): ";
                std::cin >> input;
                list[i].truthValue = (input == '1'); // Convert the input to a boolean
            }
        }
    }
}

void printList(std::vector<Node> list) //Function that prints the entire vector list
{
    std::cout << "0: " << "Value-> " << list[0].value << std::endl; //Print the root separately because it doesn't have a parent node

    for (int k = 1; list[k].value != ""; k++) //Print the index, the value, and the index of the parent for all other nodes
    {
        std::cout << k << ": " << "Value-> " << list[k].value << " ; Parent index " << list[k].pindex << std::endl;
    }
}

bool evaluateFormula(const std::vector<Node>& list, int index) //Recursive function to evaluate the truth value of a propositional formula
{
    const Node& node = list[index]; //Node variable that memorises the node from the starting index

    if (node.value == "!") //If the current node represents the NOT operator, recursively evaluate the expression that comes after the NOT operator, and then negate the result
    {
        return !evaluateFormula(list, node.lindex);
    }

    else if (node.value == "&") //If the current node represents the AND operator, recursively evaluate both the left and right subexpressions, then return the logical AND result
    {
        return evaluateFormula(list, node.lindex) && evaluateFormula(list, node.rindex);
    }

    else if (node.value == "|") //If the current node represents the OR operator, recursively evaluate both left and right subexpressions, then return the logical OR result
    {
        return evaluateFormula(list, node.lindex) || evaluateFormula(list, node.rindex);
    }

    else if (node.value == ">") //If the current node represents the IMPLICATION operator, recursively evaluate both left and right subexpressions, then apply IMPLICATION logic
    {
        //IMPLICATION is only false if the first element is true and the second element is false so the program negates the first element and checks if they're both false using the OR operator
        return !evaluateFormula(list, node.lindex) || evaluateFormula(list, node.rindex);
    }

    else if (node.value == "<")  //If the current node represents the DOUBLE IMPLICATION operator, recursively evaluate both of the directions of the implication and return the result 
    {
        return (!evaluateFormula(list, node.lindex) || evaluateFormula(list, node.rindex)) && (!evaluateFormula(list, node.rindex) || evaluateFormula(list, node.lindex));
    }

    else if (isalpha(node.value[0])) // Return the truth value of the variable
    {
        return node.truthValue;
    }

    return false; // Default case
}


int main()
{
    std::string inputString; //The string where the input proposition gets stored
    std::vector<Node> list(100); //A vector of nodes that memorates the list needed for the output

    //Prompt the user to enter their propositional formula, and explain the notation for each logical operator
    std::cout << "And: & ; Or: | ; Implication: > ; Double implication: < ; Not: !" << std::endl << "Enter your propositional formula: " << std::endl;
    std::getline(std::cin, inputString);

    //Remove all of the spaces from the input
    inputString.erase(std::remove_if(inputString.begin(), inputString.end(), ::isspace), inputString.end());

    if (checkProp(inputString) == 0) //Check if the input is a propositional formula
    {
        std::cout << "NOT A PROPOSITIONAL FORMULA"; return 0; //If it's not a propositional formula, tell the user that it's wrong and end the program
    }

    makeList(inputString, list); //Create the complete list of nodes for the input

    std::cout << std::endl;
    printList(list); //Print the list

    //Ask the user if they want to add an interpretation to the list so the program can return the value of the formula under that interpretation and save their response in a string variable
    std::string input;
    std::cout << std::endl;
    std::cout << "Would you like to add an interpretation in order to get the value of the formula? (yes/no)" << std::endl;
    std::getline(std::cin, input);
    std::cout << std::endl;

    while (input=="yes") //Keep returning values under interpretations until user wants to stop
    {
        std::vector<Node> list2(100); //Create a secondary list and copy the first list in it in order to add interpretations
        list2 = list;

        interpretation(list2); //Add the user's interpretation to the list

        bool result = evaluateFormula(list2, 0); //Start evaluation from the root (index 0)
        std::cout << std::endl << "Result: " << (result ? "true" : "false") << std::endl; //Print the result of the evaluation under the user's interpretation

        std::cout << std::endl;
        std::cout << "Would you like to add another interpretation? (yes/no)" << std::endl; //Ask for another interpretation
        std::cin.ignore(); // Remove the newline character
        std::getline(std::cin, input);
        std::cout << std::endl;
    }

    std::cout << "Goodbye";

    return 0;
}

