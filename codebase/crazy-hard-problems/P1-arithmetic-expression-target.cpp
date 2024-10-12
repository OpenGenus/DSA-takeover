/* **************************************************************************
 * Copyright 2024 The OpenGenus.org Authors. All Rights Reserved.
 *
 * Code for the book "DSA Takeover"
 *
 * Licensed under the GNU General Public License, Version 3.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.gnu.org/licenses/gpl-3.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For details about the book, please visit: https://www.amazon.com/dp/B0DJDXNXS7
 * *************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <stdexcept>

using namespace std;

// The operators that we will use in expressions
const char OPERATORS[] = {'+', '-', '*', '/'};

// ASTNode -> NumberNode
// ASTNode -> OperatorNode

// Abstract base class representing an AST (Abstract Syntax Tree) node
class ASTNode {
public:
    // Virtual function for evaluating the node
    virtual int evaluate() = 0;
    // Virtual function for converting the expression to a string
    virtual string toString() = 0;
    virtual ~ASTNode() = default;
};

// NumberNode class for storing numbers in the AST
class NumberNode : public ASTNode {
    int value;

public:
    NumberNode(int value) : value(value) {}

    // Returns the value of the node
    int evaluate() override {
        return value;
    }

    // Converts the number to a string
    string toString() override {
        return to_string(value);
    }
};

// OperatorNode class for storing an operator and two child nodes (left and right)
class OperatorNode : public ASTNode {
    char oper;
    ASTNode* left;
    ASTNode* right;

public:
    OperatorNode(char oper, ASTNode* left, ASTNode* right) 
        : oper(oper), left(left), right(right) {}

    // Evaluates the left and right nodes and applies the operator
    int evaluate() override {
        int leftVal = left->evaluate();
        int rightVal = right->evaluate();
        switch (oper) {
            case '+': return leftVal + rightVal;
            case '-': return leftVal - rightVal;
            case '*': return leftVal * rightVal;
            case '/': return rightVal != 0 ? leftVal / rightVal : INT_MAX;
            default: throw invalid_argument("Invalid operator");
        }
    }

    // Converts the operator expression to a string
    string toString() override {
        return "(" + left->toString() + oper + right->toString() + ")";
    }

    ~OperatorNode() {
        delete left;  // Safely delete left and right children
        delete right;
    }
};

// Function to recursively generate all possible expressions from a subset of nums
vector<ASTNode*> generateExpressions(const vector<int>& nums, int start, int end) {
    vector<ASTNode*> result;
    
    // Base case: if the start equals end, return a single number node
    if (start == end) {
        result.push_back(new NumberNode(nums[start]));
        return result;
    }
    
    // Recursive case: divide the array into left and right subexpressions
    for (int i = start; i < end; i++) {
        
        vector<ASTNode*> leftExpressions = generateExpressions(nums, start, i);
        vector<ASTNode*> rightExpressions = generateExpressions(nums, i + 1, end);

        // Combine left and right expressions with each operator
        for (ASTNode* left : leftExpressions) {
            for (ASTNode* right : rightExpressions) {
                for (char oper : OPERATORS) {
                    result.push_back(new OperatorNode(oper, left, right));
                }
            }
        }
    }
    
    return result;
}

// Helper function to generate expressions and filter those that meet the target
void generateExpressions(const vector<int>& nums, int start, int end, int target, vector<string>& result) {
    vector<ASTNode*> expressions = generateExpressions(nums, start, end);
    // Filter out expressions that evaluate to the target and add their string form to the result
    for (ASTNode* expression : expressions) {
        // Comment if condition to generate all expressions
        if (expression->evaluate() == target) {
            result.push_back(expression->toString());
        }
        // Safely delete expressions after evaluation
        // delete expression;
    }
}

// Function to find all valid expressions that evaluate to the target value
vector<string> findExpressions(const vector<int>& nums, int target) {
    vector<string> result;
    generateExpressions(nums, 0, nums.size() - 1, target, result);
    return result;
}

// Main function to test the solution
int main() {
    vector<int> nums = {2, 3, 4};
    int target = 20;

    // Find all valid expressions
    vector<string> expressions = findExpressions(nums, target);

    // Print the expressions that evaluate to the target
    for (const string& expr : expressions) {
        cout << expr << endl;
    }

    return 0;
}
