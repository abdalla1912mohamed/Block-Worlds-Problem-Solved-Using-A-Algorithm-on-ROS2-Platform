#!/usr/bin/env python3 
import rclpy 
from rclpy.node import Node 
from geometry_msgs.msg import Twist 
from std_msgs.msg import String
class Taskpublisher (Node) : 
    def __init__(self):
        super().__init__("TASK_PUBLISHER_NODE")
     #   self.cmd_vel_pub = self.create_publisher(Twist,"/turtle1/cmd_vel",10) 
        self.publisher_ = self.create_publisher(String,"initial_string",10)
        self.publisher_1 = self.create_publisher(String,"goal_string",10)
        
        self.timer = self.create_timer(1,self.send_string_command)

    def send_string_command(self):
           char_array_2d = [['A', 'B', 'C'], ['D', 'E']]
           char_array_goal = [['A', 'C'], ['D', 'E', 'B']]  # sender file 
           msg = String()
           msg1 = String()
           msg.data = ';'.join([','.join(array) for array in char_array_2d])                                 # change to string 
           self.publisher_.publish(msg)
           self.get_logger().info(f'Published initial: {msg.data}')
           msg1.data = ';'.join([','.join(array) for array in char_array_goal])
           self.publisher_1.publish(msg1)
           self.get_logger().info(f'Published GOAL : {msg1.data}')
           self.timer.cancel()
           self.timer_executed = True
    

