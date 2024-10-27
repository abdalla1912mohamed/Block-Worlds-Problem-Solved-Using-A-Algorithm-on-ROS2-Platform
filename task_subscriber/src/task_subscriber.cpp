
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include<algorithm>
#include <utility>
#include<sstream> 
using namespace std;

// vector<vector<string>> initial_state = {{"A", "B", "C"}, {"D","E"}}; // it works 
// vector<vector<string>> goal_state = {{"A","B","C","E","D"}}; // it works 
    vector<vector<string>> initial_state ; 
    vector<vector<string>> goal_state ; 

class TaskSubscriber : public rclcpp::Node
{
public:
    TaskSubscriber() : Node("task_subscriber")
    {
        // Subscribe to the input string topics
        initial_subscription_ = this->create_subscription<std_msgs::msg::String>(
            "initial_string", 10,
            [this](const std_msgs::msg::String::SharedPtr msg) {
                // Callback function for incoming initial string message
                RCLCPP_INFO(this->get_logger(), "Received initial string: '%s'", msg->data.c_str());
                initial_string_ = msg->data;
                processStrings();
            });

        goal_subscription_ = this->create_subscription<std_msgs::msg::String>(
            "goal_string", 10,
            [this](const std_msgs::msg::String::SharedPtr msg) {
                // Callback function for incoming goal string message
                RCLCPP_INFO(this->get_logger(), "Received goal string: '%s'", msg->data.c_str());
                goal_string_ = msg->data;
                processStrings();
            });

        // Create a publisher for the output string topic
        publisher_ = this->create_publisher<std_msgs::msg::String>("optimal_moves", 10);
    }

    std::string getInitialString() const
    {
        return initial_string_;
    }

    std::string getGoalString() const
    {
        return goal_string_;
    }

    void publishResult(const std::string &result)
    {
        std_msgs::msg::String output_msg;
        output_msg.data = result;
        publisher_->publish(output_msg);
    }

    std::string initial_string_;
    std::string goal_string_;

private:
    void processStrings()
    {
        // Check if both initial and goal strings are received
        if (!initial_string_.empty() && !goal_string_.empty())
        {
            // Strings are received and stored, ready for processing
            RCLCPP_INFO(this->get_logger(), "Both strings received, ready for processing.");
        }
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr initial_subscription_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr goal_subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};



////////////////////

   
struct action {
int start ; 
string start_block ; 
int end ; 
string end_block ;    // ana action entity that stores the possible action / move taken 
// a state have a list of current possible action  
};

class State {
public:
    vector<vector<string>> state;
    vector<string> moves;
    int cost;
    int heuristic_value;
    int stacks;
    int best_index ; 
    vector<int> stack_lengths;
    vector<action> actions_list ;   // list of possible actions 
    vector<int> children_indices ; 
    
    State(const vector<vector<string>>& initial_state) {
        update_stacks(initial_state);
    }
    State() {
    // Initialize member variables or leave them uninitialized
}

    void update_stack_lengths() {
        stack_lengths.resize(stacks);
        for (int i = 0; i < stacks; ++i) {
            stack_lengths[i] = state[i].size();
        }
    }

    void update_stacks(const vector<vector<string>>& new_state) {
        state = new_state;
        stacks = state.size();
        update_stack_lengths();
    }

    vector<vector<string>>& get_stacks() {
        return state;
    }

