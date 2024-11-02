#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <string>
#include <map>
#include <cctype>
#include <stdexcept>

using namespace std;

int precedence(const string& op) {
    if (op == "||") return 1;
    if (op == "&&") return 2;
    if (op == ">" || op == "<" || op == ">=" || op == "<=" || op == "==" || op == "!=") return 3;
    if (op == "+" || op == "-") return 4;
    if (op == "*" || op == "/" || op == "%") return 5;
    if (op == "!") return 6;
    return 0;
}

double applyOp(const string& op, double a, double b = 0) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return a / b;
    if (op == "%") return static_cast<int>(a) % static_cast<int>(b);
    if (op == "&&") return static_cast<bool>(a) && static_cast<bool>(b);
    if (op == "||") return static_cast<bool>(a) || static_cast<bool>(b);
    if (op == ">") return a > b;
    if (op == "<") return a < b;
    if (op == ">=") return a >= b;
    if (op == "<=") return a <= b;
    if (op == "==") return a == b;
    if (op == "!=") return a != b;
    if (op == "!") return !static_cast<bool>(a);
    throw runtime_error("Unknown operator");
}

vector<string> infixToPostfix(const vector<string>& tokens) {
    stack<string> ops;
    vector<string> postfix;
    for (const auto& token : tokens) {
        if (isdigit(token[0]) || token == "true" || token == "false" || (token.size() == 1 && isalpha(token[0]))) {
            postfix.push_back(token);
        } else if (token == "(") {
            ops.push(token);
        } else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") {
                postfix.push_back(ops.top());
                ops.pop();
            }
            ops.pop();
        } else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(token)) {
                postfix.push_back(ops.top());
                ops.pop();
            }
            ops.push(token);
        }
    }
    while (!ops.empty()) {
        postfix.push_back(ops.top());
        ops.pop();
    }
    return postfix;
}

double evaluatePostfix(const vector<string>& postfix) {
    stack<double> values;
    for (const auto& token : postfix) {
        if (isdigit(token[0])) {
            values.push(stod(token));
        } else if (token == "true") {
            values.push(1);
        } else if (token == "false") {
            values.push(0);
        } else if (token.size() == 1 && isalpha(token[0])) {
            values.push(static_cast<double>(token[0]));
        } else if (token == "!") {
            double a = values.top(); values.pop();
            values.push(applyOp(token, a));
        } else {
            double b = values.top(); values.pop();
            double a = values.top(); values.pop();
            values.push(applyOp(token, a, b));
        }
    }
    return values.top();
}

vector<string> tokenize(const string& expr) {
    vector<string> tokens;
    stringstream ss;
    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];
        if (isspace(ch)) continue;
        if (isdigit(ch) || ch == '.') {
            ss << ch;
        } else {
            if (!ss.str().empty()) {
                tokens.push_back(ss.str());
                ss.str("");
            }
            if (ch == '(' || ch == ')') {
                tokens.push_back(string(1, ch));
            } else if (ch == '!' && (i == 0 || expr[i-1] == '(')) {
                tokens.push_back("!");
            } else if (ch == '&' || ch == '|') {
                if (expr[i + 1] == ch) {
                    tokens.push_back(string(2, ch));
                    i++;
                }
            } else if (ch == '>' || ch == '<' || ch == '=' || ch == '!') {
                if (expr[i + 1] == '=') {
                    tokens.push_back(string(1, ch) + "=");
                    i++;
                } else {
                    tokens.push_back(string(1, ch));
                }
            } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') {
                tokens.push_back(string(1, ch));
            } else if (expr.substr(i, 4) == "true") {
                tokens.push_back("true");
                i += 3;
            } else if (expr.substr(i, 5) == "false") {
                tokens.push_back("false");
                i += 4;
            } else if (ch == '\'' && i + 2 < expr.length() && expr[i + 2] == '\'') {
                tokens.push_back(string(1, expr[i + 1]));
                i += 2;
            }
        }
    }
    if (!ss.str().empty()) tokens.push_back(ss.str());
    return tokens;
}

// 返回类型为 std::pair<double, string>
pair<double, string> evaluateExpression(const string& expr) {
    vector<string> tokens = tokenize(expr);
    vector<string> postfix = infixToPostfix(tokens);
    double result = evaluatePostfix(postfix);

    // 判断结果类型
    string type;

    // 检查是否包含逻辑或关系运算符
    bool hasLogicalOrRelationalOperator = false;
    bool containsCharacter = false;
    for (const auto& token : tokens) {
        if (token == "==" || token == "!=" || token == "<" || token == ">" || token == "<=" || token == ">=" || token == "&&" || token == "||") {
            hasLogicalOrRelationalOperator = true;
        }
        if (token.size() == 1 && isalpha(token[0]) && token != "true" && token != "false") {
            containsCharacter = true;
        }
    }

    if (hasLogicalOrRelationalOperator) {
        type = "bool";
    } else if (static_cast<int>(result) == result) {
        if (containsCharacter && result >= 0 && result <= 255) {
            type = "char";  // 表示可能的 ASCII 字符
        } else {
            type = "int";
        }
    } else {
        type = "double";
    }
    return make_pair(result, type);
}

int main() {
    string expr;
    cout << "请输入表达式: ";
    getline(cin, expr);

    try {
        pair<double, string> result = evaluateExpression(expr);
        cout << "计算结果: " << result.first << ", 类型: " << result.second << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}