"""

  vector<vector<string>> initial_arrangement_1 = {{"A", "B", "C"}, {"D", "E"}};
    vector<vector<string>> goal_arrangement_1 = {{"A", "C"}, {"D", "E", "B"}}; // pass with 3 optimal

    vector<vector<string>> initial_arrangement_2 = {{"D", "H", "B", "E"}, {"G", "A"}, {"I", "C", "J", "F"}};
    vector<vector<string>> goal_arrangement_2 = {{"D", "C"}, {"G", "E"}, {"F", "B", "A", "H"}, {"J", "I"}}; // passs 9  optimal

    vector<vector<string>> initial_arrangement_3 = {{"E", "N", "H", "O"}, {"L", "F", "K", "B"}, {"M", "C"}, {"A", "I", "J"}, {"G", "D"}};
    vector<vector<string>> goal_arrangement_3 = {{"O", "A", "K", "D", "J", "N"}, {"I"}, {"F", "H", "C"}, {"E", "L", "M"}, {"G", "B"}}; // pass 17 opitmal

    vector<vector<string>> initial_arrangement_4 = {{"D", "I"}, {"J", "K", "E", "A", "H", "C", "G"}, {"F", "B"}};
    vector<vector<string>> goal_arrangement_4 = {{"A", "C", "B"}, {"G"}, {"K"}, {"F", "H", "I", "E", "J", "D"}}; pass 13  opitmal

    vector<vector<string>> initial_arrangement_5 = {{"D", "G", "C", "P", "J", "O", "I", "H"}, {"A", "Q", "M", "K"}, {"L", "F"}, {"B", "N", "E"}};
    vector<vector<string>> goal_arrangement_5 = {{"H", "B"}, {"E", "P", "D", "K"}, {"O", "Q", "G"}, {"N", "M", "C"}, {"F", "I", "J"}, {"L", "A"}}; //22 pass not optimal 19 

    vector<vector<string>> initial_arrangement_6 = {{"F", "B", "C"}, {"H", "I"}, {"M", "E", "J"}, {"L", "D", "A", "G", "K"}}; //pass 16 optimal 14 
    vector<vector<string>> goal_arrangement_6 = {{"D", "H", "J", "E", "C"}, {"L"}, {"G", "B", "M"}, {"I", "A", "F"}, {"K"}};

    vector<vector<string>> initial_arrangement_7 = {{"J", "B", "A", "E", "O", "N"}, {"I", "G", "K", "P", "Q", "D", "L", "H"}, {"F"}, {"M", "C"}};
    vector<vector<string>> goal_arrangement_7 = {{"O", "E", "H", "L"}, {"A"}, {"M", "I", "F"}, {"B", "K", "G", "N"}, {"P"}, {"J", "Q", "D"}, {"C"}}; // 21 pass optimal 17 

    vector<vector<string>> initial_arrangement_8 = {{"O", "B", "D"}, {"F", "Q", "I", "P", "A"}, {"C", "E", "M", "G"}, {"R", "N", "L"}, {"H", "J"}, {"K"}};
    vector<vector<string>> goal_arrangement_8 = {{"M", "G", "I", "O"}, {"J", "L", "P"}, {"H", "Q", "D", "E", "C", "B"}, {"N", "K"}, {"R", "A", "F"}}; // 25 pass optimal 19

    vector<vector<string>> initial_arrangement_9 = {{"C", "B", "F", "O", "D"}, {"I", "L", "H", "G", "P", "K", "N"}, {"J", "E"}, {"A", "M"}}; //20 pass  opitmal 19 
    vector<vector<string>> goal_arrangement_9 = {{"A", "K"}, {"C", "G", "F", "O"}, {"D", "H", "J"}, {"B", "I"}, {"M", "P"}, {"E", "L"}, {"N"}};

    vector<vector<string>> initial_arrangement_10 = {{"J", "H", "I", "D", "O", "P", "B", "F", "G", "C", "E", "K", "A"}, {"Q", "L"}, {"N", "M"}};
    vector<vector<string>> goal_arrangement_10 = {{"P", "N", "H", "C"}, {"O", "K", "Q", "J", "D", "B", "I"}, {"M", "E", "A"}, {"G", "L"}, {"F"}}; // 25 pass opitmal : 23

    vector<vector<string>> initial_arrangement_11 = {{"A", "B", "C"}, {"D", "E"}}; // 5 optimal 
    vector<vector<string>> goal_arrangement_11 = {{"D", "E", "A", "B", "C"}};

    vector<vector<string>> initial_arrangement_12 = {{"A", "B", "C"}, {"D", "E", "F"}, {"G", "H", "I"}};
    vector<vector<string>> goal_arrangement_12 = {{"I", "H", "G", "F", "E", "D", "C", "B", "A"}}; // 11  optimal = 9  

    vector<vector<string>> initial_arrangement_13 = {{"A", "B", "C"}, {"D", "E", "F"}, {"G", "H", "I"}};
    vector<vector<string>> goal_arrangement_13 = {{"F", "D", "C", "I", "G", "A"}, {"B", "E", "H"}}; pass 13 optimal = 11 

"""