    bool operator<(const State& other) const {
        return (cost + heuristic_value) > (other.cost + other.heuristic_value);
    }









void print_state( ){
vector<vector<string>> initial_stacks = state ; 
cout<<" \n" ;
 for (int i = 0; i < initial_stacks.size(); ++i) {
        cout << "  Stack " << i << ": ";
        for (const string& block : initial_stacks[i]) { // do a print state function 
            cout << block << " ";
        }
        cout << endl;
    }




}





void printActions() {
    cout << " \n List of Possible Actions:" << endl;
    for (const auto& action : actions_list) {
        cout << "Move block " << action.start_block << " from stack " << action.start << " to stack " << action.end;
        cout << " (on Top of " << action.end_block << ")" << endl;
    }
}






};


void removeEmptyStacks(vector<vector<string>>& stacks) {
    for (auto it = stacks.begin(); it != stacks.end();) {
        if (it->empty()) {
            it = stacks.erase(it);
        } else {
            ++it;
        }
    }
}
// Function to sort the stacks by their sizes from bigger to smaller using bubble sort
vector<vector<string>> sortByStackSize(vector<vector<string>>& stacks) {
    // Remove empty stacks
    removeEmptyStacks(stacks);

    // Bubble sort the stacks by size
    bool swapped;
    do {
        swapped = false;
        for (size_t i = 0; i < stacks.size() - 1; ++i) {
            if (stacks[i].size() < stacks[i + 1].size()) {
                swap(stacks[i], stacks[i + 1]);
                swapped = true;
            }
        }
    } while (swapped);

    return stacks;
}

int heuristic(vector<vector<string>>& current_state, vector<vector<string>>& goal_state) {
    int sum = 0;

    // Iterate through each stack in the current state
    for (const auto& stack : current_state) {
        // Find the corresponding goal stack
        vector<string> goal_stack;
        for (const auto& other_stack : goal_state) {
            if (!stack.empty() && find(other_stack.begin(), other_stack.end(), stack[0]) != other_stack.end()) {
                goal_stack = other_stack;
                break;
            }
        }

        // Iterate through each block in the stack
        for (size_t i = 0; i < stack.size(); ++i) {
            string block = stack[i];
            int block_position = i;

            // If the block is in the goal stack
            if (!goal_stack.empty() && find(goal_stack.begin(), goal_stack.end(), block) != goal_stack.end()) {
                int goal_block_position = find(goal_stack.begin(), goal_stack.end(), block) - goal_stack.begin();

                // If the block is already in its correct position
                if (block_position == goal_block_position) {
                   sum=sum-1;  // important 
                    continue;
                }

                // Calculate the number of moves needed
                sum += stack.size() - block_position;

                // Move the block to its correct position
                for (size_t j = block_position; j < stack.size(); ++j) {
                    string stack_block = stack[j];
                    int stack_position = find(stack.begin(), stack.end(), stack_block) - stack.begin();

                    // If the block is not at the bottom of the stack
                    if (stack_position != 0) {
                        // Find the corresponding block in other stacks
                        for (const auto& other_stack : goal_state) {
                            if (find(other_stack.begin(), other_stack.end(), stack_block) != other_stack.end()) {
                                int other_position = find(other_stack.begin(), other_stack.end(), stack_block) - other_stack.begin();
                                // Count the number of blocks above this block in the other stack
                                for (size_t k = 0; k < stack_position; ++k) {
                                    string other_block = stack[k];
                                    int other_block_position = find(other_stack.begin(), other_stack.end(), other_block) - other_stack.begin();
                                    if (other_block_position < other_position) {
                                        sum += 1*i; // old was sum+=1
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                break;
            }
        }
    }

    return sum;
}






















State apply_action( const State& current_state, const action& action_to_apply){
    
 State next_state = current_state ; 
 
 int start = action_to_apply.start ; 
 int end = action_to_apply.end ; 
 string moved_block = action_to_apply.start_block ;
string end_block = action_to_apply.end_block ; 

 next_state.state[start].pop_back() ;  // pop the moved block 
 //next_state.print_state();
if(end_block == "table"){
    vector<string> stack ={moved_block} ;  // create a new stack in case of a table 
    next_state.state.push_back(stack) ; // push the new stack with the one element into state ( array of stacks )
    next_state.state=  sortByStackSize(next_state.state) ;

    next_state.update_stacks(next_state.state) ; //update the length of the next state 
} else {
next_state.state[end].push_back(moved_block) ;   // add the moved block to the target stack  
 next_state.state=  sortByStackSize(next_state.state) ; // everytime i apply an action i sort the stack and remove empty locations 
 next_state.update_stacks(next_state.state) ;  // update the length of the new state 

}

return next_state ; 

}
vector<action> possibleActions(vector<vector<string>> &x) {
   
    vector<action> result ; 
    for (int i = 0; i < x.size(); ++i) {
        cout<<" SIZE "<<x.size() ; 
        // Skip empty stacks
        if (x[i].empty()) {
            continue;
        }

        // Get the index of the top block in the current stack
        int top_index = x[i].size() - 1;
        string top_block = x[i][top_index];

        // Iterate through other stacks
        for (int j = 0; j < x.size(); ++j) {
            // Exclude the same stack
            if (j != i) {
                // Append action (index of current stack, block being moved, index of other stack)
                result.push_back({i, top_block, j, (x[j].empty() ? "" : x[j].back())});
            }
        }
    }
    for(int i =  0 ; i<x.size();i++){
       if (x[i].empty()) {
            continue;
        }
        int top_index = x[i].size()-1 ; 
        string top_block = x[i][top_index] ; 
        int n = x.size() ; 
        string table = "table" ; 
         if(x[i].size()>1) // if the size of the stack is greater than 1 element 
         result.push_back({i,top_block,n,table}) ; // make sure when handling the action taken to on table we add a new stack 
         // cout<<"the nn table " <<n ; 
        // cout << "Move block " << action.start_block << " from stack " << action.start << " to stack " << action.end;
        // cout << " (on Top of " << action.end_block << ")" << endl;






    }
    return result ;
 
    
}
int findindex(const vector<int>& vec, int element) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == element) {
            return static_cast<int>(i); // Return the index as an integer
        }
    }
    // If the element is not found, return -1
    return -1;
}

vector<vector<string>> parseString(const string& input) {
    vector<vector<string>> result;

    istringstream iss(input);
    string token;
    
    // Split the input string using ';' as delimiter
    while (getline(iss, token, ';')) {
        vector<string> innerVector;
        istringstream innerIss(token);
        string innerToken;

        // Split the token using ',' as delimiter
        while (getline(innerIss, innerToken, ',')) {
            innerVector.push_back(innerToken);
        }
        
        result.push_back(innerVector);
    }

    return result;
}



  int  A_star_step_result(State x, vector<vector<string>> goal_state) {
    State best_state = x;
    x.actions_list = possibleActions(x.state);
     int s = x.actions_list.size();
    vector<int> arr(s);
    vector<State> possible_next_states(s);
    vector<int> possible_next_states_hvalue(s);
    cout << "\n SIZE OF ACTIONS " << s << "\n";
    int Astar_index = 0;
    int v = 10000 ; 
    int p =0 ; 
    cout << "inside A_star_step\n";
    if (x.actions_list.size() != 0) {
        for (int i = 0; i < s; i++) {
            possible_next_states[i] = apply_action(x, x.actions_list[i]);
            cout << "action " << i << "\n";
            possible_next_states[i].print_state();
        }
        cout << "done the possible states\n\n";
    }  
    
   
    if (s != 0) {
        for (int i = 0; i < s; i++) {
            cout << "state " << i;
            possible_next_states[i].print_state();

           int v = heuristic(possible_next_states[i].state,goal_state);
            cout << "v is " << v << " index is " << i << "\n";
          possible_next_states[i].heuristic_value = v;
            possible_next_states_hvalue[i] = v;
            arr[i] = v;
        }
        cout << "\n yay \n";
        //  Astar_index = getIndexOfSmallestInteger(&arr[0], s);
          for (int i = 1; i < s; ++i) {
        if (arr[i] < arr[p]) {
            p = i; // Update the index of the smallest element
        }
        Astar_index = p ; 
    }
        for (int i = 0; i < s; i++) {
            cout << "index " << i << " of children, value = " << arr[i] << "\n";
        }
        cout<<"ASTAR_INDEX \n  = " << Astar_index<<"\n" ; 
        
        if(Astar_index>=s){
            cout<<"\n \n \n ERROR EEEEEEEEEee \n \n \n" ; 
            return -1 ;  // do again 
        }
            if (Astar_index < s) {
                cout<<"\n ENTERED \n" ; 
                return Astar_index;
            } 
        
    } 
}

bool goal_check(const vector<vector<string>>& current_state, const vector<vector<string>>& goal_state) {
    // Check if the number of stacks is the same in both states
    if (current_state.size() != goal_state.size()) {
        return false; // If not, they can't be in the same position
    }

    // Vector to keep track of whether a goal stack has been matched
    vector<bool> goal_stack_matched(goal_state.size(), false);

    // Iterate through each stack in the current state
    for (size_t i = 0; i < current_state.size(); ++i) {
        bool stack_matched = false;

        // Iterate through each stack in the goal state
        for (size_t j = 0; j < goal_state.size(); ++j) {
            // If the goal stack has already been matched, skip it
            if (goal_stack_matched[j]) {
                continue;
            }

            // Check if the size of the current stack matches the size of the corresponding stack in the goal state
            if (current_state[i].size() == goal_state[j].size()) {
                // Check if the content of the current stack matches the content of the goal stack
                bool content_match = true;
                for (size_t k = 0; k < current_state[i].size(); ++k) {
                    if (current_state[i][k] != goal_state[j][k]) {
                        content_match = false;
                        break;
                    }
                }

                // If the content of the stacks match, mark the goal stack as matched
                if (content_match) {
                    goal_stack_matched[j] = true;
                    stack_matched = true;
                    break; // Move to the next stack in the current state
                }
            }
        }

        // If no match was found for the current stack in the goal state, return false
        if (!stack_matched) {
            return false;
        }
    }

    // If all stacks in the current state have a corresponding match in the goal state, return true
    return true;
}






















void runAlgorithm(TaskSubscriber* node)
{
    // Implement your algorithm here
    std::string result = "Optimal moves calculated for initial string: '" + node->initial_string_
                         + "' and goal string: '" + node->goal_string_ + "'";


  initial_state = parseString(node->initial_string_) ; 
  goal_state = parseString(node->goal_string_) ; 

initial_state=sortByStackSize(initial_state); 
  goal_state=sortByStackSize(goal_state) ; 
 vector<State> Visited_states ; 
     action move ; 
     vector<action> moves_array ; //execeuted action  
     goal_state = sortByStackSize((goal_state));

 State initial(initial_state);
  initial.print_state(); 

    int h= heuristic(initial_state,goal_state) ; 
    cout<<h; 
    int i = 0 ; 
   initial.actions_list= possibleActions(initial.state) ; 
    initial.printActions();
State u = initial; 
int A_Index = 0 ; // A* best index
int P_Index = 0 ;  // path index
int number_of_moves = 0 ; 
int optimal_moves = 5 ; 

while(goal_check(u.state, goal_state)== false){
Visited_states.push_back(u) ; 
A_Index = A_star_step_result(u,goal_state) ;  // index of the best action /state 
move = u.actions_list[A_Index] ; 
moves_array.push_back(move) ; // push the best action in the moves list 
cout << "Move block " << move.start_block << " from stack " << move.start << " to stack " << move.end;   //print the move
cout << " (on Top of " << move.end_block << ")" << endl;
u = apply_action(u,move) ; // the next state is 
P_Index++;
cout<<P_Index<<" state is \n "; 
u.print_state() ; 
cout<<" possible actions \n " ; 
u.actions_list =  possibleActions(u.state) ;  
u.printActions() ; 
if(P_Index>1000){
    cout<<" FAILEEED" ; 
    
    exit(0) ; 
}
}
Visited_states.push_back(u) ; 

cout<<"\n \n OPTIMAL  SOLUTION" ;
for(int i =0 ; i<Visited_states.size()-1;i++){
    Visited_states[i].print_state() ; 
    cout << "Move block " << moves_array[i].start_block << " from stack " << moves_array[i].start << " to stack " << moves_array[i].end;   //print the move
cout << " (on Top of " << moves_array[i].end_block << ")" << endl;

}

Visited_states[Visited_states.size()-1].print_state();
cout<<"GOAL STATE in  "<< Visited_states.size()-1<<" MOVES" ; 


cout<< " WINNNNNN" ; 





    node->publishResult(result);
}

































int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TaskSubscriber>();

    // Spin the node in a separate thread
    std::thread spin_thread([&node]() {
        rclcpp::spin(node);
    });

    // Wait for both strings to be received (extended wait with periodic checks)
    const int max_wait_time_seconds = 30;
    const int check_interval_ms = 500;
    int waited_time = 0;

    while (waited_time < max_wait_time_seconds * 1000 && (node->initial_string_.empty() || node->goal_string_.empty()))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(check_interval_ms));
        waited_time += check_interval_ms;
    }

    // Run the algorithm if both strings are received
    if (!node->initial_string_.empty() && !node->goal_string_.empty())
    {
        runAlgorithm(node.get());
    }
    else
    {
        RCLCPP_WARN(node->get_logger(), "Initial or goal string not received within the timeout period.");
    }

    rclcpp::shutdown();
    spin_thread.join();
    return 0;
}











































/*
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include<algorithm>
#include <utility>
#include<sstream> 
using namespace std;

// vector<vector<string>> initial_state = {{"A", "B", "C"}, {"D","E"}}; // it works 
// vector<vector<string>> goal_state = {{"A","B","C","E","D"}}; // it works 
    vector<vector<string>> initial_state ; 
    vector<vector<string>> goal_state ; 

class TaskSubscriber : public rclcpp::Node
{
public:
    TaskSubscriber() : Node("task_subscriber")
    {
        // Subscribe to the input string topics
        initial_subscription_ = this->create_subscription<std_msgs::msg::String>(
            "initial_string", 10,
            [this](const std_msgs::msg::String::SharedPtr msg) {
                // Callback function for incoming initial string message
                RCLCPP_INFO(this->get_logger(), "Received initial string: '%s'", msg->data.c_str());
                initial_string_ = msg->data;
                processStrings();
            });

        goal_subscription_ = this->create_subscription<std_msgs::msg::String>(
            "goal_string", 10,
            [this](const std_msgs::msg::String::SharedPtr msg) {
                // Callback function for incoming goal string message
                RCLCPP_INFO(this->get_logger(), "Received goal string: '%s'", msg->data.c_str());
                goal_string_ = msg->data;
                processStrings();
            });

        // Create a publisher for the output string topic
        publisher_ = this->create_publisher<std_msgs::msg::String>("optimal_moves", 10);
    }

    std::string getInitialString() const
    {
        return initial_string_;
    }

    std::string getGoalString() const
    {
        return goal_string_;
    }

    void publishResult(const std::string &result)
    {
        std_msgs::msg::String output_msg;
        output_msg.data = result;
        publisher_->publish(output_msg);
    }

    std::string initial_string_;
    std::string goal_string_;

private:
    void processStrings()
    {
        // Check if both initial and goal strings are received
        if (!initial_string_.empty() && !goal_string_.empty())
        {
            // Strings are received and stored, ready for processing
            RCLCPP_INFO(this->get_logger(), "Both strings received, ready for processing.");
        }
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr initial_subscription_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr goal_subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};



////////////////////

   
struct action {
int start ; 
string start_block ; 
int end ; 
string end_block ;    // ana action entity that stores the possible action / move taken 
// a state have a list of current possible action  
};

class State {
public:
    vector<vector<string>> state;
    vector<string> moves;
    int cost;
    int heuristic_value;
    int stacks;
    int best_index ; 
    vector<int> stack_lengths;
    vector<action> actions_list ;   // list of possible actions 
    vector<int> children_indices ; 
    
    State(const vector<vector<string>>& initial_state) {
        update_stacks(initial_state);
    }
    State() {
    // Initialize member variables or leave them uninitialized
}

    void update_stack_lengths() {
        stack_lengths.resize(stacks);
        for (int i = 0; i < stacks; ++i) {
            stack_lengths[i] = state[i].size();
        }
    }

    void update_stacks(const vector<vector<string>>& new_state) {
        state = new_state;
        stacks = state.size();
        update_stack_lengths();
    }

    vector<vector<string>>& get_stacks() {
        return state;
    }

    bool operator<(const State& other) const {
        return (cost + heuristic_value) > (other.cost + other.heuristic_value);
    }









void print_state( ){
vector<vector<string>> initial_stacks = state ; 
cout<<" \n" ;
 for (int i = 0; i < initial_stacks.size(); ++i) {
        cout << "  Stack " << i << ": ";
        for (const string& block : initial_stacks[i]) { // do a print state function 
            cout << block << " ";
        }
        cout << endl;
    }




}





void printActions() {
    cout << " \n List of Possible Actions:" << endl;
    for (const auto& action : actions_list) {
        cout << "Move block " << action.start_block << " from stack " << action.start << " to stack " << action.end;
        cout << " (on Top of " << action.end_block << ")" << endl;
    }
}






};


void removeEmptyStacks(vector<vector<string>>& stacks) {
    for (auto it = stacks.begin(); it != stacks.end();) {
        if (it->empty()) {
            it = stacks.erase(it);
        } else {
            ++it;
        }
    }
}
// Function to sort the stacks by their sizes from bigger to smaller using bubble sort
vector<vector<string>> sortByStackSize(vector<vector<string>>& stacks) {
    // Remove empty stacks
    removeEmptyStacks(stacks);

    // Bubble sort the stacks by size
    bool swapped;
    do {
        swapped = false;
        for (size_t i = 0; i < stacks.size() - 1; ++i) {
            if (stacks[i].size() < stacks[i + 1].size()) {
                swap(stacks[i], stacks[i + 1]);
                swapped = true;
            }
        }
    } while (swapped);

    return stacks;
}*/





/*
int heuristic(vector<vector<string>>& current_state, vector<vector<string>>& goal_state) {
    int sum = 0;

    // Iterate through each stack in the current state
    for (const auto& stack : current_state) {
        // Find the corresponding goal stack
        vector<string> goal_stack;
        for (const auto& other_stack : goal_state) {
            if (!stack.empty() && find(other_stack.begin(), other_stack.end(), stack[0]) != other_stack.end()) {
                goal_stack = other_stack;
                break;
            }
        }

        // Iterate through each block in the stack
        for (size_t i = 0; i < stack.size(); ++i) {
            string block = stack[i];
            int block_position = i;

            // If the block is in the goal stack
            if (!goal_stack.empty() && find(goal_stack.begin(), goal_stack.end(), block) != goal_stack.end()) {
                int goal_block_position = find(goal_stack.begin(), goal_stack.end(), block) - goal_stack.begin();

                // If the block is already in its correct position
                if (block_position == goal_block_position) {
                   sum=sum-1;  // important 
                    continue;
                }

                // Calculate the number of moves needed
                sum += stack.size() - block_position;

                // Move the block to its correct position
                for (size_t j = block_position; j < stack.size(); ++j) {
                    string stack_block = stack[j];
                    int stack_position = find(stack.begin(), stack.end(), stack_block) - stack.begin();

                    // If the block is not at the bottom of the stack
                    if (stack_position != 0) {
                        // Find the corresponding block in other stacks
                        for (const auto& other_stack : goal_state) {
                            if (find(other_stack.begin(), other_stack.end(), stack_block) != other_stack.end()) {
                                int other_position = find(other_stack.begin(), other_stack.end(), stack_block) - other_stack.begin();
                                // Count the number of blocks above this block in the other stack
                                for (size_t k = 0; k < stack_position; ++k) {
                                    string other_block = stack[k];
                                    int other_block_position = find(other_stack.begin(), other_stack.end(), other_block) - other_stack.begin();
                                    if (other_block_position < other_position) {
                                        sum += 1*i; // old was sum+=1
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                break;
            }
        }
    }

    return sum;
}



*/


/*
pair<int, int> findBlockPosition(const vector<vector<string>>& state, const string& block) {
    for (int i = 0; i < static_cast<int>(state.size()); ++i) {
        auto it = find(state[i].begin(), state[i].end(), block);
        if (it != state[i].end()) {
            return {i, static_cast<int>(distance(state[i].begin(), it))};
        }
    }
    return {-1, -1}; // Not found
}

int heuristic(const vector<vector<string>>& current_state, const vector<vector<string>>& goal_state) {
    int sum = 0;
    // hill climbing algorithm   
    for (const auto& stack : current_state) {
        size_t stack_size = stack.size();
        vector<bool> correct_structure(stack_size, true); // Initialize to true for each stack

        for (size_t i = 0; i < stack_size; ++i) {
            const string& block = stack[i];
            // asuming that the block string is unique 
            // Find the position of the block in the goal state
            auto [goal_stack, goal_position] = findBlockPosition(goal_state, block);  // structural binding ind modern C++ 

            // Check if the block is found in the goal state
            if (goal_stack == -1 || goal_position == -1) {
                cerr << "Block " << block << " not found in the goal state." << endl;
                correct_structure[i] = false;
                continue;
            }

            // Determine if the block is placed correctly
            if (i == 0) {
                // Check if the first block should be on the table in the goal state
                if (goal_position != 0) {
                    correct_structure[i] = false; // The first block is not correctly placed
                }
            } else {
                const string& current_below_block = stack[i - 1];
                auto [goal_below_stack, goal_below_position] = findBlockPosition(goal_state, current_below_block);

                if (goal_below_stack == goal_stack && goal_below_position == goal_position - 1) {
                    correct_structure[i] = correct_structure[i - 1]; // Continue the chain of correctness
                } else {
                    correct_structure[i] = false; // Break the chain of correctness
                }
            }
        }

        // Calculate the heuristic value for the stack
        for (size_t i = 0; i < stack_size; ++i) {
            if (correct_structure[i]) {
                sum += static_cast<int>(i) * -1;
            } else {
                sum += static_cast<int>(i) * 1;
            }

           // cout << "Block: " << stack[i] << ", Index: " << i << ", Correct: " << correct_structure[i] << ", Sum: " << sum << endl;
        }
    }

    return sum;
}   // the last triallll isa sa7















State apply_action( const State& current_state, const action& action_to_apply){
    
 State next_state = current_state ; 
 
 int start = action_to_apply.start ; 
 int end = action_to_apply.end ; 
 string moved_block = action_to_apply.start_block ;
string end_block = action_to_apply.end_block ; 

 next_state.state[start].pop_back() ;  // pop the moved block 
 //next_state.print_state();
if(end_block == "table"){
    vector<string> stack ={moved_block} ;  // create a new stack in case of a table 
    next_state.state.push_back(stack) ; // push the new stack with the one element into state ( array of stacks )
    next_state.state=  sortByStackSize(next_state.state) ;

    next_state.update_stacks(next_state.state) ; //update the length of the next state 
} else {
next_state.state[end].push_back(moved_block) ;   // add the moved block to the target stack  
 next_state.state=  sortByStackSize(next_state.state) ; // everytime i apply an action i sort the stack and remove empty locations 
 next_state.update_stacks(next_state.state) ;  // update the length of the new state 

}

return next_state ; 

}
vector<action> possibleActions(vector<vector<string>> &x) {
   
    vector<action> result ; 
    for (int i = 0; i < x.size(); ++i) {
        cout<<" SIZE "<<x.size() ; 
        // Skip empty stacks
        if (x[i].empty()) {
            continue;
        }

        // Get the index of the top block in the current stack
        int top_index = x[i].size() - 1;
        string top_block = x[i][top_index];

        // Iterate through other stacks
        for (int j = 0; j < x.size(); ++j) {
            // Exclude the same stack
            if (j != i) {
                // Append action (index of current stack, block being moved, index of other stack)
                result.push_back({i, top_block, j, (x[j].empty() ? "" : x[j].back())});
            }
        }
    }
    for(int i =  0 ; i<x.size();i++){
       if (x[i].empty()) {
            continue;
        }
        int top_index = x[i].size()-1 ; 
        string top_block = x[i][top_index] ; 
        int n = x.size() ; 
        string table = "table" ; 
         if(x[i].size()>1) // if the size of the stack is greater than 1 element 
         result.push_back({i,top_block,n,table}) ; // make sure when handling the action taken to on table we add a new stack 
         // cout<<"the nn table " <<n ; 
        // cout << "Move block " << action.start_block << " from stack " << action.start << " to stack " << action.end;
        // cout << " (on Top of " << action.end_block << ")" << endl;

    }
  
    return result ;
 
    
}


int findindex(const vector<int>& vec, int element) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == element) {
            return static_cast<int>(i); // Return the index as an integer
        }
    }
    // If the element is not found, return -1
    return -1;
}

vector<vector<string>> parseString(const string& input) {
    vector<vector<string>> result;

    istringstream iss(input);
    string token;
    
    // Split the input string using ';' as delimiter
    while (getline(iss, token, ';')) {
        vector<string> innerVector;
        istringstream innerIss(token);
        string innerToken;

        // Split the token using ',' as delimiter
        while (getline(innerIss, innerToken, ',')) {
            innerVector.push_back(innerToken);
        }
        
        result.push_back(innerVector);
    }

    return result;
}


bool goal_check(const vector<vector<string>>& current_state, const vector<vector<string>>& goal_state) {
    // Check if the number of stacks is the same in both states
    if (current_state.size() != goal_state.size()) {
        return false; // If not, they can't be in the same position
    }

    // Vector to keep track of whether a goal stack has been matched
    vector<bool> goal_stack_matched(goal_state.size(), false);

    // Iterate through each stack in the current state
    for (size_t i = 0; i < current_state.size(); ++i) {
        bool stack_matched = false;

        // Iterate through each stack in the goal state
        for (size_t j = 0; j < goal_state.size(); ++j) {
            // If the goal stack has already been matched, skip it
            if (goal_stack_matched[j]) {
                continue;
            }

            // Check if the size of the current stack matches the size of the corresponding stack in the goal state
            if (current_state[i].size() == goal_state[j].size()) {
                // Check if the content of the current stack matches the content of the goal stack
                bool content_match = true;
                for (size_t k = 0; k < current_state[i].size(); ++k) {
                    if (current_state[i][k] != goal_state[j][k]) {
                        content_match = false;
                        break;
                    }
                }

                // If the content of the stacks match, mark the goal stack as matched
                if (content_match) {
                    goal_stack_matched[j] = true;
                    stack_matched = true;
                    break; // Move to the next stack in the current state
                }
            }
        }

        // If no match was found for the current stack in the goal state, return false
        if (!stack_matched) {
            return false;
        }
    }

    // If all stacks in the current state have a corresponding match in the goal state, return true
    return true;
}

std::vector<action> remove_goingback_actions(State current, vector<action> possible_action_list, const vector<State>& visited_before) {
    vector<action> new_action_list;
    State next_state;

    for (const auto& act : possible_action_list) {
        next_state = apply_action(current, act);
        bool is_visited = false;

        for (const auto& visited : visited_before) {
            if (goal_check(next_state.state, visited.state)) { // goal check here checls the similar states not the goal 
                is_visited = true;
                break;
            }
        }

        if (!is_visited) {
            new_action_list.push_back(act);
        }
    }

    return new_action_list;
}


int  A_star_step_result(State x, vector<vector<string>> goal_state) {
    State best_state = x;
    x.actions_list = possibleActions(x.state);
     int s = x.actions_list.size();
    vector<int> arr(s);
    vector<State> possible_next_states(s);
    vector<int> possible_next_states_hvalue(s);
    cout << "\n SIZE OF ACTIONS " << s << "\n";
    int Astar_index = 0;
    int v = 10000 ; 
    int p =0 ; 
    cout << "inside A_star_step\n";
    if (x.actions_list.size() != 0) {
        for (int i = 0; i < s; i++) {
            possible_next_states[i] = apply_action(x, x.actions_list[i]);
            cout << "action " << i << "\n";
            possible_next_states[i].print_state();
        }
        cout << "done the possible states\n\n";
    }  
    
   
    if (s != 0) {
        for (int i = 0; i < s; i++) {
            cout << "state " << i;
            possible_next_states[i].print_state();

           int v = heuristic(possible_next_states[i].state,goal_state);
            cout << "v is " << v << " index is " << i << "\n";
          possible_next_states[i].heuristic_value = v;
            possible_next_states_hvalue[i] = v;
            arr[i] = v;
        }
        cout << "\n yay \n";
        //  Astar_index = getIndexOfSmallestInteger(&arr[0], s);
          for (int i = 1; i < s; ++i) {
        if (arr[i] < arr[p]) {
            p = i; // Update the index of the smallest element
        }
        Astar_index = p ; 
    }
        for (int i = 0; i < s; i++) {
            cout << "index " << i << " of children, value = " << arr[i] << "\n";
        }
        cout<<"ASTAR_INDEX \n  = " << Astar_index<<"\n" ; 
        
        if(Astar_index>=s){
            cout<<"\n \n \n ERROR EEEEEEEEEee \n \n \n" ; 
            return -1 ;  // do again 
        }
            if (Astar_index < s) {
                cout<<"\n ENTERED \n" ; 
                return Astar_index;
            }else return -1 ;  
        
    }else return -1 ;  
}
































void runAlgorithm(TaskSubscriber* node)
{
    // Implement your algorithm here
    std::string result = "Optimal moves calculated for initial string: '" + node->initial_string_
                         + "' and goal string: '" + node->goal_string_ + "'";


  initial_state = parseString(node->initial_string_) ; 
  goal_state = parseString(node->goal_string_) ; 

initial_state=sortByStackSize(initial_state); 
  goal_state=sortByStackSize(goal_state) ; 
 vector<State> Visited_states ; 
     action move ; 
     vector<action> moves_array ; //execeuted action  
     goal_state = sortByStackSize((goal_state));

 State initial(initial_state);
  initial.print_state(); 

    int h= heuristic(initial_state,goal_state) ; 
    cout<<h; 
    int i = 0 ; 
   initial.actions_list= possibleActions(initial.state) ; 
    initial.printActions();
State u = initial; 
int A_Index = 0 ; // A* best index
int P_Index = 0 ;  // path index
int number_of_moves = 0 ; 
int optimal_moves = 5 ; 

while(goal_check(u.state, goal_state)== false){
Visited_states.push_back(u) ; 
A_Index = A_star_step_result(u,goal_state) ;  // index of the best action /state 
if(A_Index ==-1)
continue; 
move = u.actions_list[A_Index] ; 
moves_array.push_back(move) ; // push the best action in the moves list 
cout << "Move block " << move.start_block << " from stack " << move.start << " to stack " << move.end;   //print the move
cout << " (on Top of " << move.end_block << ")" << endl;
u = apply_action(u,move) ; // the next state is 
P_Index++;
cout<<P_Index<<" state is \n "; 
u.print_state() ; 
cout<<" possible actions \n " ; 
u.actions_list =  possibleActions(u.state) ;  
u.printActions() ; 
if(P_Index>1000){
    cout<<" FAILEEED" ; 
    
    exit(0) ; 
}
}
Visited_states.push_back(u) ; 

cout<<"\n \n OPTIMAL  SOLUTION" ;
for(int i =0 ; i<Visited_states.size()-1;i++){
    Visited_states[i].print_state() ; 
    cout << "Move block " << moves_array[i].start_block << " from stack " << moves_array[i].start << " to stack " << moves_array[i].end;   //print the move
cout << " (on Top of " << moves_array[i].end_block << ")" << endl;

}

Visited_states[Visited_states.size()-1].print_state();
cout<<"GOAL STATE in  "<< Visited_states.size()-1<<" MOVES" ; 


cout<< " WINNNNNN" ; 






    node->publishResult(result);
}

































int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TaskSubscriber>();

    // Spin the node in a separate thread
    std::thread spin_thread([&node]() {
        rclcpp::spin(node);
    });

    // Wait for both strings to be received (extended wait with periodic checks)
    const int max_wait_time_seconds = 30;
    const int check_interval_ms = 500;
    int waited_time = 0;

    while (waited_time < max_wait_time_seconds * 1000 && (node->initial_string_.empty() || node->goal_string_.empty()))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(check_interval_ms));
        waited_time += check_interval_ms;
    }

    // Run the algorithm if both strings are received
    if (!node->initial_string_.empty() && !node->goal_string_.empty())
    {
        runAlgorithm(node.get());
    }
    else
    {
        RCLCPP_WARN(node->get_logger(), "Initial or goal string not received within the timeout period.");
    }

    rclcpp::shutdown();
    spin_thread.join();
    return 0;
}
*/