"""def test():
    initial_arrangement_1  =  [['A', 'B', 'C'], ['D', 'E']]
    goal_arrangement_1  =  [['A', 'C'], ['D', 'E', 'B']]      // wins in optimaal 5 
    optimal_movements_1= [('C', 'Table'), ('B', 'E'), ('C', 'A')]

    initial_arrangement_2  =[['D', 'H', 'B', 'E'], ['G', 'A'], ['I', 'C', 'J', 'F']]  // wins not optimal
    goal_arrangement_2 = [['D', 'C'], ['G', 'E'], ['F', 'B', 'A', 'H'], ['J', 'I']]
    optimal_movements_2 =  [('A', 'Table'), ('E', 'G'), ('F', 'Table'), ('B', 'F'), ('A', 'B'), ('H', 'A'), ('J', 'Table'), ('C', 'D'), ('I', 'J')]

    initial_arrangement_3 = [['E', 'N', 'H', 'O'], ['L', 'F', 'K', 'B'], ['M', 'C'], ['A', 'I', 'J'], ['G', 'D']] 
    goal_arrangement_3 = [['O', 'A', 'K', 'D', 'J', 'N'], ['I'], ['F', 'H', 'C'], ['E', 'L', 'M'], ['G', 'B']]
    optimal_movements_3  = [('J', 'Table'), ('I', 'Table'), ('O', 'Table'), ('A', 'O'), ('B', 'Table'), ('K', 'A'), ('D', 'K'), ('B', 'G'), ('J', 'D'), ('H', 'Table'), ('N', 'J'), ('F', 'Table'), ('L', 'E'), ('H', 'F'), ('C', 'H'), ('M', 'L')]

    initial_arrangement_4 = [['D', 'I'], ['J', 'K', 'E', 'A', 'H', 'C', 'G'], ['F', 'B']] 
    goal_arrangement_4 = [['A', 'C', 'B'], ['G'], ['K'], ['F', 'H', 'I', 'E', 'J', 'D']]
    optimal_movements_4  = [('G', 'Table'), ('I', 'Table'), ('C', 'D'), ('B', 'C'), ('H', 'F'), ('I', 'H'), ('A', 'Table'), ('E', 'I'), ('K', 'Table'), ('J', 'E'), ('B', 'Table'), ('C', 'A'), ('D', 'J'), ('B', 'C')]

    initial_arrangement_5 = [['D', 'G', 'C', 'P', 'J', 'O', 'I', 'H'], ['A', 'Q', 'M', 'K'], ['L', 'F'], ['B', 'N', 'E']]
    goal_arrangement_5 = [['H', 'B'], ['E', 'P', 'D', 'K'], ['O', 'Q', 'G'], ['N', 'M', 'C'], ['F', 'I', 'J'], ['L', 'A']] 
    optimal_movements_5  = [('H', 'Table'), ('F', 'Table'), ('I', 'F'), ('O', 'Table'), ('J', 'I'), ('E', 'Table'), ('P', 'E'), ('N', 'Table'), ('B', 'H'), ('K', 'Table'), ('M', 'N'), ('C', 'M'), ('G', 'Q'), ('D', 'P'), ('K', 'D'), ('G', 'Table'), ('Q', 'O'), ('A', 'L'), ('G', 'Q')]

    initial_arrangement_6 = [['F', 'B', 'C'], ['H', 'I'], ['M', 'E', 'J'], ['L', 'D', 'A', 'G', 'K']] 
    goal_arrangement_6 = [['D', 'H', 'J', 'E', 'C'], ['L'], ['G', 'B', 'M'], ['I', 'A', 'F'], ['K']]
    optimal_movements_6  = [('K', 'Table'), ('C', 'Table'), ('G', 'Table'), ('B', 'G'), ('A', 'Table'), ('D', 'Table'), ('I', 'Table'), ('H', 'D'), ('J', 'H'), ('E', 'J'), ('M', 'B'), ('C', 'E'), ('A', 'I'), ('F', 'A')]

    initial_arrangement_7 = [['J', 'B', 'A', 'E', 'O', 'N'], ['I', 'G', 'K', 'P', 'Q', 'D', 'L', 'H'], ['F'], ['M', 'C']]
    goal_arrangement_7 = [['O', 'E', 'H', 'L'], ['A'], ['M', 'I', 'F'], ['B', 'K', 'G', 'N'], ['P'], ['J', 'Q', 'D'], ['C']] 
    optimal_movements_7  = [('C', 'Table'), ('N', 'Table'), ('O', 'Table'), ('E', 'O'), ('A', 'Table'), ('H', 'E'), ('L', 'H'), ('B', 'Table'), ('D', 'Table'), ('Q', 'J'), ('D', 'Q'), ('P', 'Table'), ('K', 'B'), ('G', 'K'), ('N', 'G'), ('I', 'M'), ('F', 'I')]

    initial_arrangement_8 = [['O', 'B', 'D'], ['F', 'Q', 'I', 'P', 'A'], ['C', 'E', 'M', 'G'], ['R', 'N', 'L'], ['H', 'J'], ['K']]
    goal_arrangement_8 = [['M', 'G', 'I', 'O'], ['J', 'L', 'P'], ['H', 'Q', 'D', 'E', 'C', 'B'], ['N', 'K'], ['R', 'A', 'F']] 
    optimal_movements_8  = [('J', 'Table'), ('L', 'J'), ('A', 'Table'), ('P', 'L'), ('I', 'Table'), ('Q', 'H'), ('D', 'Q'), ('N', 'Table'), ('K', 'N'), ('A', 'R'), ('F', 'A'), ('G', 'Table'), ('M', 'Table'), ('E', 'D'), ('C', 'E'), ('B', 'C'), ('G', 'M'), ('I', 'G'), ('O', 'I')]

    initial_arrangement_9 = [['C', 'B', 'F', 'O', 'D'], ['I', 'L', 'H', 'G', 'P', 'K', 'N'], ['J', 'E'], ['A', 'M']]
    goal_arrangement_9 = [['A', 'K'], ['C', 'G', 'F', 'O'], ['D', 'H', 'J'], ['B', 'I'], ['M', 'P'], ['E', 'L'], ['N']] 
    optimal_movements_9  = [('N', 'Table'), ('M', 'Table'), ('K', 'A'), ('P', 'M'), ('D', 'Table'), ('G', 'Table'), ('H', 'D'), ('E', 'Table'), ('L', 'E'), ('J', 'H'), ('O', 'Table'), ('F', 'Table'), ('O', 'F'), ('B', 'Table'), ('I', 'B'), ('G', 'C'), ('O', 'Table'), ('F', 'G'), ('O', 'F')]

    initial_arrangement_10 = [['J', 'H', 'I', 'D', 'O', 'P', 'B', 'F', 'G', 'C', 'E', 'K', 'A'], ['Q', 'L'], ['N', 'M']] 
    goal_arrangement_10 = [['P', 'N', 'H', 'C'], ['O', 'K', 'Q', 'J', 'D', 'B', 'I'], ['M', 'E', 'A'], ['G', 'L'], ['F']]
    optimal_movements_10  = [('A', 'Table'), ('K', 'Table'), ('M', 'Table'), ('E', 'M'), ('A', 'E'), ('C', 'Table'), ('G', 'Table'), ('L', 'G'), ('F', 'Table'), ('B', 'Table'), ('P', 'Table'), ('N', 'P'), ('O', 'Table'), ('K', 'O'), ('Q', 'K'), ('D', 'Table'), ('I', 'Table'), ('H', 'N'), ('C', 'H'), ('J', 'Q'), ('D', 'J'), ('B', 'D'), ('I', 'B')]

    initial_arrangement_11 = [['A', 'B', 'C'], ['D', 'E']] 
    goal_arrangement_11 = [['D', 'E', 'A', 'B', 'C']]
    optimal_movements_11 = [('C', 'Table'), ('B', 'Table'), ('A', 'E'), ('B', 'A'), ('C', 'B')]

    initial_arrangement_12 = [['A', 'B', 'C'], ['D', 'E', 'F'], ['G', 'H', 'I']] 
    goal_arrangement_12 = [['I', 'H', 'G', 'F', 'E', 'D', 'C', 'B', 'A']]
    optimal_movements_12  = [('I', 'Table'), ('H', 'I'), ('G', 'H'), ('F', 'G'), ('E', 'F'), ('D', 'E'), ('C', 'D'), ('B', 'C'), ('A', 'B')]

    initial_arrangement_13 = [['A', 'B', 'C'], ['D', 'E', 'F'], ['G', 'H', 'I']]  
    goal_arrangement_13 = [['F', 'D', 'C', 'I', 'G', 'A'], ['B', 'E', 'H']]
    optimal_movements_13  = [('F', 'Table'), ('E', 'Table'), ('D', 'F'), ('C', 'D'), ('I', 'C'), ('B', 'Table'), ('E', 'B'), ('H', 'Table'), ('H', 'E'), ('G', 'I'), ('A', 'G')]

"""





def main(args=None) : 
    rclpy.init(args=args) 
    node = Taskpublisher() 
    rclpy.spin(node)
    rclpy.shutdown()